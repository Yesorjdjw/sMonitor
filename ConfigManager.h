#ifndef CONFIGMANAGER_H
#define CONFIGMANAGER_H

#include <QString>
#include <QObject>
#include <QReadWriteLock>

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

    QString storagePath() const { QReadLocker locker(&m_lock); return m_storagePath; }
    void setStoragePath(const QString &path) { QWriteLocker locker(&m_lock); m_storagePath = path; }

    bool autoRecord() const { QReadLocker locker(&m_lock); return m_autoRecord; }
    void setAutoRecord(bool record) { QWriteLocker locker(&m_lock); m_autoRecord = record; }

    int retainDays() const { QReadLocker locker(&m_lock); return m_retainDays; }
    void setRetainDays(int days) { QWriteLocker locker(&m_lock); m_retainDays = days; }

    int diskFullPolicy() const { QReadLocker locker(&m_lock); return m_diskFullPolicy; }
    void setDiskFullPolicy(int policy) { QWriteLocker locker(&m_lock); m_diskFullPolicy = policy; }

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

    mutable QReadWriteLock m_lock;
};

#endif // CONFIGMANAGER_H
