#include <QtWidgets/QApplication>
#include <sodium.h>

#include "ui/MainWindow.h"
#include "ui/UnlockDialog.h"

int main(int argc, char* argv[]) {
    if (sodium_init() < 0) {
        return 1;
    }

    QApplication app(argc, argv);

    // 1) Ask for master password
    UnlockDialog unlock;
    if (unlock.exec() != QDialog::Accepted) {
        return 0; // user cancelled
    }

    QString passwordQt = unlock.password();
    std::string masterPassword = passwordQt.toStdString();

    // 2) Show main window, passing the password
    MainWindow w(masterPassword);
    w.show();

    return app.exec();
}
