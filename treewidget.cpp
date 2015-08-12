#include "treewidget.h"
#include "ui_treewidget.h"
#include <QTest>
#include <QTime>
#include <QFile>
#include<QTextStream>
#include<QMessageBox>
#include"common.h"
#include <QMessageBox>
#include <QHostAddress>
#include <QList>
#include <QRegExp>
#include <QValidator>

/*
const int CMD_LOAD = 0x0001;
const int CMD_LOAD_RES = 0x0101;
const int CMD_START = 0x0002;
const int CMD_START_RES = 0x0102;
const int CMD_STOP = 0x0003;
const int CMD_STOP_RES = 0x0203;
const int CMD_SET = 0x0004;
const int CMD_SET_RES = 0x0104;
const int CMD_KILL = 0x0005;
const int CMD_KILL_RES = 0x0105;

class stFrameHeader
{
public:
    stFrameHeader()
    {
        FLAG[0] = 'M';
        FLAG[1]= 'M';
        FLAG[2]='X';
        FLAG[3] = 'H';
        cmd = 0x11;
    }
    char FLAG[4];
    qint16 DATA_LEN;
    qint16 cmd;
};//head for msg

*/


treeWidget::treeWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::treeWidget)
{
    ui->setupUi(this);

    ui->lineEdit_IpAddress->setText(glbServerAdress);
    ui->lineEdit_Port->setText(QString::number(glbServerPort) );


    QRegExp rx_ip("(\\d{1,2}|1\\d\\d|2[0-4]\\d|25[0-5])\\.(\\d{1,2}|1\\d\\d|2[0-4]\\d|25[0-5])\\.(\\d{1,2}|1\\d\\d|2[0-4]\\d|25[0-5])\\.(\\d{1,2}|1\\d\\d|2[0-4]\\d|25[0-5])");
    ui->lineEdit_IpAddress->setValidator(new QRegExpValidator(rx_ip, this));
    ui->lineEdit_Port->setValidator(new QIntValidator(7000,65535));

    ui->pushButton_stop->setEnabled(false);

    //    //在QTreeWidget上显示树形结构
    //  ui->treeWidget_2->insertTopLevelItems(0,TreeWigItms);
    rootParam = NULL;
    docParam = NULL;
    rootStatus = NULL;
    docStatus = NULL;
    //Connect Server
//    tcpSocket = new QTcpSocket();

//    connect(tcpSocket, SIGNAL(disconnected()), this, SLOT(slotDisconnected()));
//    connect(tcpSocket, SIGNAL(readyRead()), this, SLOT(dataReceived()));


    pTimer = new QTimer(this);
    connect(pTimer, SIGNAL(timeout()), this, SLOT(update()));

    pTimer->start(1000);

    pSocket = NULL;
    // connect(pServer,SIGNAL(Msg_log(QString &,int ,bool)),this,SLOT(log(QString &,int,bool)));

}

treeWidget::~treeWidget()
{
    delete ui;
}

bool treeWidget::readXml(char* filepath)
{
    /*
    TiXmlDocument doc(filepath);
    bool loadOkay = doc.LoadFile();
    // faile to load 'phonebookdata.xml'.
    if (!loadOkay) {
        printf( "Could not load test file %s. Error='%s'. Exiting.\n", filepath, doc.ErrorDesc() );
        exit( 1 );
    }

    //     get dom root of 'phonebookdata.xml', here root should be 'phonebook'.
    TiXmlElement* root = doc.RootElement();
    printf("_______________________________________\n\n");
    printf("     contacted person information      \n\n");

    //遍历block
    for( TiXmlNode* block = root->FirstChild("block"); block; block = block->NextSibling("block") )
    {
        printf("____________________1___________________\n");

        //        // read name.
        TiXmlNode* block_child = block->FirstChild();
        const char* key = block_child->ToElement()->GetText();
        if (key) {
            printf("block_key:%s\n",key);
        } else {
            printf("block_key:\n");
        }
        Block_Key_List.append(key);

        block_child = block->IterateChildren(block_child);
        const char* name = block_child->ToElement()->GetText();
        if (name)
        {
            printf("block_name:%s\n", name);
        }
        else
        {
            printf("block_name:\n");
        }

        QTreeWidgetItem *Block_item = new QTreeWidgetItem;
        //        PareNodeItm->setFlags(PareNodeItm->flags() | Qt::ItemIsEditable);
        Block_item->setText(0, name);           				//设置节点的名称
        TreeWigItms.push_back(Block_item);


        block_child = block->IterateChildren(block_child);
        const char* affinity = block_child->ToElement()->GetText();
        if (affinity)
        {
            printf("block_affinity:%s\n", affinity);
        }
        else
        {
            printf("block_affinity:\n");
        }
        Block_Affinity_List.append(affinity);

        //遍历block中的sub_block
        for( TiXmlNode*  sub_block = block->FirstChild( "subblock" );  sub_block;  sub_block = sub_block->NextSibling( "subblock" ) )
        {
            printf("__________________2_____________________\n");

            TiXmlNode* sub_child = sub_block->FirstChild();
            const char* sub_key = sub_child->ToElement()->GetText();
            if (sub_key) {
                printf("sub_key:%s\n",sub_key);
            } else {
                printf("sub_key:\n");
            }
            Sub_Key_List.append(sub_key);

            sub_child = sub_block->IterateChildren(sub_child);
            const char* sub_name = sub_child->ToElement()->GetText();
            if (sub_name) {
                printf("sub_name:%s\n",sub_name);
            } else {
                printf("sub_name:\n");
            }

            QTreeWidgetItem *subBlock_item = new QTreeWidgetItem(Block_item);
            //            subBlock_item->setFlags(subBlock_item->flags() | Qt::ItemIsEditable);
            subBlock_item->setText(1, sub_name);              //节点名称

            //遍历sub_block中的param
            for( TiXmlNode*  param = sub_block->FirstChild("param");  param;  param = param->NextSibling("param" ) )
            {
                //                table->setRowCount(count);
                printf("_ _ _ _3 _ _ _ _ \n");

                TiXmlNode* child_param = param->FirstChild();
                const char* param_key = child_param->ToElement()->GetText();
                if (param_key)
                {
                    printf("param_key:%s\n", param_key);
                }
                else
                {
                    printf("param_key:\n");
                }

                child_param = param->IterateChildren(child_param);
                const char* param_name = child_param->ToElement()->GetText();
                if (param_name)
                {
                    printf("param_name:%s\n", param_name);
                }
                else
                {
                    printf("param_name:\n");
                }

                // read telephone no.
                child_param = param->IterateChildren(child_param);
                const char* param_value = child_param->ToElement()->GetText();
                if (param_value)
                {
                    printf("param_value:%s\n", param_value);
                }
                else
                {
                    printf("param_value:\n");
                }

                child_param = param->IterateChildren(child_param);
                const char* param_type = child_param->ToElement()->GetText();
                if (param_type)
                {
                    printf("param_type:%s\n", param_type);
                }
                else
                {
                    printf("param_type:\n");
                }

                QTreeWidgetItem *sub_param = new QTreeWidgetItem(subBlock_item);
                sub_param->setFlags(sub_param->flags() | Qt::ItemIsEditable);
                sub_param->setText(2, param_key);              //节点名称
                sub_param->setText(3, param_name);              //节点名称
                sub_param->setText(4, param_value);              //节点名称
                sub_param->setText(5, param_type);              //节点名称
            }
            printf("\n");
        }
    }*/
    return true;
}

