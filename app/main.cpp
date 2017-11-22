#include <QApplication>

#include "windows/MainWindow.h"
#include "stuff/Log.h"

#include "stuff/NetworkManager.h"

int main(int argc, char *argv[])
{
    Log::create();

    int result = 1;
    
    try {
        QApplication application(argc, argv);

        MainWindow mainWindow;
        mainWindow.show();

        result = application.exec();
    }
    catch (const NetworkManager::SocketErrorException& e) {
        Log::write("[ERROR]", e.what());
    }

    return result;
}
