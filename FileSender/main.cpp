
#include <QCoreApplication>
#include <QTimer>


#include "tasks.hpp"

using namespace std;

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    int isServer = -1;

    std::cout << "Server - 0, Client  - 1" << std::endl;
    std::cin >> isServer;

    QObject *task = nullptr;

    if(isServer == 0)
        task = new ServerTask(&app);
    else if(isServer == 1)
        task = new ClientTask(&app);
    else
        return 0;


    QObject::connect(task, SIGNAL(finished()), &app, SLOT(quit()));

    QTimer::singleShot(0, task, SLOT(run()));

    return app.exec();
}

