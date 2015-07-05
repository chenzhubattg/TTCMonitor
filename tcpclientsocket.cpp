#include "tcpclientsocket.h"
#include "common.h"
#include <iostream>
#include <QFile>
using namespace std;

TcpClientSocket::TcpClientSocket(QObject *parent)
{
    connect(this,SIGNAL(readyRead()),this,SLOT(dataReceived()));
    connect(this,SIGNAL(disconnected()),this,SLOT(slotDisconnected()));
}

void TcpClientSocket::dataReceived()
{
    while(bytesAvailable()>0)
    {
        dataFlow.append(this->readAll());
        //qDebug()<<"data length:"<<dataFlow.length();
        char *p=dataFlow.data();
        char *q=p+dataFlow.size();

        while(true){
            //1.读Header
            if(q-p<sizeof(stFrameHeader))break ;	//帧头不完整包，退出
                stFrameHeader header;
                memcpy(&header, p, sizeof(stFrameHeader));
                //header.print();

            if(header.FLAG[0]!='M' || header.FLAG[1]!='M' || header.FLAG[2]!='X' || header.FLAG[3]!='H'){
                p=q;		//Header不对，丢弃当前所有缓存
                break ;
            }
            //2.读BODY,并移动流指针
            if(q-p<sizeof(stFrameHeader)+header.DATA_LEN)break ;//内容不完整包，退出
                readMsgBody(header, p+sizeof(stFrameHeader),header.DATA_LEN);
                p+=sizeof(stFrameHeader)+header.DATA_LEN;
        }
        dataFlow=dataFlow.right( q - p);
    }
}


void TcpClientSocket::readMsgBody(stFrameHeader &header,char * body,int bodyLength)
{
  /*
    qint16 cmd = header.cmd;
    switch (cmd) {
    case CMD_LOAD:
    {

        break;
    }
    case CMD_KILL:
    {

    }
    case CMD_START:
    {

     //   pserver->SendMsg();
        break;
    }
    case CMD_STOP:
        break;
    case CMD_SET:
    {
        cout << "Info:Receive CMD_SET cmd!!!" << endl;
        char FileName[56];
        memcpy(FileName,body,56);
        QString XMLFileName(FileName);
        QFile paraFile(XMLFileName);
        QString strlog;
        if (!paraFile.open(QIODevice::ReadWrite | QIODevice::Truncate))
        {
            strlog = "Error:Can't open" + XMLFileName +"file!!!" ;
            cout << strlog.toStdString().c_str() << endl;
            return ;
        }
        char *pdata = body + 56;
        if ((bodyLength - 56) != paraFile.write(pdata,bodyLength - 56))
        {
            cout <<  "Error:Write param info failed!!!" << endl;
            paraFile.close();
            return;
        }
        else
        {
            cout << "Info:Write para info to para file successuflly!!!" << endl;
        }
       // stFrameHeader  da;
       // da.cmd = CMD_SET;
        //da.DATA_LEN = 0;
        //pserver->SendMsg((char *)&da,sizeof(stFrameHeader));
        //cout << "Info:send Set Param cmd to radio!!!" << endl;
        break;
    }
    default:
        break;
    }*/
}

void TcpClientSocket::slotDisconnected()
{
    emit disconnected(this->socketDescriptor());
}