void clearGraph(GRAPH *gp)
{
    gp->gp.clear();
    gp->gstatus.clear();
    for (int i =0 ; i < gp->gb.length(); i++)
    {
        BLOCK *p = &gp->gb[i];
        p->bp.clear();
        p->bstatus.clear();
        for (int j = 0; j < p->bs.length(); j ++)
        {
            SUBBLOCK *subblock = &p->bs[j];
            subblock->param.clear();
            subblock->status.clear();
        }
        p->bs.clear();
     }
    gp->gb.clear();
}

//解析XML文件 生成treewidget
bool treeWidget::ParseXml(const char* filepath,  GRAPH &graph,  TiXmlElement* &root ,  TiXmlDocument * &docxml,
                                               QList<QTreeWidgetItem *> &TreeWigItms,  QMap<QTreeWidgetItem *, TiXmlNode *> &domElementForItem,  int type = 0 )
{

    clearGraph(&graph);
    for (int i = TreeWigItms.length() - 1; i  >= 0; i--)
    {
        QTreeWidgetItem *p = TreeWigItms.at(i);
        delete p;
    }
    TreeWigItms.clear();
    domElementForItem.clear();
    if(docxml != NULL)
    {
        delete docxml;
        docxml = NULL;
    }


    docxml = new TiXmlDocument(filepath);
    bool loadOkay = docxml->LoadFile();

    if (!loadOkay) {
        QString strLog = QString("Could not load test file%1. Error='%2'. Exiting.\n").arg( filepath).arg(docxml->ErrorDesc());
        QMessageBox msgBox;
        msgBox.setText(strLog);
        msgBox.exec();
        delete docxml;
        return false;
    }
    root = docxml->RootElement();
    printf("_______________________________________\n\n");
    printf("     contacted person information      \n\n");
    folderIcon.addPixmap(style()->standardPixmap(QStyle::SP_DirClosedIcon),
                         QIcon::Normal, QIcon::Off);
    folderIcon.addPixmap(style()->standardPixmap(QStyle::SP_DirOpenIcon),
                         QIcon::Normal, QIcon::On);
    bookmarkIcon.addPixmap(style()->standardPixmap(QStyle::SP_FileIcon));
    statusIcon.addPixmap(style()->standardPixmap(QStyle::SP_MessageBoxInformation));
    //s
    char *SN = "name";
    char *ID = "id";
    char *SP = "param";
    char *SStatus = "status";
    char *SB = "block";
    char *SSB = "subblock";
    qDebug()<<"Graph's param:";
    int GraphParamIndex = 1;


    TiXmlNode *GraphName = root->FirstChild(SN);
    graph.Name = QString(GraphName->ToElement()->GetText());
    TiXmlNode *GraphID = root->FirstChild(ID);
    graph.ID = QString(GraphID->ToElement()->GetText());
    QTreeWidgetItem *topgraph = new QTreeWidgetItem;
    topgraph->setText(0,graph.Name);              //节点名称
    topgraph ->setIcon(0, folderIcon);
    TreeWigItms.push_back(topgraph);
    domElementForItem.insert(topgraph,GraphName );


    //-------------------parse param--------------
    for (TiXmlNode *GraphParam = root->FirstChild(SP);GraphParam;GraphParam = GraphParam->NextSibling(SP))
    {
        PARAM p;
        TiXmlNode *param = GraphParam->FirstChild();
        p.key = QString(param->ToElement()->GetText());
        param = GraphParam->IterateChildren(param);
        p.name = QString(param->ToElement()->GetText());
        param = GraphParam->IterateChildren(param);
        p.value = QString(param->ToElement()->GetText());
        param = GraphParam->IterateChildren(param);
        p.type = QString(param->ToElement()->GetText());
        graph.gp.append(p);
        QString output = QString("%1 %2 %3 %4").arg(p.key).arg(p.name).arg(p.value).arg(p.type);
        qDebug()<<"No:"<< GraphParamIndex++ << " " << output;
        //*****************************************************************
        if (type == 0 || type == 1)
        {
            QTreeWidgetItem *graphparam = new QTreeWidgetItem(topgraph);
            graphparam->setFlags(graphparam->flags() | Qt::ItemIsEditable);
            graphparam->setText(0, p.key);              //节点名称
            graphparam->setText(1,  p.name);              //节点名称
            graphparam->setText(2,  p.type);              //节点名称
            graphparam->setText(3,  p.value);              //节点名称
            graphparam ->setIcon(0, bookmarkIcon);
            TreeWigItms.push_back(graphparam);
            domElementForItem.insert(graphparam,GraphParam);
            qDebug()  <<  graphparam  <<  "    " << GraphParam << "****" << param;
        }
    }


    //---status----------------------------------------------
    int GraphStatusIndex = 1;
    for (TiXmlNode *GraphStatus = root->FirstChild(SStatus);GraphStatus;GraphStatus = GraphStatus->NextSibling(SStatus))
    {
        STATUS status;
        TiXmlNode *statusNode = GraphStatus->FirstChild();
        status.key = QString(statusNode->ToElement()->GetText());
        statusNode = GraphStatus->IterateChildren(statusNode);
        status.name = QString(statusNode->ToElement()->GetText());
        statusNode = GraphStatus->IterateChildren(statusNode);
        status.value = QString(statusNode->ToElement()->GetText());
        statusNode = GraphStatus->IterateChildren(statusNode);
        status.type = QString(statusNode->ToElement()->GetText());
        graph.gstatus.append(status);
        QString output = QString("status: %1 %2 %3 %4").arg(status.key).arg(status.name).arg(status.value).arg(status.type);
        qDebug()<<"No:"<< GraphStatusIndex++ << " " << output;
        //*****************************************************************
        if (type == 0 || type == 2)
        {
            QTreeWidgetItem *graphstatus = new QTreeWidgetItem(topgraph);
            graphstatus->setText(0, status.key);              //节点名称
            graphstatus->setText(1,  status.name);              //节点名称
            graphstatus->setText(2,  status.type);              //节点名称
            graphstatus->setText(3,  status.value);              //节点名称
            graphstatus ->setIcon(0, statusIcon);
            TreeWigItms.push_back(graphstatus);
            domElementForItem.insert(graphstatus,GraphStatus);
            qDebug()  <<  graphstatus  <<  "    " << GraphStatus << "****" << statusNode;
        }
    }


    //---block----------------------------------------------
    for (TiXmlNode *GraphBlock = root->FirstChild(SB);GraphBlock;GraphBlock = GraphBlock->NextSibling(SB))
    {
        BLOCK b;
        TiXmlNode *BlockName = GraphBlock->FirstChild(SN);
        b .Name = QString(BlockName->ToElement()->GetText());
        TiXmlNode *BlockID = GraphBlock->FirstChild(ID);
        b.ID = QString(BlockID->ToElement()->GetText());
        static int BlockIndex = 1;
        qDebug()<<"BlockIndex:"<<BlockIndex++;
        //***********************************//
        QTreeWidgetItem *block = new QTreeWidgetItem(topgraph);
        block->setText(0,b.Name);              //节点名称
        block ->setIcon(0, folderIcon);
        TreeWigItms.push_back(block);
        domElementForItem.insert(block,GraphBlock );
        int BlockParamIndex= 1;

        //---param in block----------------------------------------------
        for (TiXmlNode *BlockParam = GraphBlock->FirstChild(SP);BlockParam;BlockParam = BlockParam->NextSibling(SP))
        {
            PARAM p;
            TiXmlNode *param = BlockParam->FirstChild();
            p.key = QString(param->ToElement()->GetText());
            param = BlockParam->IterateChildren(param);
            p.name = QString(param->ToElement()->GetText());
            param = BlockParam->IterateChildren(param);
            p.value = QString(param->ToElement()->GetText());
            param = BlockParam->IterateChildren(param);
            p.type = QString(param->ToElement()->GetText());
            b.bp.append(p);
            QString output = QString("%1 %2 %3 %4").arg(p.key).arg(p.name).arg(p.value).arg(p.type);
            qDebug()<<"BlockParamIndex:"<< BlockParamIndex++ << " " << output;
            //********************************************
            if (type == 0 || type == 1)
            {
                QTreeWidgetItem *block_param = new QTreeWidgetItem(block);
                block_param->setFlags(block_param->flags() | Qt::ItemIsEditable);
                block_param->setText(0, p.key);              //节点名称
                block_param->setText(1, p.name);              //节点名称
                block_param->setText(2,  p.type);              //节点名称
                block_param->setText(3,  p.value);              //节点名称
                block_param ->setIcon(0, bookmarkIcon);
                TreeWigItms.push_back(block_param);
                domElementForItem.insert(block_param,BlockParam );
            }
        }

        //---status in block----------------------------------------------
        int BlockStatusIndex = 1;
        for (TiXmlNode *BlockStatusNode = GraphBlock->FirstChild(SStatus);BlockStatusNode;BlockStatusNode = BlockStatusNode->NextSibling(SStatus))
        {
            STATUS status;
            TiXmlNode *statusNode = BlockStatusNode->FirstChild();
            status.key = QString(statusNode->ToElement()->GetText());
            statusNode = BlockStatusNode->IterateChildren(statusNode);
            status.name = QString(statusNode->ToElement()->GetText());
            statusNode = BlockStatusNode->IterateChildren(statusNode);
            status.value = QString(statusNode->ToElement()->GetText());
            statusNode = BlockStatusNode->IterateChildren(statusNode);
            status.type = QString(statusNode->ToElement()->GetText());
            graph.gstatus.append(status);
            b.bstatus.append(status);
            QString output = QString("status: %1 %2 %3 %4").arg(status.key).arg(status.name).arg(status.value).arg(status.type);
            qDebug()<<"No:"<< BlockStatusIndex++ << " " << output;
            //*****************************************************************
            if (type == 0 || type == 2)
            {
                QTreeWidgetItem *blockstatusTree = new QTreeWidgetItem(block);
                blockstatusTree->setText(0, status.key);              //节点名称
                blockstatusTree->setText(1,  status.name);              //节点名称
                blockstatusTree->setText(2,  status.type);              //节点名称
                blockstatusTree->setText(3,  status.value);              //节点名称
                blockstatusTree ->setIcon(0, statusIcon);
                TreeWigItms.push_back(blockstatusTree);
                domElementForItem.insert(blockstatusTree,BlockStatusNode);
            }
            //    qDebug()  <<  blockstatusTree  <<  "    " << BlockStatusNode << "****" << statusNode;
        }


        //---subblock in block----------------------------------------------
        int SubBlockIndex = 1;
        for (TiXmlNode *SubBlock = GraphBlock->FirstChild(SSB);SubBlock;SubBlock = SubBlock->NextSibling(SSB))
        {
            qDebug()<<"SubBlockIndex:"<<SubBlockIndex++;
            SUBBLOCK subblock;
            TiXmlNode *SubBlockName = SubBlock->FirstChild(SN);
            subblock.Name = QString(SubBlockName->ToElement()->GetText());
            TiXmlNode *SubBlockID = SubBlock->FirstChild(ID);
            subblock.ID = QString(SubBlockID->ToElement()->GetText());

            QTreeWidgetItem *subblock_tree = new QTreeWidgetItem(block);
            subblock_tree->setText(0,subblock.ID);              //节点名称
            subblock_tree ->setIcon(0, folderIcon);
            TreeWigItms.push_back(subblock_tree);
            domElementForItem.insert(subblock_tree,SubBlock );


            //---param in subblock----------------------------------------------
            int SubBlockParamIndex= 1;
            for (TiXmlNode *SubBlockparam = SubBlock->FirstChild(SP);SubBlockparam;SubBlockparam = SubBlockparam->NextSibling(SP))
            {
                PARAM p;
                TiXmlNode *param = SubBlockparam->FirstChild();
                p.key = QString(param->ToElement()->GetText());
                param = SubBlockparam->IterateChildren(param);
                p.name = QString(param->ToElement()->GetText());
                param = SubBlockparam->IterateChildren(param);
                p.value = QString(param->ToElement()->GetText());
                param = SubBlockparam->IterateChildren(param);
                p.type = QString(param->ToElement()->GetText());
                subblock.param.append(p);
                QString output = QString("%1 %2 %3 %4").arg(p.key).arg(p.name).arg(p.value).arg(p.type);
                qDebug()<<"SubBlockParamIndex:"<< SubBlockParamIndex++ << " " << output;
                //********************************************
                if (type ==0 || type == 1)
                {
                    QTreeWidgetItem *subblockParam_tree = new QTreeWidgetItem(subblock_tree);
                    subblockParam_tree->setFlags(subblockParam_tree->flags() | Qt::ItemIsEditable);
                    subblockParam_tree->setText(0, p.key);              //节点名称
                    subblockParam_tree->setText(1, p.name);              //节点名称
                    subblockParam_tree->setText(2,  p.type);              //节点名称
                    subblockParam_tree->setText(3,  p.value);              //节点名称
                    subblockParam_tree ->setIcon(0, bookmarkIcon);
                    TreeWigItms.push_back(subblockParam_tree);
                    domElementForItem.insert(subblockParam_tree,SubBlockparam );
                }
            }


            //---status in subblock----------------------------------------------
            int SubBlockStatusIndex= 1;
            for (TiXmlNode *SubBlockStatusNode = SubBlock->FirstChild(SStatus);SubBlockStatusNode;SubBlockStatusNode = SubBlockStatusNode->NextSibling(SStatus))
            {
                STATUS p;
                TiXmlNode *param = SubBlockStatusNode->FirstChild();
                p.key = QString(param->ToElement()->GetText());
                param = SubBlockStatusNode->IterateChildren(param);
                p.name = QString(param->ToElement()->GetText());
                param = SubBlockStatusNode->IterateChildren(param);
                p.value = QString(param->ToElement()->GetText());
                param = SubBlockStatusNode->IterateChildren(param);
                p.type = QString(param->ToElement()->GetText());
                subblock.status.append(p);
                QString output = QString("%1 %2 %3 %4").arg(p.key).arg(p.name).arg(p.value).arg(p.type);
                qDebug()<<"SubBlockStatusIndex:"<< SubBlockStatusIndex++ << " " << output;
                //********************************************
                if (type == 0 || type == 2)
                {
                    QTreeWidgetItem *subblockStatus_tree = new QTreeWidgetItem(subblock_tree);
                    subblockStatus_tree->setFlags(subblockStatus_tree->flags() | Qt::ItemIsEditable);
                    subblockStatus_tree->setText(0, p.key);              //节点名称
                    subblockStatus_tree->setText(1, p.name);              //节点名称
                    subblockStatus_tree->setText(2,  p.type);              //节点名称
                    subblockStatus_tree->setText(3,  p.value);              //节点名称
                    subblockStatus_tree ->setIcon(0, statusIcon);
                    TreeWigItms.push_back(subblockStatus_tree);
                    domElementForItem.insert(subblockStatus_tree,SubBlockStatusNode );
                }
            }
            b.bs.append(subblock);
            SUBBLOCK cc = b.bs.at(0);
        }

        graph.gb.append(b);
    }
    return true;
}

