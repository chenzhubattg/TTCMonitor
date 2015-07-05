#include "treewidget.h"
#include "ui_treewidget.h"
#include <QTest>
#include <QTime>
#include <QFile>
#include<QTextStream>
#include<QMessageBox>
#include"common.h"
#include"TCPServer.h"
#include <QMessageBox>
#include<QHostAddress>

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


#include<QList>



extern TCPServer *pServer;
treeWidget::treeWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::treeWidget)
{
    ui->setupUi(this);


    connect(ui->treeWidget_param, SIGNAL(itemChanged(QTreeWidgetItem*,int)),
            this, SLOT(updateDomElement(QTreeWidgetItem*,int)));

    //char* filepath = "..//GraphMonitor//TTC_Param.xml";
    //if( readXml(filepath) )
    //   printf("________________Read OK___________________\n");

    char* filepath = "..//GraphMonitor//TTC_Param_V1.xml";
    //    char* filepath = "..//GraphMonitor//TTT.xml";
    //   ParseXml(filepath);




    //  headers << "block_name" <<"subBlock_name" << "key" << "name" << "value" << "type";
    //  ui->treeWidget_2->setHeaderLabels(headers);

    //    //在QTreeWidget上显示树形结构
    //  ui->treeWidget_2->insertTopLevelItems(0,TreeWigItms);
    rootParam = NULL ;
    docParam= NULL;
   rootStatus= NULL ;
   docStatus= NULL;
    //Connect Server
    tcpSocket = new QTcpSocket();

    connect(tcpSocket,SIGNAL(disconnected()),this,SLOT(slotDisconnected()));
    connect(tcpSocket,SIGNAL(readyRead()),this,SLOT(dataReceived()));


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

bool treeWidget::ParseXml(const char* filepath,
                          GRAPH &graph,
                          TiXmlElement* &root ,
                          TiXmlDocument * &docxml,
                          QList<QTreeWidgetItem *> &TreeWigItms,
                          QMap<QTreeWidgetItem *, TiXmlNode *> &domElementForItem,
                          int type = 0)
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
        //---status----------------------------------------------

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
            //status

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
    //*********************************************************************
    /*
    for (int i = 0; i < graph.gp.length();i++)
    {
        PARAM pm = graph.gp.at(i);
        QTreeWidgetItem *graphparam = new QTreeWidgetItem;
        graphparam->setFlags(graphparam->flags() | Qt::ItemIsEditable);
        graphparam->setText(0, pm.key);              //节点名称
        graphparam->setText(1,  pm.name);              //节点名称
        graphparam->setText(2,  pm.type);              //节点名称
        graphparam->setText(3,  pm.value);              //节点名称
        graphparam ->setIcon(0, bookmarkIcon);
        TreeWigItmsParam.push_back(graphparam);
    }
     for (int j = 0; j < graph.gb.length(); j++)
    {
            BLOCK *pblock = &graph.gb[j];
             QTreeWidgetItem *block = new QTreeWidgetItem;
             block->setText(0,pblock->Name);              //节点名称
             block ->setIcon(0, folderIcon);
             TreeWigItmsParam.push_back(block);
             for (int n = 0; n < pblock->bp.length(); n++)
             {
                 PARAM *pblockparam = &pblock->bp[n];
                 QTreeWidgetItem *block_param = new QTreeWidgetItem(block);
                 block_param->setFlags(block_param->flags() | Qt::ItemIsEditable);
                 block_param->setText(0, pblockparam->key);              //节点名称
                 block_param->setText(1, pblockparam->name);              //节点名称
                 block_param->setText(2,  pblockparam->type);              //节点名称
                 block_param->setText(3,  pblockparam->value);              //节点名称
                block_param ->setIcon(0, bookmarkIcon);
             }

                 for(int m = 0; m < pblock->bs.length(); m++)
                 {
                     SUBBLOCK *psubblock = &pblock->bs[m];

                     QTreeWidgetItem *subblock_tree = new QTreeWidgetItem(block);
                     subblock_tree->setText(0,psubblock->ID);              //节点名称
                     subblock_tree ->setIcon(0, folderIcon);

                     for (int kk = 0; kk < psubblock->param.length(); kk++)
                     {
                         PARAM *pparam = &psubblock->param[kk];
                         QTreeWidgetItem *subblockParam_tree = new QTreeWidgetItem(subblock_tree);
                         subblockParam_tree->setFlags(subblockParam_tree->flags() | Qt::ItemIsEditable);
                         subblockParam_tree->setText(0, pparam->key);              //节点名称
                         subblockParam_tree->setText(1,  pparam->name);              //节点名称
                         subblockParam_tree->setText(2,  pparam->type);              //节点名称
                         subblockParam_tree->setText(3,  pparam->value);              //节点名称
                         subblockParam_tree->setIcon(0, bookmarkIcon);
                     }
                 }
             }*/
    return true;
}








