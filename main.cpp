#include "mainwidget.h"
#include "saveandopenwidget.h"
#include <QApplication>
#include <QStyle>
#include <QStyleFactory>
#include <fcntl.h>

int main(int argc, char** argv){
    QApplication * app = new QApplication(argc, argv);

    MainWidget *mw = new MainWidget();
    mw->show();

    return app->exec();
}