//根据接收到的字符串进行状态更新
bool treeWidget::UpdateXmlState(const char *filepath, GRAPH &graph, TiXmlElement *&root, TiXmlDocument *&docxml,
                                               QList<QTreeWidgetItem *> &TreeWigItms, QMap<QTreeWidgetItem *, TiXmlNode *> &domElementForItem, int type)
{
    for (int i = TreeWigItms.length() - 1; i  >= 0; i--)
    {
        QTreeWidgetItem *p = TreeWigItms.at(i);
        delete p;
    }
    TreeWigItms.clear();
    domElementForItem.clear();


    docxml = new TiXmlDocument();
    docxml->Parse(filepath);

//    if (!loadOkay) {
//        QString strLog = QString("Could not load test file%1. Error='%2'. Exiting.\n").arg( filepath).arg(docxml->ErrorDesc());
//        QMessageBox msgBox;
//        msgBox.setText(strLog);
//        msgBox.exec();
//        delete docxml;
//        return false;
//    }

    root = docxml->RootElement();
    printf("_______________________________________\n\n");
    printf("     contacted person information      \n\n");
    folderIcon.addPixmap(style()->standardPixmap(QStyle::SP_DirClosedIcon),
                         QIcon::Normal, QIcon::Off);
    folderIcon.addPixmap(style()->standardPixmap(QStyle::SP_DirOpenIcon),
                         QIcon::Normal, QIcon::On);
    bookmarkIcon.addPixmap(style()->standardPixmap(QStyle::SP_FileIcon));
    statusIcon.addPixmap(style()->standardPixmap(QStyle::SP_MessageBoxInformation));
    //s
    char *SN = "name";
    char *ID = "id";
    char *SP = "param";
    char *SStatus = "status";
    char *SB = "block";
    char *SSB = "subblock";
    qDebug()<<"Graph's param:";
    int GraphParamIndex = 1;


    TiXmlNode *GraphName = root->FirstChild(SN);
    graph.Name = QString(GraphName->ToElement()->GetText());
    TiXmlNode *GraphID = root->FirstChild(ID);
    graph.ID = QString(GraphID->ToElement()->GetText());
    QTreeWidgetItem *topgraph = new QTreeWidgetItem;
    topgraph->setText(0,graph.Name);              //节点名称
    topgraph ->setIcon(0, folderIcon);
    TreeWigItms.push_back(topgraph);
    domElementForItem.insert(topgraph,GraphName );


    //-------------------parse param--------------
    for (TiXmlNode *GraphParam = root->FirstChild(SP);GraphParam;GraphParam = GraphParam->NextSibling(SP))
    {
        PARAM p;
        TiXmlNode *param = GraphParam->FirstChild();
        p.key = QString(param->ToElement()->GetText());
        param = GraphParam->IterateChildren(param);
        p.name = QString(param->ToElement()->GetText());
        param = GraphParam->IterateChildren(param);
        p.value = QString(param->ToElement()->GetText());
        param = GraphParam->IterateChildren(param);
        p.type = QString(param->ToElement()->GetText());
        graph.gp.append(p);
        QString output = QString("%1 %2 %3 %4").arg(p.key).arg(p.name).arg(p.value).arg(p.type);
        qDebug()<<"No:"<< GraphParamIndex++ << " " << output;
        //*****************************************************************
        if (type == 0 || type == 1)
        {
            QTreeWidgetItem *graphparam = new QTreeWidgetItem(topgraph);
            graphparam->setFlags(graphparam->flags() | Qt::ItemIsEditable);
            graphparam->setText(0, p.key);              //节点名称
            graphparam->setText(1,  p.name);              //节点名称
            graphparam->setText(2,  p.type);              //节点名称
            graphparam->setText(3,  p.value);              //节点名称
            graphparam ->setIcon(0, bookmarkIcon);
            TreeWigItms.push_back(graphparam);
            domElementForItem.insert(graphparam,GraphParam);
            qDebug()  <<  graphparam  <<  "    " << GraphParam << "****" << param;
        }
    }


    //---status----------------------------------------------
    int GraphStatusIndex = 1;
    for (TiXmlNode *GraphStatus = root->FirstChild(SStatus);GraphStatus;GraphStatus = GraphStatus->NextSibling(SStatus))
    {
        STATUS status;
        TiXmlNode *statusNode = GraphStatus->FirstChild();
        status.key = QString(statusNode->ToElement()->GetText());
        statusNode = GraphStatus->IterateChildren(statusNode);
        status.name = QString(statusNode->ToElement()->GetText());
        statusNode = GraphStatus->IterateChildren(statusNode);
        status.value = QString(statusNode->ToElement()->GetText());
        statusNode = GraphStatus->IterateChildren(statusNode);
        status.type = QString(statusNode->ToElement()->GetText());
        graph.gstatus.append(status);
        QString output = QString("status: %1 %2 %3 %4").arg(status.key).arg(status.name).arg(status.value).arg(status.type);
        qDebug()<<"No:"<< GraphStatusIndex++ << " " << output;
        //*****************************************************************
        if (type == 0 || type == 2)
        {
            QTreeWidgetItem *graphstatus = new QTreeWidgetItem(topgraph);
            graphstatus->setText(0, status.key);              //节点名称
            graphstatus->setText(1,  status.name);              //节点名称
            graphstatus->setText(2,  status.type);              //节点名称
            graphstatus->setText(3,  status.value);              //节点名称
            graphstatus ->setIcon(0, statusIcon);
            TreeWigItms.push_back(graphstatus);
            domElementForItem.insert(graphstatus,GraphStatus);
            qDebug()  <<  graphstatus  <<  "    " << GraphStatus << "****" << statusNode;
        }
    }


    //---block----------------------------------------------
    for (TiXmlNode *GraphBlock = root->FirstChild(SB);GraphBlock;GraphBlock = GraphBlock->NextSibling(SB))
    {
        BLOCK b;
        TiXmlNode *BlockName = GraphBlock->FirstChild(SN);
        b .Name = QString(BlockName->ToElement()->GetText());
        TiXmlNode *BlockID = GraphBlock->FirstChild(ID);
        b.ID = QString(BlockID->ToElement()->GetText());
        static int BlockIndex = 1;
        qDebug()<<"BlockIndex:"<<BlockIndex++;
        //***********************************//
        QTreeWidgetItem *block = new QTreeWidgetItem(topgraph);
        block->setText(0,b.Name);              //节点名称
        block ->setIcon(0, folderIcon);
        TreeWigItms.push_back(block);
        domElementForItem.insert(block,GraphBlock );
        int BlockParamIndex= 1;

        //---param in block----------------------------------------------
        for (TiXmlNode *BlockParam = GraphBlock->FirstChild(SP);BlockParam;BlockParam = BlockParam->NextSibling(SP))
        {
            PARAM p;
            TiXmlNode *param = BlockParam->FirstChild();
            p.key = QString(param->ToElement()->GetText());
            param = BlockParam->IterateChildren(param);
            p.name = QString(param->ToElement()->GetText());
            param = BlockParam->IterateChildren(param);
            p.value = QString(param->ToElement()->GetText());
            param = BlockParam->IterateChildren(param);
            p.type = QString(param->ToElement()->GetText());
            b.bp.append(p);
            QString output = QString("%1 %2 %3 %4").arg(p.key).arg(p.name).arg(p.value).arg(p.type);
            qDebug()<<"BlockParamIndex:"<< BlockParamIndex++ << " " << output;
            //********************************************
            if (type == 0 || type == 1)
            {
                QTreeWidgetItem *block_param = new QTreeWidgetItem(block);
                block_param->setFlags(block_param->flags() | Qt::ItemIsEditable);
                block_param->setText(0, p.key);              //节点名称
                block_param->setText(1, p.name);              //节点名称
                block_param->setText(2,  p.type);              //节点名称
                block_param->setText(3,  p.value);              //节点名称
                block_param ->setIcon(0, bookmarkIcon);
                TreeWigItms.push_back(block_param);
                domElementForItem.insert(block_param,BlockParam );
            }
        }

        //---status in block----------------------------------------------
        int BlockStatusIndex = 1;
        for (TiXmlNode *BlockStatusNode = GraphBlock->FirstChild(SStatus);BlockStatusNode;BlockStatusNode = BlockStatusNode->NextSibling(SStatus))
        {
            STATUS status;
            TiXmlNode *statusNode = BlockStatusNode->FirstChild();
            status.key = QString(statusNode->ToElement()->GetText());
            statusNode = BlockStatusNode->IterateChildren(statusNode);
            status.name = QString(statusNode->ToElement()->GetText());
            statusNode = BlockStatusNode->IterateChildren(statusNode);
            status.value = QString(statusNode->ToElement()->GetText());
            statusNode = BlockStatusNode->IterateChildren(statusNode);
            status.type = QString(statusNode->ToElement()->GetText());
            graph.gstatus.append(status);
            b.bstatus.append(status);
            QString output = QString("status: %1 %2 %3 %4").arg(status.key).arg(status.name).arg(status.value).arg(status.type);
            qDebug()<<"No:"<< BlockStatusIndex++ << " " << output;
            //*****************************************************************
            if (type == 0 || type == 2)
            {
                QTreeWidgetItem *blockstatusTree = new QTreeWidgetItem(block);
                blockstatusTree->setText(0, status.key);              //节点名称
                blockstatusTree->setText(1,  status.name);              //节点名称
                blockstatusTree->setText(2,  status.type);              //节点名称
                blockstatusTree->setText(3,  status.value);              //节点名称
                blockstatusTree ->setIcon(0, statusIcon);
                TreeWigItms.push_back(blockstatusTree);
                domElementForItem.insert(blockstatusTree,BlockStatusNode);
            }
            //    qDebug()  <<  blockstatusTree  <<  "    " << BlockStatusNode << "****" << statusNode;
        }


        //---subblock in block----------------------------------------------
        int SubBlockIndex = 1;
        for (TiXmlNode *SubBlock = GraphBlock->FirstChild(SSB);SubBlock;SubBlock = SubBlock->NextSibling(SSB))
        {
            qDebug()<<"SubBlockIndex:"<<SubBlockIndex++;
            SUBBLOCK subblock;
            TiXmlNode *SubBlockName = SubBlock->FirstChild(SN);
            subblock.Name = QString(SubBlockName->ToElement()->GetText());
            TiXmlNode *SubBlockID = SubBlock->FirstChild(ID);
            subblock.ID = QString(SubBlockID->ToElement()->GetText());

            QTreeWidgetItem *subblock_tree = new QTreeWidgetItem(block);
            subblock_tree->setText(0,subblock.ID);              //节点名称
            subblock_tree ->setIcon(0, folderIcon);
            TreeWigItms.push_back(subblock_tree);
            domElementForItem.insert(subblock_tree,SubBlock );


            //---param in subblock----------------------------------------------
            int SubBlockParamIndex= 1;
            for (TiXmlNode *SubBlockparam = SubBlock->FirstChild(SP);SubBlockparam;SubBlockparam = SubBlockparam->NextSibling(SP))
            {
                PARAM p;
                TiXmlNode *param = SubBlockparam->FirstChild();
                p.key = QString(param->ToElement()->GetText());
                param = SubBlockparam->IterateChildren(param);
                p.name = QString(param->ToElement()->GetText());
                param = SubBlockparam->IterateChildren(param);
                p.value = QString(param->ToElement()->GetText());
                param = SubBlockparam->IterateChildren(param);
                p.type = QString(param->ToElement()->GetText());
                subblock.param.append(p);
                QString output = QString("%1 %2 %3 %4").arg(p.key).arg(p.name).arg(p.value).arg(p.type);
                qDebug()<<"SubBlockParamIndex:"<< SubBlockParamIndex++ << " " << output;
                //********************************************
                if (type ==0 || type == 1)
                {
                    QTreeWidgetItem *subblockParam_tree = new QTreeWidgetItem(subblock_tree);
                    subblockParam_tree->setFlags(subblockParam_tree->flags() | Qt::ItemIsEditable);
                    subblockParam_tree->setText(0, p.key);              //节点名称
                    subblockParam_tree->setText(1, p.name);              //节点名称
                    subblockParam_tree->setText(2,  p.type);              //节点名称
                    subblockParam_tree->setText(3,  p.value);              //节点名称
                    subblockParam_tree ->setIcon(0, bookmarkIcon);
                    TreeWigItms.push_back(subblockParam_tree);
                    domElementForItem.insert(subblockParam_tree,SubBlockparam );
                }
            }


            //---status in subblock----------------------------------------------
            int SubBlockStatusIndex= 1;
            for (TiXmlNode *SubBlockStatusNode = SubBlock->FirstChild(SStatus);SubBlockStatusNode;SubBlockStatusNode = SubBlockStatusNode->NextSibling(SStatus))
            {
                STATUS p;
                TiXmlNode *param = SubBlockStatusNode->FirstChild();
                p.key = QString(param->ToElement()->GetText());
                param = SubBlockStatusNode->IterateChildren(param);
                p.name = QString(param->ToElement()->GetText());
                param = SubBlockStatusNode->IterateChildren(param);
                p.value = QString(param->ToElement()->GetText());
                param = SubBlockStatusNode->IterateChildren(param);
                p.type = QString(param->ToElement()->GetText());
                subblock.status.append(p);
                QString output = QString("%1 %2 %3 %4").arg(p.key).arg(p.name).arg(p.value).arg(p.type);
                qDebug()<<"SubBlockStatusIndex:"<< SubBlockStatusIndex++ << " " << output;
                //********************************************
                if (type == 0 || type == 2)
                {
                    QTreeWidgetItem *subblockStatus_tree = new QTreeWidgetItem(subblock_tree);
                    subblockStatus_tree->setFlags(subblockStatus_tree->flags() | Qt::ItemIsEditable);
                    subblockStatus_tree->setText(0, p.key);              //节点名称
                    subblockStatus_tree->setText(1, p.name);              //节点名称
                    subblockStatus_tree->setText(2,  p.type);              //节点名称
                    subblockStatus_tree->setText(3,  p.value);              //节点名称
                    subblockStatus_tree ->setIcon(0, statusIcon);
                    TreeWigItms.push_back(subblockStatus_tree);
                    domElementForItem.insert(subblockStatus_tree,SubBlockStatusNode );
                }
            }
            b.bs.append(subblock);
//            SUBBLOCK cc = b.bs.at(0);
        }

        graph.gb.append(b);
    }
    return true;
}

