#include "configmanager.h"
#include <QFile>
#include <QTextStream>
#include <QJsonDocument>
#include <QJsonObject>

ConfigManager::ConfigManager(const QString &cfgDir, QObject *parent)
    : QObject(parent), m_cfgPath(cfgDir + "/sMonitor.cfg")
{
    loadConfig();
}

void ConfigManager::loadConfig()
{
    QFile file(m_cfgPath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine().trimmed();
        if (line.isEmpty() || line.startsWith('#')) continue;
        int eq = line.indexOf('=');
        if (eq < 0) continue;
        QString key = line.left(eq).trimmed();
        QString val = line.mid(eq + 1).trimmed();

        if (key == "alarmMotion")    m_alarmMotion    = (val == "true");
        if (key == "alarmCover")     m_alarmCover     = (val == "true");
        if (key == "alarmIntrude")   m_alarmIntrude   = (val == "true");
        if (key == "alarmStorage")   m_alarmStorage   = (val == "true");
        if (key == "alarmFace")      m_alarmFace      = (val == "true");
        if (key == "alarmLinkage")   m_alarmLinkage   = val.toInt();
        if (key == "alarmTimeStart") m_alarmTimeStart = QTime::fromString(val, "HH:mm");
        if (key == "alarmTimeEnd")   m_alarmTimeEnd   = QTime::fromString(val, "HH:mm");
        if (key == "ttsPort")        m_ttsPort = val;
        if (key == "ttsBaud")        m_ttsBaud = val.toInt();
        if (key == "ttsSpeaker")     m_ttsSpeaker = val;
        if (key == "ttsVolume")      m_ttsVolume = val.toInt();
        if (key == "ttsSpeed")       m_ttsSpeed = val.toInt();
        if (key == "ttsLoopEnabled")  m_ttsLoopEnabled = (val == "true");
        if (key == "ttsLoopCount")    m_ttsLoopCount = val.toInt();
        if (key == "ttsLoopInterval") m_ttsLoopInterval = val.toInt();
    }
    file.close();
}

void ConfigManager::saveConfig()
{
    QStringList lines;
    QFile oldFile(m_cfgPath);
    if (oldFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&oldFile);
        while (!in.atEnd()) {
            QString line = in.readLine().trimmed();
            if (line.isEmpty() || line.startsWith('#')) continue;
            int eq = line.indexOf('=');
            if (eq < 0) continue;
            QString key = line.left(eq).trimmed();
            if (!key.startsWith("alarm") && !key.startsWith("tts"))
                lines.append(line);
        }
        oldFile.close();
    }

    lines.append("alarmMotion=" + QString(m_alarmMotion ? "true" : "false"));
    lines.append("alarmCover=" + QString(m_alarmCover ? "true" : "false"));
    lines.append("alarmIntrude=" + QString(m_alarmIntrude ? "true" : "false"));
    lines.append("alarmStorage=" + QString(m_alarmStorage ? "true" : "false"));
    lines.append("alarmFace=" + QString(m_alarmFace ? "true" : "false"));
    lines.append("alarmLinkage=" + QString::number(m_alarmLinkage));
    lines.append("alarmTimeStart=" + m_alarmTimeStart.toString("HH:mm"));
    lines.append("alarmTimeEnd=" + m_alarmTimeEnd.toString("HH:mm"));
    lines.append("ttsPort=" + m_ttsPort);
    lines.append("ttsBaud=" + QString::number(m_ttsBaud));
    lines.append("ttsSpeaker=" + m_ttsSpeaker);
    lines.append("ttsVolume=" + QString::number(m_ttsVolume));
    lines.append("ttsSpeed=" + QString::number(m_ttsSpeed));
    lines.append("ttsLoopEnabled=" + QString(m_ttsLoopEnabled ? "true" : "false"));
    lines.append("ttsLoopCount=" + QString::number(m_ttsLoopCount));
    lines.append("ttsLoopInterval=" + QString::number(m_ttsLoopInterval));

    QFile file(m_cfgPath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate))
        return;
    QTextStream out(&file);
    for (const auto &l : lines)
        out << l << "\n";
    file.close();

    emit configChanged();
}

QString ConfigManager::toJson() const
{
    QJsonObject obj;
    obj["alarmMotion"]  = m_alarmMotion;
    obj["alarmCover"]   = m_alarmCover;
    obj["alarmIntrude"] = m_alarmIntrude;
    obj["alarmStorage"] = m_alarmStorage;
    obj["alarmFace"]    = m_alarmFace;
    obj["alarmLinkage"] = m_alarmLinkage;
    obj["alarmTimeStart"] = m_alarmTimeStart.toString("HH:mm");
    obj["alarmTimeEnd"]   = m_alarmTimeEnd.toString("HH:mm");
    obj["ttsPort"]        = m_ttsPort;
    obj["ttsBaud"]        = m_ttsBaud;
    obj["ttsSpeaker"]     = m_ttsSpeaker;
    obj["ttsVolume"]      = m_ttsVolume;
    obj["ttsSpeed"]       = m_ttsSpeed;
    obj["ttsLoopEnabled"]  = m_ttsLoopEnabled;
    obj["ttsLoopCount"]    = m_ttsLoopCount;
    obj["ttsLoopInterval"] = m_ttsLoopInterval;
    return QJsonDocument(obj).toJson(QJsonDocument::Compact);
}