/*

    for( TiXmlNode* block = root->FirstChild("block"); block; block = block->NextSibling("block") )
    {

        Block pb;




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

    }
*/



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


void treeWidget::on_pushButton_clicked()
{
    QString fileName = QFileDialog::getSaveFileName(this, "Save Xml File", QDir::currentPath(), "XML Files (*.xml)");
    qDebug() << fileName;
    //    qDebug() << fileName;
    //    qDebug() << QDir::currentPath();
    //    qDebug() << fileName.remove(0, QDir::currentPath().size()+1);
    //    if(fileName.isEmpty())
    //        return;

    QString realFileName = fileName.remove(0, QDir::currentPath().size()+1);

    //    QFile file(fileName);
    //    if(!file.open(QFile::WriteOnly |QFile::Text))
    //    {
    //        QMessageBox::warning(this, "Save Xml File", "Cannot Write File");
    //        return;
    //    }


    doc3 = new TiXmlDocument;

    const char * xmlFile = realFileName.toStdString().c_str();
    decl = new TiXmlDeclaration("1.0", "UTF-8", "");
    graphElement = new TiXmlElement( "flow_graph" );

    QTreeWidgetItemIterator it(ui->treeWidget_status->topLevelItem(0));

    int i = 0;
    int j =0;
    while(*it)
    {
        if( (*it)->text(0) != "")
        {

            blockElement = new TiXmlElement( "block" );
            blockkeyElement = new TiXmlElement("key");
            blockkeyElement->LinkEndChild(new TiXmlText( Block_Key_List.at(i).toStdString().c_str()) );
            blocknameElement = new TiXmlElement("name");
            blocknameElement->LinkEndChild(new TiXmlText( (*it)->text(0).toStdString().c_str() ));
            blockaffinityElement = new TiXmlElement("affinity");
            blockaffinityElement->LinkEndChild(new TiXmlText( Block_Affinity_List.at(i).toStdString().c_str() ));

            blockElement->LinkEndChild(blockkeyElement);
            blockElement->LinkEndChild(blocknameElement);
            blockElement->LinkEndChild(blockaffinityElement);

            graphElement->LinkEndChild(blockElement);
            i++;
            ++it;
        }

        if( (*it)->text(1) != "")
        {

            subblockElement = new TiXmlElement( "subblock" );
            subblockkeyElement = new TiXmlElement("key");
            subblockkeyElement->LinkEndChild(new TiXmlText( Sub_Key_List.at(j).toStdString().c_str() ) );
            subblocknameElement = new TiXmlElement("name");
            subblocknameElement->LinkEndChild(new TiXmlText( (*it)->text(1).toStdString().c_str() ));

            subblockElement->LinkEndChild(subblockkeyElement);
            subblockElement->LinkEndChild(subblocknameElement);

            blockElement->LinkEndChild(subblockElement);

            j++;
            ++it;
        }

        if( (*it)->text(2) != "" && (*it)->text(3) != "" && (*it)->text(4) != "" )
        {
            paramElement = new TiXmlElement( "param" );
            paramkeyElement = new TiXmlElement("key");
            paramkeyElement->LinkEndChild(new TiXmlText( (*it)->text(2).toStdString().c_str() ) );
            paramnameElement = new TiXmlElement("name");
            paramnameElement->LinkEndChild(new TiXmlText( (*it)->text(3).toStdString().c_str() ));
            paramvalueElement = new TiXmlElement("value");
            paramvalueElement->LinkEndChild(new TiXmlText( (*it)->text(4).toStdString().c_str() ));
            paramtypeElement = new TiXmlElement("type");
            paramtypeElement->LinkEndChild(new TiXmlText( (*it)->text(5).toStdString().c_str() ));
            paramElement->LinkEndChild(paramkeyElement);
            paramElement->LinkEndChild(paramnameElement);
            paramElement->LinkEndChild(paramvalueElement);
            paramElement->LinkEndChild(paramtypeElement);
            subblockElement->LinkEndChild(paramElement);
            ++it;
        }
    }
    doc3->LinkEndChild(decl);
    doc3->LinkEndChild(graphElement);
    doc3->SaveFile(xmlFile);

    if (pServer->ConnectedNum() == 0)
    {
        QString strLog = "None is connected.";
        log(strLog,0,true);
        return;
    }

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
    QString purFileName = fileName.right(fileName.length() - fileName.lastIndexOf("//")  - 1);
    memset(name,0,56);
    QByteArray arb = purFileName.toLatin1();
    memcpy(name,arb.data(),purFileName.length());
    QByteArray dataName;
    dataName.append(name,56);
    QTextStream in(&paramFile);
    QByteArray filearray = in.readAll().toLatin1();
    FH.DATA_LEN = 56 + filearray.length();
    QByteArray header((char *)&FH,sizeof(stFrameHeader));
    QByteArray data = header + dataName + filearray;
    paramFile.close();
    QFile param("testttt.xml");
    param.open(QIODevice::ReadWrite);
    QTextStream out(&param);
    out << filearray;
    param.close();
    pServer->SendMsg(data.data(),data.length());
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

void treeWidget::on_pushButton_Listen_clicked()
{
    pServer->StartListen();
}

void treeWidget::on_pushButton_load_clicked()
{
    if (pServer->ConnectedNum() == 0)
    {
        QString strLog = "None is connected.";
        log(strLog,0,true);
        return;
    }
    stFrameHeader FH;
    FH.cmd = CMD_LOAD;
    FH.DATA_LEN = 0;
    pServer->SendMsg((char *)&FH,sizeof(stFrameHeader));
    QString strLog = "Send load cmd.";
    log(strLog);
}

void treeWidget::on_pushButton_start_clicked()
{
    if (pServer->ConnectedNum() == 0)
    {
        QString strLog = "None is connected.";
        log(strLog,0,true);
        return;
    }
    stFrameHeader FH;
    FH.cmd = CMD_START;
    FH.DATA_LEN = 0;
    pServer->SendMsg((char *)&FH,sizeof(stFrameHeader));
    QString strLog = "Send start cmd.";
    log(strLog);
}

void treeWidget::on_pushButton_stop_clicked()
{
    if (pServer->ConnectedNum() == 0)
    {
        QString strLog = "None is connected.";
        log(strLog,0,true);
        return;
    }
    stFrameHeader FH;
    FH.cmd = CMD_STOP;
    FH.DATA_LEN = 0;
    pServer->SendMsg((char *)&FH,sizeof(stFrameHeader));
    QString strLog = "Send stop cmd.";
    log(strLog);
}

void treeWidget::on_pushButton_Kill_clicked()
{
    if (pServer->ConnectedNum() == 0)
    {
        QString strLog = "None is connected.";
        log(strLog,0,true);
        return;
    }
    stFrameHeader FH;
    FH.cmd = CMD_KILL;
    FH.DATA_LEN = 0;
    pServer->SendMsg((char *)&FH,sizeof(stFrameHeader));
    QString strLog = "Send kill cmd.";
    log(strLog);
}

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
    QString str = item->text(3);
    T->SetValue(str.toStdString().c_str());
}