//日志
void treeWidget::log(QString &str,int  type  = 0,bool  display = false)
{

    QString strLog;
    if (type == INFO)
        strLog = QTime::currentTime().toString() + " INFO: " + str;
    else if (type == WARNING)
        strLog = QTime::currentTime().toString() + " WARNING: " + str;
    else if (type == ERROR)
        strLog = QTime::currentTime().toString() + " ERROR: " + str;
    else
        strLog = QTime::currentTime().toString() + " OTHER: " + str;
    QMessageBox msgBox;
    msgBox.setText(strLog);
    if (display != 0)
        msgBox.exec();
    ui->listWidget->insertItem(0,strLog);
}

//setting 按钮
void treeWidget::on_pushButton_clicked()
{
//    QString fileName = QFileDialog::getSaveFileName(this, "Save Xml File", QDir::currentPath(), "XML Files (*.xml)");
//    qDebug() << fileName;
    //    qDebug() << fileName;
    //    qDebug() << QDir::currentPath();
    //    qDebug() << fileName.remove(0, QDir::currentPath().size()+1);
    //    if(fileName.isEmpty())
    //        return;

//    QString realFileName = fileName.remove(0, QDir::currentPath().size()+1);

    //    QFile file(fileName);
    //    if(!file.open(QFile::WriteOnly |QFile::Text))
    //    {
    //        QMessageBox::warning(this, "Save Xml File", "Cannot Write File");
    //        return;
    //    }


//    doc3 = new TiXmlDocument;

//    const char * xmlFile = realFileName.toStdString().c_str();
//    decl = new TiXmlDeclaration("1.0", "UTF-8", "");
//    graphElement = new TiXmlElement( "flow_graph" );

//    QTreeWidgetItemIterator it(ui->treeWidget_status->topLevelItem(0));

//    int i = 0;
//    int j =0;
//    while(*it)
//    {
//        if( (*it)->text(0) != "")
//        {

//            blockElement = new TiXmlElement( "block" );
//            blockkeyElement = new TiXmlElement("key");
//            blockkeyElement->LinkEndChild(new TiXmlText( Block_Key_List.at(i).toStdString().c_str()) );
//            blocknameElement = new TiXmlElement("name");
//            blocknameElement->LinkEndChild(new TiXmlText( (*it)->text(0).toStdString().c_str() ));
//            blockaffinityElement = new TiXmlElement("affinity");
//            blockaffinityElement->LinkEndChild(new TiXmlText( Block_Affinity_List.at(i).toStdString().c_str() ));

//            blockElement->LinkEndChild(blockkeyElement);
//            blockElement->LinkEndChild(blocknameElement);
//            blockElement->LinkEndChild(blockaffinityElement);

//            graphElement->LinkEndChild(blockElement);
//            i++;
//            ++it;
//        }

//        if( (*it)->text(1) != "")
//        {

//            subblockElement = new TiXmlElement( "subblock" );
//            subblockkeyElement = new TiXmlElement("key");
//            subblockkeyElement->LinkEndChild(new TiXmlText( Sub_Key_List.at(j).toStdString().c_str() ) );
//            subblocknameElement = new TiXmlElement("name");
//            subblocknameElement->LinkEndChild(new TiXmlText( (*it)->text(1).toStdString().c_str() ));

//            subblockElement->LinkEndChild(subblockkeyElement);
//            subblockElement->LinkEndChild(subblocknameElement);

//            blockElement->LinkEndChild(subblockElement);

//            j++;
//            ++it;
//        }

//        if( (*it)->text(2) != "" && (*it)->text(3) != "" && (*it)->text(4) != "" )
//        {
//            paramElement = new TiXmlElement( "param" );
//            paramkeyElement = new TiXmlElement("key");
//            paramkeyElement->LinkEndChild(new TiXmlText( (*it)->text(2).toStdString().c_str() ) );
//            paramnameElement = new TiXmlElement("name");
//            paramnameElement->LinkEndChild(new TiXmlText( (*it)->text(3).toStdString().c_str() ));
//            paramvalueElement = new TiXmlElement("value");
//            paramvalueElement->LinkEndChild(new TiXmlText( (*it)->text(4).toStdString().c_str() ));
//            paramtypeElement = new TiXmlElement("type");
//            paramtypeElement->LinkEndChild(new TiXmlText( (*it)->text(5).toStdString().c_str() ));
//            paramElement->LinkEndChild(paramkeyElement);
//            paramElement->LinkEndChild(paramnameElement);
//            paramElement->LinkEndChild(paramvalueElement);
//            paramElement->LinkEndChild(paramtypeElement);
//            subblockElement->LinkEndChild(paramElement);
//            ++it;
//        }
//    }
//    doc3->LinkEndChild(decl);
//    doc3->LinkEndChild(graphElement);
//    doc3->SaveFile(xmlFile);

//    if (pServer->ConnectedNum() == 0)
//    {
//        QString strLog = "None is connected.";
//        log(strLog,0,true);
//        return;
//    }

    stFrameHeader FH;
    FH.cmd = CMD_SETPARAM;
    QFile   paramFile(fileName);
    if (!paramFile.open(QIODevice::ReadOnly))
    {
        QString strLog = fileName + "is not found!";
        log(strLog);
        return;
    }
    char name[56];
    QString purFileName = fileName.right(fileName.length() - fileName.lastIndexOf("/")  - 1);
    memset(name, 0, 56);
    QByteArray arb = purFileName.toLatin1();
    memcpy(name, arb.data(), purFileName.length());
    QByteArray dataName;
    dataName.append(name, 56);
    QTextStream in(&paramFile);
    QByteArray filearray = in.readAll().toLatin1();
    FH.DATA_LEN = 56 + filearray.length() + 4;
    QByteArray header((char *)&FH, sizeof(stFrameHeader));
    int nLen = filearray.length();
    QByteArray data = header + dataName + QByteArray((char *)&nLen  ,sizeof(int)) + filearray;
    paramFile.close();
    QFile param("testttt.xml");
    param.open(QIODevice::ReadWrite);
    QTextStream out(&param);
    out << filearray;
    param.close();
    pSocket->write(data.data(), data.length());
}

