#include "mqttmanager.h"
#include <QDebug>
#include <QtEndian>

MqttManager::MqttManager(QObject *parent)
    : QObject(parent)
{
    m_socket = new QTcpSocket(this);
    connect(m_socket, &QTcpSocket::connected, this, &MqttManager::onSocketConnected);
    connect(m_socket, &QTcpSocket::disconnected, this, &MqttManager::onSocketDisconnected);
    connect(m_socket, &QTcpSocket::readyRead, this, &MqttManager::onSocketReadyRead);
    connect(m_socket, static_cast<void (QTcpSocket::*)(QAbstractSocket::SocketError)>(&QTcpSocket::error),
            this, &MqttManager::onSocketError);

    m_pingTimer = new QTimer(this);
    connect(m_pingTimer, &QTimer::timeout, this, &MqttManager::onPingTimer);
}

MqttManager::~MqttManager()
{
    disconnect();
}

void MqttManager::connectToBroker(const QString &host, quint16 port)
{
    m_host = host;
    m_port = port;
    m_buffer.clear();
    qDebug() << "MQTT connecting to" << host << ":" << port;
    m_socket->connectToHost(host, port);
}

void MqttManager::disconnect()
{
    if (m_socket->state() != QAbstractSocket::UnconnectedState) {
        QByteArray frame;
        frame.append((char)0xE0);
        frame.append((char)0x00);
        m_socket->write(frame);
        m_socket->flush();
    }
    m_pingTimer->stop();
    m_socket->disconnectFromHost();
    m_connected = false;
}

bool MqttManager::isConnected() const
{
    return m_connected;
}

void MqttManager::publish(const QString &topic, const QByteArray &payload, int qos)
{
    if (!m_connected) return;

    QByteArray topicData = topic.toUtf8();
    QByteArray frame;
    int remaining = 2 + topicData.size() + payload.size();

    // Fixed header
    frame.append(0x30 | (qos << 1));
    // Remaining length
    do {
        char byte = remaining % 128;
        remaining /= 128;
        if (remaining > 0) byte |= 0x80;
        frame.append(byte);
    } while (remaining > 0);

    // Variable header: topic length + topic
    quint16 topicLen = qToBigEndian((quint16)topicData.size());
    frame.append(reinterpret_cast<const char*>(&topicLen), 2);
    frame.append(topicData);

    // Payload
    frame.append(payload);

    m_socket->write(frame);
}

void MqttManager::subscribe(const QString &topic, int qos)
{
    if (!m_connected) return;

    QByteArray topicData = topic.toUtf8();
    int remaining = 2 + 2 + topicData.size() + 1;

    QByteArray frame;
    frame.append(SUBSCRIBE);
    do {
        char byte = remaining % 128;
        remaining /= 128;
        if (remaining > 0) byte |= 0x80;
        frame.append(byte);
    } while (remaining > 0);

    // Packet identifier
    quint16 msgId = qToBigEndian(m_msgId++);
    frame.append(reinterpret_cast<const char*>(&msgId), 2);

    // Topic filter
    quint16 topicLen = qToBigEndian((quint16)topicData.size());
    frame.append(reinterpret_cast<const char*>(&topicLen), 2);
    frame.append(topicData);
    frame.append((char)qos);

    m_socket->write(frame);
    qDebug() << "MQTT subscribed:" << topic;
}

void MqttManager::onSocketConnected()
{
    qDebug() << "MQTT socket connected";
    sendConnect();
}

void MqttManager::onSocketDisconnected()
{
    m_connected = false;
    m_pingTimer->stop();
    qDebug() << "MQTT disconnected";
    emit disconnected();
}

