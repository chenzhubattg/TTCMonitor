#ifndef TCPCLIENT_H
#define TCPCLIENT_H
#include <QHostAddress>
#include <QTcpSocket>
#include "common.h"
class TCPClient:QObject
{
    Q_OBJECT
    
public:
    TCPClient(QString  serverIpAdress,int serverPort);
    ~TCPClient();
public:
    bool status;
    QString strServerAddress;
    int     nServerPort;
    QHostAddress *serverIP;
    QString userName;
    QTcpSocket *tcpSocket;
     QByteArray 		dataFlow;
public slots:
    void slotEnter();
    void slotConnected();
    void slotDisconnected();
    void dataReceived();
    void slotSend();
public:
   void readMsgBody(stFrameHeader &header,char * body,int bodyLength);
};

#endif // TCPCLIENT_H