/*void treeWidget::on_pushButtonConnectServer_clicked()
{

    QString strServerAddress = "127.0.0.1";
    int nServerPort = 8878;
    serverIP =new QHostAddress();
    serverIP->setAddress(strServerAddress);
    QString strInfo;
    strInfo = QString("Try to connect TTCServer(%1:%2)").arg(strServerAddress).arg(nServerPort);
    //cout << strInfo.toStdString().c_str() << endl;
    log(strInfo);
    ui->pushButtonConnectServer->setText("Connecting...");
  while (1)
    {
        tcpSocket->connectToHost(*serverIP,nServerPort);
        if (tcpSocket->waitForConnected(2000))
        {
          //  cout << "Connected TTCServer successuflly." << endl;
            strInfo = "Connected TTCServer successuflly.";
            log(strInfo);
            break;
        }
        else
        {
            QTest::qWait(1000);
             strInfo = "Try again.";
            log(strInfo);
        }
       }
    ui->pushButtonConnectServer->setText("Connect");
}*/

//开始按钮
void treeWidget::on_pushButton_start_clicked()
{
    QString strLog;
    if (pSocket == NULL || pSocket->state() != QAbstractSocket::ConnectedState)
    {
        strLog = "Please connect to server";
        log(strLog);
        QMessageBox msgBox;
        msgBox.setText(strLog);
        msgBox.setIcon(QMessageBox::Critical);
        msgBox.exec();
        return;
    }

    stFrameHeader FH;
    FH.cmd = CMD_START;
    FH.DATA_LEN = 0;
    pSocket->write( (char *)&FH, sizeof(stFrameHeader) );
    strLog = "Send start cmd.";
    log(strLog);

    ui->pushButton_start->setEnabled(false);
    ui->pushButton_stop->setEnabled(true);
}

