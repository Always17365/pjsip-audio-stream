#include <QApplication>

#include "mainwidget.h"
#include "CAudioUtil.h"

int main(int argc, char *argv[])
{    
    QApplication a(argc, argv);

    if (0 != CAudioUtil::Init()) {
        return -1;
    }

    MainWidget* w = new MainWidget;
    w->show();

    int ret = a.exec();

    delete w;

    CAudioUtil::Clear();

    return ret;
}
