#ifndef SERVER_H
#define SERVER_H

#include <QTcpServer>
#include <QObject>
#include "tcpclientsocket.h"
#include <QTimer>


class TCPServer : public QTcpServer
{
    Q_OBJECT
public:
    TCPServer(QObject *parent=0,int port=0);
    QList<TcpClientSocket*> tcpClientSocketList;
    QTimer *timer;


signals:
    void updateServer(QString,int);

    void Msg_log(QString &strLog,int type,bool display);
public slots:
    void updateClients(QString,int);
    void slotDisconnected(int);
    void update();

public:
    void SendMsg(const char *pdata,int length);
    void Log(QString &strLog,int type,bool display);
                void StartListen();
                int  ConnectedNum();
protected:
    void incomingConnection(qintptr socketDescriptor);
};

#endif // SERVER