void treeWidget::on_pushButton_save_clicked()
{
    QString fileName = QFileDialog::getSaveFileName(this, "Save Xml File", QDir::currentPath(), "XML Files (*.xml)");
    QString realFileName = fileName.remove(0, QDir::currentPath().size()+1);
    const char * xmlFile = realFileName.toStdString().c_str();
    TiXmlDocument *doc3 = new TiXmlDocument;
    decl = new TiXmlDeclaration("1.0", "UTF-8", "");
    doc3->LinkEndChild(decl);
    TiXmlElement newroot = *rootParam;
    doc3->LinkEndChild(&newroot);
    if (doc3->SaveFile(xmlFile))
    {
        qDebug() << "successfully";
    }
    else
    {
        QMessageBox msgBox;
        msgBox.setText("Don't save XML file.");
        msgBox.exec();
    }
}

void treeWidget::on_pushButton_openXML_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open XML File"),
                                                    QDir::currentPath(),
                                                    tr("XML(*.xml *.XML)"));
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

void treeWidget::on_pushButton_send_clicked()
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


    QString grcFileName = QFileDialog::getOpenFileName(this, tr("Open GRC File"),
                                                    QDir::currentPath(),
                                                    tr("GRC(*.grc *.GRC)"));
    if (grcFileName.length() == 0)
        return;
    int nTemp = grcFileName.indexOf(".grc");
    QString xmlFileName = grcFileName;
    xmlFileName = xmlFileName.replace(nTemp,4,".xml");
    if (!QFile::exists(xmlFileName))
    {
        strLog = xmlFileName + "is not existed";
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
    memset(name,0,56);


    QString purGrcFileName = grcFileName.right(grcFileName.length() - grcFileName.lastIndexOf("/")  - 1);
    qDebug() << grcFileName.lastIndexOf("/");
    QByteArray arb = purGrcFileName.toLatin1();
    memcpy(name,arb.data(),purGrcFileName.length());
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
    int nLen = grcfilearray.length();
    Senddata = dataName +  QByteArray((char *)&nLen   ,sizeof(int)) + grcfilearray;
    //read XML file
    memset(name,0,56);
    QString purXMLFileName = xmlFileName.right(xmlFileName.length() - xmlFileName.lastIndexOf("/")  - 1);
    arb = purXMLFileName.toLatin1();
    memcpy(name,arb.data(),purGrcFileName.length());
    dataName.clear();
    dataName.append(name,56);
    QFile   XMLFile(xmlFileName);
    if (!XMLFile.open(QIODevice::ReadOnly))
    {
        strLog = xmlFileName + "is not found!";
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
Senddata +=  dataName +  QByteArray((char *)&nLen,sizeof(int)) + grcfilearrayxml;




    FH.DATA_LEN  =  Senddata.length();
    FH.cmd = CMD_LOAD;
        QByteArray header((char *)&FH,sizeof(stFrameHeader));
     Senddata = header + Senddata;
     pSocket->write(Senddata);



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

void treeWidget::on_pushButton_Connect_clicked()
{
    QString strLog;
    if (pSocket != NULL)
    {
        strLog = "Monitor has already connected to server.";
        log(strLog);
        return;
    }
    QString strServerAddress = "127.0.0.1"; //QString::fromStdString(string(serverIpAdress));       //("%1").arg(s = serverIpAdress;
    int nServerPort = 7010;
   QHostAddress *serverIP =new QHostAddress();
   serverIP->setAddress(strServerAddress);
   pSocket = new QTcpSocket();
   QString strInfo;
   strInfo = QString("Try to connect TTCServer(%1:%2)").arg(strServerAddress).arg(nServerPort);
   log(strInfo);
   while (1)
   {
       pSocket->connectToHost(*serverIP,nServerPort);
       if (pSocket->waitForConnected(2000))
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
   connect(pSocket,SIGNAL(disconnected()),this,SLOT(slotDisconnected()));
   connect(pSocket,SIGNAL(readyRead()),this,SLOT(dataReceived()));
   }
}

   void treeWidget::slotDisconnected()
   {
     //  delete
       //if (pS)
          // cout <<  "disconnected server." << endl;
   }
void treeWidget::on_pushButton_Disconnect_clicked()
{
    QString strLog;
    if (pSocket == NULL)
    {
        strLog = "Client socket is not available.";
        log(strLog);
        return;
    }
    if (pSocket->state() != QAbstractSocket::ConnectedState)
    {
        strLog = "Monitor is not  already connected to server.";
        log(strLog);
        return;
    }
    pSocket->disconnectFromHost();
    delete pSocket;
    pSocket = NULL;
}