void MqttManager::onSocketReadyRead()
{
    m_buffer.append(m_socket->readAll());

    // Simple MQTT frame parser — one frame at a time
    while (m_buffer.size() >= 2) {
        unsigned char header = (unsigned char)m_buffer.at(0);
        int packetType = header & 0xF0;

        // Decode remaining length
        int remainingLen = 0;
        int multiplier = 1;
        int offset = 1;
        while (offset < m_buffer.size() && offset <= 4) {
            unsigned char byte = (unsigned char)m_buffer.at(offset);
            remainingLen += (byte & 0x7F) * multiplier;
            offset++;
            if (!(byte & 0x80)) break;
            multiplier *= 128;
        }
        if (offset > 4 || (offset == 4 && (unsigned char)m_buffer.at(3) & 0x80)) {
            qWarning() << "MQTT: malformed remaining length";
            return;
        }

        int frameLen = offset + remainingLen;
        if (m_buffer.size() < frameLen) {
            qDebug() << "MQTT: need more data, have" << m_buffer.size() << "need" << frameLen;
            break; // incomplete frame
        }

        QByteArray payload = m_buffer.mid(offset, remainingLen);

        switch (packetType) {
        case CONNACK:  handleConnack(payload); break;
        case PUBLISH:  handlePublish(payload, 0); break;
        case SUBACK:   handleSuback(payload); break;
        case PINGRESP: handlePingResp(); break;
        default:
            qDebug() << "MQTT: unknown packet type:" << packetType;
            break;
        }

        m_buffer.remove(0, frameLen);
    }
}

void MqttManager::onSocketError(QAbstractSocket::SocketError err)
{
    Q_UNUSED(err)
    emit errorOccurred(m_socket->errorString());
}

void MqttManager::onPingTimer()
{
    sendPingReq();
}

void MqttManager::sendConnect()
{
    QByteArray payload;
    // Variable header: protocol name + version
    QString protocol = "MQTT";
    quint16 protoLen = qToBigEndian((quint16)4);
    payload.append(reinterpret_cast<const char*>(&protoLen), 2);
    payload.append("MQTT");
    payload.append((char)4); // protocol level 3.1.1

    // Connect flags: clean session
    payload.append((char)0x02);

    // Keep alive: 60 seconds
    quint16 keepAlive = qToBigEndian((quint16)60);
    payload.append(reinterpret_cast<const char*>(&keepAlive), 2);

    // Client ID
    QByteArray clientId = "sMonitorServer_Qt";
    quint16 clientLen = qToBigEndian((quint16)clientId.size());
    payload.append(reinterpret_cast<const char*>(&clientLen), 2);
    payload.append(clientId);

    // Build frame
    QByteArray frame;
    frame.append(CONNECT);
    int remaining = payload.size();
    do {
        char byte = remaining % 128;
        remaining /= 128;
        if (remaining > 0) byte |= 0x80;
        frame.append(byte);
    } while (remaining > 0);
    frame.append(payload);

    m_socket->write(frame);
    qDebug() << "MQTT CONNECT sent";
}

void MqttManager::sendPingReq()
{
    if (m_socket->state() != QAbstractSocket::ConnectedState) return;
    QByteArray frame;
    frame.append((char)0xC0);
    frame.append((char)0x00);
    m_socket->write(frame);
}

void MqttManager::handleConnack(const QByteArray &data)
{
    qDebug() << "MQTT CONNACK rcvd, size:" << data.size();
    if (data.size() < 2) return;
    unsigned char sessionPresent = (unsigned char)data.at(0);
    unsigned char connackByte = (unsigned char)data.at(1);
    qDebug() << "MQTT CONNACK sp:" << sessionPresent << "rc:" << connackByte;
    if (connackByte == 0) {
        m_connected = true;
        m_pingTimer->start(30000);
        qDebug() << "MQTT connected successfully";
        emit connected();
    } else {
        qWarning() << "MQTT CONNACK rejected:" << (int)connackByte;
        emit errorOccurred(QString("Connection rejected: code %1").arg(connackByte));
    }
}

void MqttManager::handlePublish(const QByteArray &data, int offset)
{
    if (data.size() < offset + 2) return;

    quint16 topicLen = qFromBigEndian<quint16>(reinterpret_cast<const uchar*>(data.constData() + offset));
    if (data.size() < offset + 2 + topicLen) return;

    QString topic = QString::fromUtf8(data.mid(offset + 2, topicLen));
    QByteArray payload = data.mid(offset + 2 + topicLen);

    emit messageReceived(topic, payload);
}

void MqttManager::handleSuback(const QByteArray &data)
{
    Q_UNUSED(data)
    qDebug() << "MQTT SUBACK received";
}

void MqttManager::handlePingResp()
{
    // qDebug() << "MQTT PINGRESP";
}
