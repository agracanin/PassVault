#include <QtWidgets/QApplication>
#include "ui/MainWindow.h"
#include <sodium.h>


int main(int argc, char* argv[]) {
    if (sodium_init() < 0) {
        return 1;
    }

    QApplication app(argc, argv);

    MainWindow w;
    w.show();

    return app.exec();
}