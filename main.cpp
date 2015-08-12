#include "treewidget.h"
#include <QApplication>
#include <QVBoxLayout>
#include <QPushButton>
#include<QSettings>
#include<QDir>
#include "TCPServer.h"
treeWidget *pWidget;
//TCPServer *pServer;

int main(int argc, char *argv[])
{
  //    InitSystemParam();
    QSettings settings("Option.ini", QSettings::IniFormat); // 当前目录的INI文件
    glbServerAdress = settings.value("TTCServer/MonitorServerIpAdress").toString();
    glbServerPort = settings.value("TTCServer/MonitorServerPort").toInt();
    QApplication a(argc, argv);
//    pServer = new TCPServer(NULL,glbLocalPort);
    pWidget = new treeWidget;
    pWidget->show();
     return a.exec();
}
