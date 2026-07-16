#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <QObject>
#include <QSqlDatabase>
#include <QStringList>

struct MediaFile {
    int id = 0;
    QString type;       // "photo" | "recording"
    QString filename;
    QString filepath;
    QString thumbnail;
    QString cameraId;
    qint64 size = 0;
    QString createdAt;
};

struct EventRecord {
    int id = 0;
    QString type;       // cover, intrude, motion, storage, face, unregistered_face
    QString description;
    QString level;      // 紧急, 重要, 一般
    QString thumbnail;  // 缩略图路径 (HTTP上传后的文件名)
    int fileId = 0;
    bool acknowledged = false;
    QString createdAt;
};

class DatabaseManager : public QObject
{
    Q_OBJECT
public:
    explicit DatabaseManager(const QString &dbPath, QObject *parent = nullptr);
    ~DatabaseManager();

    bool initDatabase();

    // users
    bool checkLogin(const QString &username, const QString &password);
    bool registerUser(const QString &username, const QString &password, const QString &role = "operator");

    // media_files
    QList<MediaFile> getMediaFiles(const QString &type = "",
                                   int page = 1, int pageSize = 9);
    int getMediaFileCount(const QString &type = "");
    bool addMediaFile(const MediaFile &file);
    bool deleteMediaFile(int id);

    // events
    QList<EventRecord> getEvents(const QString &type = "", int limit = 100);
    int getEventCount(const QString &type = "");
    bool addEvent(const EventRecord &evt);
    bool acknowledgeEvent(int id);
    int deleteAcknowledgedEvents();

private:
    bool createTables();
    QSqlDatabase m_db;
    QString m_dbPath;
    QString m_connName;   // 每个实例独立的连接名,避免相互关闭
};

#endif // DATABASEMANAGER_H
