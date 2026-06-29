#include "menu.h"
#include "ConfigManager.h"
#include <QApplication>
#include <QFile>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // Load stylesheet globally
    QFile file(":/style.qss");
    if (file.open(QFile::ReadOnly | QFile::Text)) {
        a.setStyleSheet(QString::fromLatin1(file.readAll()));
        file.close();
    }

    // Load configuration via Singleton Manager
    ConfigManager::instance().loadConfig();

    menu w;
    w.show();

    return a.exec();
}
