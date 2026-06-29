#include "ConfigManager.h"
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>

#define CONFIG_FILE "/opt/aicTrain/sMonitor/sMonitor.cfg"

ConfigManager::ConfigManager()
    : m_storagePath("/opt/aicTrain/sMonitor/video")
    , m_autoRecord(false)
    , m_retainDays(30)
    , m_diskFullPolicy(0)
{
}

void ConfigManager::loadConfig()
{
    QFile file(CONFIG_FILE);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return;
    }

    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine().trimmed();
        if (line.isEmpty() || line.startsWith('#')) continue;
        int eq = line.indexOf('='); if (eq < 0) continue;
        QString key = line.left(eq).trimmed(), val = line.mid(eq + 1).trimmed();

        if (key == "storagePath")    m_storagePath = val;
        if (key == "autoRecord")     m_autoRecord = (val == "true");
        if (key == "retainDays")     m_retainDays = val.toInt();
        if (key == "diskFullPolicy") m_diskFullPolicy = val.toInt();
    }
    file.close();
}

void ConfigManager::saveConfig()
{
    QString tmpFile = QString(CONFIG_FILE) + ".tmp";
    QFile file(tmpFile);

    if (!file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate)) {
        qWarning() << "Failed to open temporary config file for writing:" << tmpFile;
        return;
    }

    QTextStream out(&file);
    out << "storagePath=" << m_storagePath << "\n"
        << "autoRecord=" << (m_autoRecord ? "true" : "false") << "\n"
        << "retainDays=" << m_retainDays << "\n"
        << "diskFullPolicy=" << m_diskFullPolicy << "\n";

    // Flush the stream to the QIODevice
    out.flush();

    // Get the underlying file descriptor and ensure it's written to disk
    int fd = file.handle();
    if (fd != -1) {
        fsync(fd);
    }
    file.close();

    // Atomic rename
    if (rename(tmpFile.toLocal8Bit().constData(), CONFIG_FILE) != 0) {
        qWarning() << "Atomic rename of config file failed.";
    } else {
        emit configChanged();
    }
}
