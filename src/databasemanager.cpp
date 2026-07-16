#include "databasemanager.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QDir>
#include <QFileInfo>

DatabaseManager::DatabaseManager(const QString &dbPath, QObject *parent)
    : QObject(parent), m_dbPath(dbPath)
{
}

DatabaseManager::~DatabaseManager()
{
    QString cn = m_connName;
    if (m_db.isOpen())
        m_db.close();
    m_db = QSqlDatabase();               // 释放句柄,才能安全移除连接
    if (!cn.isEmpty())
        QSqlDatabase::removeDatabase(cn);
}

bool DatabaseManager::initDatabase()
{
    // 每个实例用唯一连接名,避免临时 DatabaseManager 析构时关掉主连接
    static int s_seq = 0;
    m_connName = QString("server_conn_%1").arg(++s_seq);
    m_db = QSqlDatabase::addDatabase("QSQLITE", m_connName);
    m_db.setDatabaseName(m_dbPath);

    if (!m_db.open()) {
        qWarning() << "Failed to open database:" << m_db.lastError().text();
        return false;
    }
    QSqlQuery(m_db).exec("PRAGMA busy_timeout=3000"); // 与中心服务并发读写不冲突
    qDebug() << "Database opened:" << m_dbPath << "(" << m_connName << ")";
    return createTables();
}

bool DatabaseManager::createTables()
{
    QSqlQuery q(m_db);

    const QString users = R"(
        CREATE TABLE IF NOT EXISTS users (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            username TEXT UNIQUE NOT NULL,
            password TEXT NOT NULL,
            role TEXT DEFAULT 'operator',
            created_at TEXT DEFAULT (datetime('now','localtime'))
        )
    )";

    const QString media = R"(
        CREATE TABLE IF NOT EXISTS media_files (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            type TEXT NOT NULL,
            filename TEXT NOT NULL,
            filepath TEXT NOT NULL,
            thumbnail TEXT,
            camera_id TEXT,
            size INTEGER DEFAULT 0,
            created_at TEXT DEFAULT (datetime('now','localtime'))
        )
    )";

    const QString events = R"(
        CREATE TABLE IF NOT EXISTS events (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            type TEXT NOT NULL,
            description TEXT,
            level TEXT DEFAULT '一般',
            thumbnail TEXT DEFAULT '',
            file_id INTEGER DEFAULT 0,
            acknowledged INTEGER DEFAULT 0,
            created_at TEXT DEFAULT (datetime('now','localtime'))
        )
    )";

    if (!q.exec(users)) { qWarning() << "Create users failed:" << q.lastError().text(); return false; }
    if (!q.exec(media)) { qWarning() << "Create media_files failed:" << q.lastError().text(); return false; }
    if (!q.exec(events)) { qWarning() << "Create events failed:" << q.lastError().text(); return false; }

    // Insert default admin if not exists
    q.exec("SELECT COUNT(*) FROM users");
    if (q.next() && q.value(0).toInt() == 0) {
        QSqlQuery ins(m_db);
        ins.prepare("INSERT INTO users (username, password, role) VALUES (?, ?, ?)");
        ins.addBindValue("admin");
        ins.addBindValue("123456");
        ins.addBindValue("administrator");
        if (!ins.exec())
            qWarning() << "Insert default admin failed:" << ins.lastError().text();
    }
    return true;
}

bool DatabaseManager::checkLogin(const QString &username, const QString &password)
{
    QSqlQuery q(m_db);
    q.prepare("SELECT id, role FROM users WHERE username=? AND password=?");
    q.addBindValue(username);
    q.addBindValue(password);
    if (!q.exec()) return false;
    return q.next();
}

bool DatabaseManager::registerUser(const QString &username, const QString &password, const QString &role)
{
    QSqlQuery q(m_db);
    q.prepare("INSERT INTO users (username, password, role) VALUES (?, ?, ?)");
    q.addBindValue(username);
    q.addBindValue(password);
    q.addBindValue(role);
    return q.exec();
}

