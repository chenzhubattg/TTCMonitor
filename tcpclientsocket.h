#ifndef TCPCLIENTSOCKET_H
#define TCPCLIENTSOCKET_H
#include <QTcpSocket>
#include <QObject>
#include"common.h"

class TcpClientSocket : public QTcpSocket
{
    Q_OBJECT
public:
    TcpClientSocket(QObject *parent=0);
    QByteArray 		dataFlow;
signals:
    void updateClients(QString,int);
    void disconnected(int);
protected slots:
    void dataReceived();
    void slotDisconnected();
public:
    void readMsgBody(stFrameHeader &header,char * body,int bodyLength);
};

#endif // TCPCLIENTSOCKET_H
