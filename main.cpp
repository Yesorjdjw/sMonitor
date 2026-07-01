#include "menu.h"
#include "logindlg.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setQuitOnLastWindowClosed(false);

    LoginDlg loginDlg;
    if (loginDlg.exec() == QDialog::Accepted) {
        menu w;
        w.show();
        return a.exec();
    }

    return 0;
}
