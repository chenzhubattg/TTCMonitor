#include "TCPServer.h"
#include <iostream>
#include <QDebug>
#include "treewidget.h"
using namespace std;
extern treeWidget *pWidget;
TCPServer::TCPServer(QObject *parent,int port)
    :QTcpServer(parent)
{



//    if(listen(QHostAddress::Any,port))
//    {
//        cout << "Watcher start listening" << endl;
//    }
//    else
//    {
 //       cout << "Listening is failed! " << endl;
 //   }
}
void TCPServer::StartListen()
{
    QString strLog = "Start Listening.port:" + QString::number(glbLocalPort) ;
    Log(strLog,0,false);
    if(listen(QHostAddress::Any,glbLocalPort))
        {
            cout << "Watcher start listening" << endl;
        }
        else
        {
           cout << "Listening is failed! " << endl;
       }
}

void TCPServer::incomingConnection(qintptr socketDescriptor)
{
    TcpClientSocket *tcpClientSocket=new TcpClientSocket(this);
    connect(tcpClientSocket,SIGNAL(updateClients(QString,int)),this,SLOT(updateClients(QString,int)));
    connect(tcpClientSocket,SIGNAL(disconnected(int)),this,SLOT(slotDisconnected(int)));
    connect(tcpClientSocket,SIGNAL(readyRead()),this,SLOT(dataReceived()));
    tcpClientSocket->setSocketDescriptor(socketDescriptor);
    QHostAddress	 add = tcpClientSocket->localAddress();
    quint16 port = tcpClientSocket->localPort();
    qDebug() << add.toString() << " port:"<< port;
    tcpClientSocketList.append(tcpClientSocket);

    QString strLog = "IP " + tcpClientSocket->peerAddress().toString() + " connect this server.";
    Log(strLog,0,false);


   // timer = new QTimer(this);
    //connect(timer, SIGNAL(timeout()), this, SLOT(update()));
  //  timer->start(1000);
}
void TCPServer::update()
{
    static int nfre = 100;
    if (nfre == 2000)
        nfre = 100;
    QTcpSocket *item = tcpClientSocketList.at(0);
    // char *pdata="abcde";
    //  item->write(pdata,6);
    QString str = QString::number(nfre);
    item->write(str.toStdString().c_str(),str.length());
    nfre += 100;

}

void TCPServer::updateClients(QString msg,int length)
{
    emit updateServer(msg,length);
    for(int i=0;i<tcpClientSocketList.count();i++)
    {
        QTcpSocket *item = tcpClientSocketList.at(i);
        if(item->write(msg.toLatin1(),length)!=length)
        {
            continue;
        }
    }
}

void TCPServer::SendMsg(const char *pdata,int length)
{
    for(int i=0;i<tcpClientSocketList.count();i++)
    {
        QTcpSocket *item = tcpClientSocketList.at(i);
        if(item->write(pdata,length)!=length)
        {
            continue;
        }
    }
}
int  TCPServer::ConnectedNum()
{
    return tcpClientSocketList.count();
}

void TCPServer::slotDisconnected(int descriptor)
{
    for(int i=0;i<tcpClientSocketList.count();i++)
    {
        QTcpSocket *item = tcpClientSocketList.at(i);
        if(item->socketDescriptor()==descriptor)
        {
            tcpClientSocketList.removeAt(i);
            return;
        }
    }
    return;
}
void TCPServer::Log(QString &strLog,int type = 0,bool display = false)
{
    //    emit Msg_log(strLog,type,display);
    pWidget->log(strLog,type,display);
}