//停止按钮
void treeWidget::on_pushButton_stop_clicked()
{
    stFrameHeader FH;
    FH.cmd = CMD_STOP;
    FH.DATA_LEN = 0;
    pSocket->write((char *)&FH, sizeof(stFrameHeader));
    QString strLog = "Send stop cmd.";
    log(strLog);
    ui->pushButton_start->setEnabled(true);
    ui->pushButton_stop->setEnabled(false);
}

//OK item Changed func
void treeWidget::on_treeWidget_param_itemChanged(QTreeWidgetItem *item, int column)
{
    qDebug() << "abcc";
    TiXmlNode *ParamItem = domElementForItem_param[item];
    qDebug() << ParamItem;
    TiXmlNode *param = ParamItem->FirstChild();                     //key
    qDebug() << QString(param->ToElement()->GetText());
    param = ParamItem->IterateChildren(param);                      //name
    param = ParamItem->IterateChildren(param);
    TiXmlNode *T = param->FirstChild();
    QString str = item->text(column);
    T->SetValue(str.toStdString().c_str());
    docParam->SaveFile();
}

//保存按钮
void treeWidget::on_pushButton_save_clicked()
{
    QString fileName = QFileDialog::getSaveFileName(this, "Save Xml File", QDir::currentPath(), "XML Files (*.xml)");
    QString realFileName = fileName.remove(0, QDir::currentPath().size()+1);
    const char * xmlFile = realFileName.toStdString().c_str();
    TiXmlDocument *doc3 = new TiXmlDocument;
//    decl = new TiXmlDeclaration("1.0", "UTF-8", "");
//    doc3->LinkEndChild(decl);
    TiXmlElement newroot = *rootParam;
    doc3->LinkEndChild(&newroot);
    if (doc3->SaveFile(xmlFile))
    {
        QMessageBox msgBox;
        msgBox.setText("Save XML file successfully.");
        msgBox.exec();
    }
    else
    {
        QMessageBox msgBox;
        msgBox.setText("Don't save XML file.");
        msgBox.exec();
    }
}

