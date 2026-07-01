#ifndef CONFIGMANAGER_H
#define CONFIGMANAGER_H

#include <QString>
#include <QObject>

class ConfigManager : public QObject
{
    Q_OBJECT

public:
    static ConfigManager& instance()
    {
        static ConfigManager _instance;
        return _instance;
    }

    void loadConfig();
    void saveConfig();

    QString storagePath() const { return m_storagePath; }
    void setStoragePath(const QString &path) { m_storagePath = path; }

    bool autoRecord() const { return m_autoRecord; }
    void setAutoRecord(bool record) { m_autoRecord = record; }

    int retainDays() const { return m_retainDays; }
    void setRetainDays(int days) { m_retainDays = days; }

    int diskFullPolicy() const { return m_diskFullPolicy; }
    void setDiskFullPolicy(int policy) { m_diskFullPolicy = policy; }

signals:
    void configChanged();

private:
    ConfigManager();
    ~ConfigManager() = default;
    ConfigManager(const ConfigManager&) = delete;
    ConfigManager& operator=(const ConfigManager&) = delete;

    QString m_storagePath;
    bool    m_autoRecord;
    int     m_retainDays;
    int     m_diskFullPolicy;
};

#endif // CONFIGMANAGER_H