QList<MediaFile> DatabaseManager::getMediaFiles(const QString &type, int page, int pageSize)
{
    QList<MediaFile> list;
    QSqlQuery q(m_db);
    QString sql = "SELECT id, type, filename, filepath, thumbnail, camera_id, size, created_at "
                  "FROM media_files";
    QStringList params;
    if (!type.isEmpty()) {
        sql += " WHERE type=?";
        params << type;
    } else {
        sql += " WHERE type IN ('photo','recording')";
    }
    sql += " ORDER BY created_at DESC LIMIT ? OFFSET ?";
    q.prepare(sql);
    for (const auto &p : params) q.addBindValue(p);
    q.addBindValue(pageSize);
    q.addBindValue((page - 1) * pageSize);

    if (!q.exec()) return list;
    while (q.next()) {
        MediaFile f;
        f.id = q.value(0).toInt();
        f.type = q.value(1).toString();
        f.filename = q.value(2).toString();
        f.filepath = q.value(3).toString();
        f.thumbnail = q.value(4).toString();
        f.cameraId = q.value(5).toString();
        f.size = q.value(6).toLongLong();
        f.createdAt = q.value(7).toString();
        list.append(f);
    }
    return list;
}

int DatabaseManager::getMediaFileCount(const QString &type)
{
    QSqlQuery q(m_db);
    if (type.isEmpty())
        q.exec("SELECT COUNT(*) FROM media_files WHERE type IN ('photo','recording')");
    else {
        q.prepare("SELECT COUNT(*) FROM media_files WHERE type=?");
        q.addBindValue(type);
        q.exec();
    }
    return q.next() ? q.value(0).toInt() : 0;
}

bool DatabaseManager::addMediaFile(const MediaFile &file)
{
    QSqlQuery q(m_db);
    q.prepare("INSERT INTO media_files (type, filename, filepath, thumbnail, camera_id, size) "
              "VALUES (?, ?, ?, ?, ?, ?)");
    q.addBindValue(file.type);
    q.addBindValue(file.filename);
    q.addBindValue(file.filepath);
    q.addBindValue(file.thumbnail);
    q.addBindValue(file.cameraId);
    q.addBindValue(file.size);
    return q.exec();
}

bool DatabaseManager::deleteMediaFile(int id)
{
    QSqlQuery q(m_db);
    q.prepare("DELETE FROM media_files WHERE id=?");
    q.addBindValue(id);
    return q.exec();
}

QList<EventRecord> DatabaseManager::getEvents(const QString &type, int limit)
{
    QList<EventRecord> list;
    QSqlQuery q(m_db);
    QString sql = "SELECT id, type, description, level, thumbnail, file_id, acknowledged, created_at FROM events";
    if (!type.isEmpty())
        sql += " WHERE type=?";
    sql += " ORDER BY created_at DESC LIMIT ?";
    q.prepare(sql);
    if (!type.isEmpty()) q.addBindValue(type);
    q.addBindValue(limit);

    if (!q.exec()) return list;
    while (q.next()) {
        EventRecord e;
        e.id = q.value(0).toInt();
        e.type = q.value(1).toString();
        e.description = q.value(2).toString();
        e.level = q.value(3).toString();
        e.thumbnail = q.value(4).toString();
        e.fileId = q.value(5).toInt();
        e.acknowledged = q.value(6).toBool();
        e.createdAt = q.value(7).toString();
        list.append(e);
    }
    return list;
}

int DatabaseManager::getEventCount(const QString &type)
{
    QSqlQuery q(m_db);
    if (type.isEmpty())
        q.exec("SELECT COUNT(*) FROM events");
    else {
        q.prepare("SELECT COUNT(*) FROM events WHERE type=?");
        q.addBindValue(type);
        q.exec();
    }
    return q.next() ? q.value(0).toInt() : 0;
}

bool DatabaseManager::addEvent(const EventRecord &evt)
{
    QSqlQuery q(m_db);
    q.prepare("INSERT INTO events (type, description, level, thumbnail, file_id) VALUES (?, ?, ?, ?, ?)");
    q.addBindValue(evt.type);
    q.addBindValue(evt.description);
    q.addBindValue(evt.level);
    q.addBindValue(evt.thumbnail);
    q.addBindValue(evt.fileId);
    return q.exec();
}

bool DatabaseManager::acknowledgeEvent(int id)
{
    QSqlQuery q(m_db);
    q.prepare("UPDATE events SET acknowledged=1 WHERE id=?");
    q.addBindValue(id);
    return q.exec();
}

int DatabaseManager::deleteAcknowledgedEvents()
{
    QSqlQuery q(m_db);
    if (!q.exec("DELETE FROM events WHERE acknowledged=1"))
        return 0;
    return q.numRowsAffected();
}
