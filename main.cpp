#include "treewidget.h"
#include <QApplication>
#include <QVBoxLayout>
#include <QPushButton>
#include "TCPServer.h"
treeWidget *pWidget;
TCPServer *pServer;

int main(int argc, char *argv[])
{
   //  InitSystemParam();
    QApplication a(argc, argv);
    pServer = new TCPServer(NULL,glbLocalPort);
    pWidget = new treeWidget;
    pWidget->show();
     return a.exec();
}
