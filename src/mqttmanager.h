#ifndef MQTTMANAGER_H
#define MQTTMANAGER_H

#include <QObject>
#include <QTcpSocket>
#include <QTimer>
#include <QByteArray>

// MQTT Control Packet Types
#define CONNECT     0x10
#define CONNACK     0x20
#define PUBLISH     0x30
#define PUBACK      0x40
#define SUBSCRIBE   0x82
#define SUBACK      0x90
#define PINGREQ     0xC0
#define PINGRESP    0xD0
#define DISCONNECT  0xE0

class MqttManager : public QObject
{
    Q_OBJECT
public:
    explicit MqttManager(QObject *parent = nullptr);
    ~MqttManager();

    void connectToBroker(const QString &host, quint16 port = 1883);
    void disconnect();
    bool isConnected() const;

    void publish(const QString &topic, const QByteArray &payload, int qos = 0);
    void subscribe(const QString &topic, int qos = 0);

signals:
    void connected();
    void disconnected();
    void messageReceived(const QString &topic, const QByteArray &payload);
    void errorOccurred(const QString &error);

private slots:
    void onSocketConnected();
    void onSocketDisconnected();
    void onSocketReadyRead();
    void onSocketError(QAbstractSocket::SocketError err);
    void onPingTimer();

private:
    void sendConnect();
    void sendPingReq();
    void handleConnack(const QByteArray &data);
    void handlePublish(const QByteArray &data, int offset);
    void handleSuback(const QByteArray &data);
    void handlePingResp();

    QTcpSocket *m_socket = nullptr;
    QTimer *m_pingTimer = nullptr;
    QString m_host;
    quint16 m_port = 1883;
    bool m_connected = false;
    quint16 m_msgId = 1;
    QByteArray m_buffer;
};

#endif // MQTTMANAGER_H
