#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>

class MonitorWidget;
class FileManagerWidget;
class AlarmWidget;
class SetupWidget;
class MqttManager;
class DatabaseManager;
class ConfigManager;

namespace Ui { class MainWindow; }

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(const QString &username, QWidget *parent = nullptr);
    ~MainWindow();

    void setMqttManager(MqttManager *mgr);
    void setDatabaseManager(DatabaseManager *db);

    MonitorWidget *monitorWidget() const;
    FileManagerWidget *fileWidget() const;

private slots:
    void switchPage(int index);
    void updateClock();
    void onMqttMessage(const QString &topic, const QByteArray &payload);
    void onControlCommand(const QString &cmd);

private:
    void drawLogo(QPainter &p, const QRect &r) const;
    void setupPages();
    void setupConnections();
    void subscribeTopics();
    void triggerVoiceAlert(const QString &type, const QString &description);

    Ui::MainWindow *ui;
    QTimer *m_clockTimer;
    MonitorWidget *m_monitorWidget;
    FileManagerWidget *m_fileWidget;
    AlarmWidget *m_alarmWidget = nullptr;
    SetupWidget *m_setupWidget = nullptr;
    MqttManager *m_mqtt = nullptr;
    DatabaseManager *m_db = nullptr;
    ConfigManager *m_cfg = nullptr;
    QString m_username;
    bool m_recording = false;
};

#endif // MAINWINDOW_H
