#ifndef CONFIGMANAGER_H
#define CONFIGMANAGER_H

#include <QObject>
#include <QString>
#include <QTime>

// 服务端告警策略配置(简版,与 v2 sMonitor.cfg 格式兼容)
class ConfigManager : public QObject
{
    Q_OBJECT
public:
    explicit ConfigManager(const QString &cfgDir, QObject *parent = nullptr);

    void loadConfig();
    void saveConfig();

    // Alarm settings
    bool alarmMotion()  const { return m_alarmMotion; }
    bool alarmCover()   const { return m_alarmCover; }
    bool alarmIntrude() const { return m_alarmIntrude; }
    bool alarmStorage() const { return m_alarmStorage; }
    bool alarmFace()    const { return m_alarmFace; }
    int  alarmLinkage() const { return m_alarmLinkage; }
    QTime alarmTimeStart() const { return m_alarmTimeStart; }
    QTime alarmTimeEnd()   const { return m_alarmTimeEnd; }

    void setAlarmMotion(bool v)  { m_alarmMotion = v; }
    void setAlarmCover(bool v)   { m_alarmCover = v; }
    void setAlarmIntrude(bool v) { m_alarmIntrude = v; }
    void setAlarmStorage(bool v) { m_alarmStorage = v; }
    void setAlarmFace(bool v)    { m_alarmFace = v; }
    void setAlarmLinkage(int v)  { m_alarmLinkage = v; }
    void setAlarmTimeStart(const QTime &t) { m_alarmTimeStart = t; }
    void setAlarmTimeEnd(const QTime &t)   { m_alarmTimeEnd = t; }

    // TTS
    QString ttsPort()    const { return m_ttsPort; }
    int     ttsBaud()    const { return m_ttsBaud; }
    QString ttsSpeaker() const { return m_ttsSpeaker; }
    int     ttsVolume()  const { return m_ttsVolume; }
    int     ttsSpeed()   const { return m_ttsSpeed; }
    bool ttsLoopEnabled()  const { return m_ttsLoopEnabled; }
    int  ttsLoopCount()    const { return m_ttsLoopCount; }
    int  ttsLoopInterval() const { return m_ttsLoopInterval; }
    void setTtsPort(const QString &v)    { m_ttsPort = v; }
    void setTtsBaud(int v)               { m_ttsBaud = v; }
    void setTtsSpeaker(const QString &v) { m_ttsSpeaker = v; }
    void setTtsVolume(int v)             { m_ttsVolume = v; }
    void setTtsSpeed(int v)              { m_ttsSpeed = v; }
    void setTtsLoopEnabled(bool v)       { m_ttsLoopEnabled = v; }
    void setTtsLoopCount(int v)          { m_ttsLoopCount = v; }
    void setTtsLoopInterval(int v)       { m_ttsLoopInterval = v; }

    // 序列化为 JSON(供 MQTT 下发到 v2)
    QString toJson() const;

signals:
    void configChanged();

private:
    QString m_cfgPath;
    bool m_alarmMotion   = true;
    bool m_alarmCover    = true;
    bool m_alarmIntrude  = true;
    bool m_alarmStorage  = true;
    bool m_alarmFace     = true;
    int  m_alarmLinkage  = 0;
    QTime m_alarmTimeStart{0, 0};
    QTime m_alarmTimeEnd{23, 59};
    QString m_ttsPort    = "/dev/ttySAC2";
    int     m_ttsBaud    = 9600;
    QString m_ttsSpeaker = "m3";
    int     m_ttsVolume  = 5;
    int     m_ttsSpeed   = 5;
    bool    m_ttsLoopEnabled  = true;
    int     m_ttsLoopCount    = 2;
    int     m_ttsLoopInterval = 10;
};

#endif // CONFIGMANAGER_H
