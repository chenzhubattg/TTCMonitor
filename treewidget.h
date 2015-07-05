#ifndef TREEWIDGET_H
#define TREEWIDGET_H

#include <QWidget>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QStringList>
#include <QDebug>
#include <QList>
#include <QFile>
#include <QFileDialog>
#include <QDir>
#include <QMessageBox>
#include <tinyxml/tinystr.h>
#include <tinyxml/tinyxml.h>
#include <QTcpSocket>
#include <QHostAddress>
#include "TCPServer.h"
#include <QIcon>
#include <QTimer>

#include<vector>
#include<map>
using namespace std;


namespace Ui {
class treeWidget;
}


#define INFO 0
#define WARNING 1
#define ERROR 2

typedef struct stParam{
    QString key;
    QString name;
    QString value;
    QString type;
}PARAM;

typedef struct stStatus{
    QString key;
    QString name;
    QString value;
    QString type;
}STATUS;

typedef struct stSubBlock
{
    QString Name;
    QString ID;
    QVector <PARAM> param;
    QVector <STATUS> status;
}SUBBLOCK;

typedef struct stBlock{
    QString Name;
    QString ID;
    QVector <PARAM> bp;
    QVector <STATUS> bstatus;
    QVector <SUBBLOCK> bs;
}BLOCK;

typedef struct stGraph{
    QString Name;
    QString ID;
    QVector <PARAM> gp;
    QVector <STATUS> gstatus;
    QVector <BLOCK> gb;
}GRAPH;



class treeWidget : public QWidget
{
    Q_OBJECT

public:
    explicit treeWidget(QWidget *parent = 0);
    ~treeWidget();
    bool readXml(char *filepath);

    QTcpSocket *pSocket;


private slots:
    void on_pushButton_clicked();

    //  void on_pushButtonConnectServer_clicked();

    void on_pushButton_Listen_clicked();
    void on_pushButton_load_clicked();

    void on_pushButton_start_clicked();

    void on_pushButton_stop_clicked();

    void on_pushButton_Kill_clicked();

    void on_treeWidget_param_itemChanged(QTreeWidgetItem *item, int column);

    void on_pushButton_save_clicked();

    void on_pushButton_openXML_clicked();

    void update();

    void on_pushButton_send_clicked();

    void on_pushButton_Connect_clicked();

    void slotDisconnected();

    void on_pushButton_Disconnect_clicked();

public:
    void log(QString &str,int type,bool  display);

private:
    Ui::treeWidget *ui;
    QList<QTreeWidgetItem *> TreeWigItmsParam;
    QList<QTreeWidgetItem *> TreeWigItmsStatus;
    QStringList headers;
    QList<QString> Block_Key_List, Block_Affinity_List, Sub_Key_List;
    TiXmlDeclaration *decl;
    TiXmlElement  *graphElement,
    *blockElement,        *blockkeyElement,       *blocknameElement, *blockaffinityElement,
    *subblockElement, *subblockkeyElement, *subblocknameElement,
    *paramElement,     *paramkeyElement,      *paramnameElement, *paramvalueElement, *paramtypeElement;
    TCPServer *pserver;
    TiXmlDocument *doc3;
    QTcpSocket *tcpSocket;
    QHostAddress *serverIP;
    bool ParseXml(const char* filepath,
                              GRAPH &graph,
                              TiXmlElement* &root ,
                              TiXmlDocument * &docxml,
                              QList<QTreeWidgetItem *> &TreeWigItms,
                              QMap<QTreeWidgetItem *, TiXmlNode *> &domElementForItem,
                              int type);
    QMap<QTreeWidgetItem *, TiXmlNode *> domElementForItem_status;
    QMap<QTreeWidgetItem *, TiXmlNode *> domElementForItem_param;
    QIcon folderIcon;
    QIcon bookmarkIcon;
    QIcon statusIcon;
    TiXmlElement* rootParam ;
    TiXmlDocument *docParam;
    TiXmlElement* rootStatus ;
    TiXmlDocument *docStatus;

    QTimer *pTimer;

    GRAPH graphParam;
    GRAPH graphStatus;
};

#endif // TREEWIDGET_H