//打开XML按钮
void treeWidget::on_pushButton_openXML_clicked()
{
    fileName = QFileDialog::getOpenFileName(this, tr("Open XML File"),
                                                    QDir::currentPath(),
                                                    tr("XML(*.xml *.XML)"));
    ui->lineEdit_XmlPath->setText(fileName);
    if (fileName.length() == 0)
        return;
   //char* filepath = "..//GraphMonitor//TTC_Param_V1.xml";
    ParseXml(fileName.toStdString().c_str(),
                              graphParam,
                              rootParam ,
                              docParam,
                              TreeWigItmsParam,
                              domElementForItem_param,
                               0);
    ui->treeWidget_param->clear();
    ui->treeWidget_param->insertTopLevelItems(0,TreeWigItmsParam);
    ui->treeWidget_param->expandAll();

}

void treeWidget::update()
{


  //  QString fileName = QFileDialog::getOpenFileName(this, tr("Open XML File"),
  //                                                  QDir::currentPath(),
  //                                                  tr("XML(*.xml *.XML)"));
 //   if (fileName.length() == 0)
    //    return;


    /*char* filepath = "..//Remote//TTC_Param_V1.xml";
    ParseXml(filepath,
                              graphStatus,
                              rootStatus,
                              docStatus,
                              TreeWigItmsStatus,
                              domElementForItem_status,
                               2);
    ui->treeWidget_status->clear();
    ui->treeWidget_status->insertTopLevelItems(0,TreeWigItmsStatus);
    ui->treeWidget_status->expandAll();
*/

   // if (pServer->ConnectedNum() == 0)
   // {
      //  QString strLog = "None is connected.";
     //   log(strLog,0,true);
    //    return;
    //}
    /*stFrameHeader FH;
    FH.cmd = 4;
    FH.DATA_LEN = 0;
    static double fre = 1e3;
   if (fre > 16e3)
       fre = 0;
   else
       fre += 2e3;
   QByteArray senddata((char *)&FH,sizeof(stFrameHeader));

   senddata = senddata + QByteArray((char *)&fre,sizeof(double));
    if (pServer->ConnectedNum() != 0)
        pServer->SendMsg(senddata.data(),senddata.length());
   // QString strLog = "Send kill cmd.";
 //   log(strLog);
*/
}

//OK load file
void treeWidget::on_pushButton_load_clicked()
{
    QString strLog;
    if (pSocket == NULL || pSocket->state() != QAbstractSocket::ConnectedState)
    {
        strLog = "Please connect to server";
        log(strLog);
        QMessageBox msgBox;
        msgBox.setText(strLog);
        msgBox.setIcon(QMessageBox::Critical);
        msgBox.exec();
        return;
    }

    int nTemp = fileName.indexOf(".xml");
    //查找同名的grc文件
    QString grcFileName = fileName;
    grcFileName.replace(nTemp, 4, ".grc");

    qDebug() << grcFileName;
     if (!QFile::exists(grcFileName))
    {
        strLog = grcFileName + "is not existed";
        log(strLog);
        QMessageBox msgBox;
        msgBox.setText(strLog);
        msgBox.setIcon(QMessageBox::Critical);
        msgBox.exec();
        return;
    }

    QByteArray Senddata;
    stFrameHeader FH;
    char name[56];
    memset(name, 0, 56);

    //分离出grc的文件名（除路径以外的文件名）
    QString purGrcFileName = grcFileName.right(grcFileName.length() - grcFileName.lastIndexOf("/")  - 1);
    qDebug() << grcFileName.lastIndexOf("/");
    QByteArray arb = purGrcFileName.toLatin1();
    //将grc文件名存入char name[56]中
    memcpy(name, arb.data(), purGrcFileName.length());
    QByteArray dataName;
    dataName.append(name,56);
    QFile   GrcFile(grcFileName);
    if (!GrcFile.open(QIODevice::ReadOnly))
    {
        strLog = grcFileName + "is not found!";
        log(strLog);
        QMessageBox msgBox;
        msgBox.setText(strLog);
        msgBox.setIcon(QMessageBox::Critical);
        msgBox.exec();
        return;
    }
    QTextStream in(&GrcFile);
    QByteArray grcfilearray = in.readAll().toLatin1();
    qint32 nLen = grcfilearray.length();
    Senddata = dataName +  QByteArray((char *)&nLen  ,sizeof(qint32)) + grcfilearray;

    //read XML file
    memset(name,0,56);
    QString purXMLFileName = fileName.right(fileName.length() - fileName.lastIndexOf("/")  - 1);
    arb = purXMLFileName.toLatin1();
    memcpy(name,arb.data(), purXMLFileName.length());
    dataName.clear();
    dataName.append(name,56);
    QFile   XMLFile(fileName);
    if (!XMLFile.open(QIODevice::ReadOnly))
    {
        strLog = fileName + "is not found!";
        log(strLog);
        QMessageBox msgBox;
        msgBox.setText(strLog);
        msgBox.setIcon(QMessageBox::Critical);
        msgBox.exec();
        return;
    }
    QTextStream inxml(&XMLFile);
    QByteArray grcfilearrayxml = inxml.readAll().toLatin1();

    nLen = grcfilearrayxml.length();
    Senddata +=  dataName +  QByteArray((char *)&nLen,sizeof(qint32)) + grcfilearrayxml;

    FH.DATA_LEN  =  Senddata.length();

    FH.cmd = CMD_LOAD;
    QByteArray header((char *)&FH,sizeof(stFrameHeader));
     Senddata = header + Senddata;
     qDebug() << Senddata.size()  << FH.DATA_LEN;
     if( pSocket->write(Senddata) == Senddata.length())
     {
         QMessageBox msgBox;
         msgBox.setText("Load File Successful !");
         msgBox.setIcon(QMessageBox::Information);
         msgBox.exec();
     }

/*

            56 + filearray.length();
    QByteArray header((char *)&FH,sizeof(stFrameHeader));
    QByteArray data = header + dataName + filearray;
    paramFile.close();
    QFile param("testttt.xml");
    param.open(QIODevice::ReadWrite);
    QTextStream out(&param);
    out << filearray;
    param.close();
    pServer->SendMsg(data.data(),data.length());




   //char* filepath = "..//GraphMonitor//TTC_Param_V1.xml";
    ParseXml(fileName.toStdString().c_str(),
                              graphParam,
                              rootParam ,
                              docParam,
                              TreeWigItmsParam,
                              domElementForItem_param,
                               0);
    ui->treeWidget_param->clear();
    ui->treeWidget_param->insertTopLevelItems(0,TreeWigItmsParam);
    ui->treeWidget_param->expandAll();
    stFrameHeader FH;
    */

}

