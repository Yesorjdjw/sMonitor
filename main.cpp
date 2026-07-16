#include <QApplication>
#include <QCoreApplication>
#include <QFile>
#include <QDebug>
#include <QDir>

#include "loginwidget.h"
#include "mainwindow.h"
#include "mqttmanager.h"
#include "databasemanager.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setApplicationName("sMonitor_server");
    app.setApplicationVersion("1.0.0");

    // ---- Load stylesheet (look in same dir as executable) ----
    QDir::setCurrent(QCoreApplication::applicationDirPath());
    QFile qssFile("style.qss");
    if (qssFile.open(QFile::ReadOnly)) {
        QString style = qssFile.readAll();
        app.setStyleSheet(style);
        qssFile.close();
        qDebug() << "Stylesheet loaded";
    } else {
        qWarning() << "Could not load style.qss";
    }

    // ---- Ensure data directories exist ----
    QDir().mkpath("/opt/aicTrain/sMonitor_server/dat");
    QDir().mkpath("/opt/aicTrain/sMonitor_server/media/photos");
    QDir().mkpath("/opt/aicTrain/sMonitor_server/media/recordings");

    // ---- Initialize database ----
    // 媒体列表读中心库(Python 中心服务写入);用户/登录仍用各自的 server.db
    DatabaseManager db("/opt/aicTrain/central/media.db");
    if (!db.initDatabase()) {
        qCritical() << "Database init failed!";
    }

    // ---- Create MQTT manager ----
    MqttManager mqtt;
    mqtt.connectToBroker("127.0.0.1", 1883);

    // ---- Auto-login mode: --test flag skips login UI ----
    bool autoLogin = (argc > 1 && QString(argv[1]) == "--test");

    MainWindow *mainWin = nullptr;

    if (autoLogin) {
        qDebug() << "Auto-login mode (--test)";
        mainWin = new MainWindow("admin");
        mainWin->setMqttManager(&mqtt);
        mainWin->setDatabaseManager(&db);
        mainWin->show();
    } else {
        // ---- Login ---
        LoginWidget *login = new LoginWidget;

        QObject::connect(login, &LoginWidget::loginSuccess, [&, login](const QString &username) {
            qDebug() << "User logged in:" << username;
            login->hide();

            if (!mainWin) {
                mainWin = new MainWindow(username);
                mainWin->setMqttManager(&mqtt);
                mainWin->setDatabaseManager(&db);
            }
            mainWin->show();
        });

        login->show();
    }

    int ret = app.exec();

    // Cleanup
    mqtt.disconnect();
    delete mainWin;

    return ret;
}
