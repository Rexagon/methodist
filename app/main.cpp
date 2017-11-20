#include <QApplication>

#include "windows/MainWindow.h"
#include "stuff/Log.h"

int main(int argc, char *argv[])
{
    Log::create();
    
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