//OK conect button
void treeWidget::on_pushButton_Connect_clicked()
{
    QString strLog;
    if (pSocket != NULL)
    {
        strLog = "Monitor has already connected to server.";
        log(strLog);
        return;
    }

    QString strServerAddress = ui->lineEdit_IpAddress->text(); //QString::fromStdString(string(serverIpAdress));       //("%1").arg(s = serverIpAdress;
    int nServerPort = ui->lineEdit_Port->text().toInt();
    QHostAddress *serverIP =new QHostAddress();
    serverIP->setAddress(strServerAddress);

    pSocket = new QTcpSocket();
    connect(pSocket, SIGNAL(disconnected()), pSocket, SLOT(deleteLater()));
    connect(pSocket, SIGNAL(readyRead()), this, SLOT(dataReceived()));
    QString strInfo;
    strInfo = QString("Try to connect TTCServer(%1:%2)").arg(strServerAddress).arg(nServerPort);
    log(strInfo);
    while (1)
    {
        pSocket->connectToHost(*serverIP,nServerPort);
        if (pSocket->waitForConnected(2000)) //连接成功返回ture
        {
            strInfo = "Connected TTCServer successuflly.";
            log(strInfo);
            break;
        }
        else
        {
            QTest::qWait(1000);
            strInfo = "Try again.";
            log( strInfo );
        }
//        connect(pSocket, SIGNAL(disconnected()), this, SLOT(slotDisconnected()));
//        connect(pSocket, SIGNAL(disconnected()), pSocket, SLOT(deleteLater()));
//        connect(pSocket, SIGNAL(readyRead()), this, SLOT(dataReceived()));
    }
}

void treeWidget::slotDisconnected()
{
    //  delete
    //if (pS)
    // cout <<  "disconnected server." << endl;
    QString strLog = "server is not open";
    log(strLog);
}

//OK disconnect button
void treeWidget::on_pushButton_Disconnect_clicked()
{
    QString strLog;
    if (pSocket == NULL)
    {
        strLog = "Client socket is not available.";
        log(strLog);
        return;
    }
//    if (pSocket->state() != QAbstractSocket::ConnectedState)
//    {
//        strLog = "Monitor is not  already connected to server.";
//        log(strLog);
//        return;
//    }
    pSocket->disconnectFromHost();
    delete pSocket;
    pSocket = NULL;
    strLog = "disconnect.";
    log(strLog);
}

//Ip地址被改变时更新ini文件
void treeWidget::on_lineEdit_IpAddress_textChanged(const QString &arg1)
{
    QSettings settings("Option.ini", QSettings::IniFormat); // 当前目录的INI文件
    settings.beginGroup("DevOption");
    settings.setValue("glbLocalAdress", glbLocalAdress);
    settings.setValue("glbLocalPort", glbLocalPort);
    settings.setValue("glbServerAdress", arg1);
    settings.setValue("glbServerPort", ui->lineEdit_Port->text());
    settings.setValue("glbExeName", glbExeName);
    settings.endGroup();
}

//端口号被改变时更新ini文件
void treeWidget::on_lineEdit_Port_textChanged(const QString &arg1)
{
    QSettings settings("Option.ini", QSettings::IniFormat); // 当前目录的INI文件
    settings.beginGroup("DevOption");
    settings.setValue("glbLocalAdress", glbLocalAdress);
    settings.setValue("glbLocalPort", glbLocalPort);
    settings.setValue("glbServerAdress", ui->lineEdit_IpAddress->text());
    settings.setValue("glbServerPort", arg1);
    settings.setValue("glbExeName", glbExeName);
    settings.endGroup();
}

//接收状态
void treeWidget::dataReceived()
{

    while(pSocket->bytesAvailable())
    {
        dataFlow.append(pSocket->readAll());
        char *p = dataFlow.data();
        char *q = p + dataFlow.size();
        qDebug() << "blockSize:"  << dataFlow.size();

        while (true)
        {
            if( (q - p) < sizeof(stFrameHeader) )
                break;

            stFrameHeader header;
            qDebug() << "headSize:"  << sizeof(stFrameHeader);
            memcpy(&header, p, sizeof(stFrameHeader));
            if(header.FLAG[0] != 'M' || header.FLAG[1] != 'M' || header.FLAG[2] != 'X' || header.FLAG[3] != 'H')
            {
                p = q;
                break;
            }

            if( (q - p) < (sizeof(stFrameHeader) + header.DATA_LEN) )
                break;

            UpdateState(header, p + sizeof(stFrameHeader), header.DATA_LEN);

            p += sizeof(stFrameHeader) + header.DATA_LEN;
        }
        dataFlow = dataFlow.right( q - p);
    }

}

//更新状态
void treeWidget::UpdateState(stFrameHeader &head, char *body, int dataLen)
{

    qint16 cmd = head.cmd;
    if( cmd == CMD_GETSTATUS )
    {
//        char FileName[56] ;
//        memcmp(FileName, body, 56);

//        QString strLog;
//        strLog = "Receive CMD_STATUS cmd. FileName is :" + QString::fromLocal8Bit(FileName);
//        log(strLog);

//        int fileLen = *(int*)(body + 56);

        char *pdata = body/* + 56 + sizeof(int)*/;

        QString xmlContent = QString::fromLocal8Bit(pdata);

        UpdateXmlState(xmlContent.toStdString().c_str(),
                        graphStatus,
                        rootStatus,
                        docStatus,
                        TreeWigItmsStatus,
                        domElementForItem_status,
                        2);
        ui->treeWidget_status->clear();
        ui->treeWidget_status->insertTopLevelItems(0,TreeWigItmsStatus);
        ui->treeWidget_status->expandAll();
    }

}
