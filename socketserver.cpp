#include "socketserver.h"
#include "ui_socketserver.h"
#include<QDebug>

socketserver::socketserver(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::socketserver)
{
    ui->setupUi(this);
    connect(ui->pushButton_startServer,SIGNAL(clicked(bool)),this,SLOT(startTcpserver()));
    init();
    regulateInput();
    connect(ui->tableWidget, SIGNAL(itemClicked(QTableWidgetItem *)),this,SLOT(readMessage(QTableWidgetItem *)));
    connect(ui->pushButton_send, SIGNAL(clicked(bool)), this, SLOT(sendMessage()));
    connect(ui->lineEdit_send, SIGNAL(returnPressed()),this,SLOT(sendMessage()));

}

socketserver::~socketserver()
{
    delete ui;
}

void socketserver::init(){

    this_id = QUuid::createUuid();
    m_tcpServer = nullptr;
    cur_sock = nullptr;
    ui->tabWidget->setTabText(0,"Logs");
    ui->tabWidget->setTabText(1,"Message");
    ui->lineEdit_ip->setText("127.0.0.1");
    ui->lineEdit_port->setText("9000");
    initTable();
}

void socketserver::regulateInput(){
    ui->lineEdit_port->setValidator(new QIntValidator(0,65535,this));
    QRegExp rx("^((2[0-4]\\d|25[0-5]|[01]?\\d\\d?)\\.){3}(2[0-4]\\d|25[0-5]|[01]?\\d\\d?)$");
    ui->lineEdit_ip->setValidator(new QRegExpValidator(rx, this));
}


void socketserver::initTable(){
    QStringList labels;
    labels<<"Ip"<<"Port"<<"Status";
    ui->tableWidget->setColumnCount(3);
    ui->tableWidget->setHorizontalHeaderLabels(labels);
    ui->tableWidget->horizontalHeader()->setStretchLastSection(true);
    ui->tableWidget->verticalHeader()->setVisible(false);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(0,QHeaderView::ResizeToContents);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(1,QHeaderView::ResizeToContents);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(2,QHeaderView::ResizeToContents);
}

void socketserver::updateTable(){

    int rows  = socketdata.size();
    ui->tableWidget->setRowCount(rows);
    QFont font;
    int row=0;
    for (auto i:socketdata) {
        QTableWidgetItem* item0;
        socketData* data = i.second;
        QTcpSocket* sock = data->getSocket();
        if(!data->unRead()){
            item0 = new QTableWidgetItem (sock->peerAddress().toString());
        }
        else{
            item0 = new QTableWidgetItem ("("+QString::number(data->getUnreadNum())+")"+sock->peerAddress().toString());
        }
        QTableWidgetItem* item1 = new QTableWidgetItem (QString::number(sock->peerPort()));
        QTableWidgetItem* item2 = new QTableWidgetItem (QString::number(sock->state()));

        item0->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
        item1->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
        item2->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);


        if(i.second->unRead()){

            font.setBold(true);
            item0->setFont(font);
            item1->setFont(font);
            item2->setFont(font);
        }


        ui->tableWidget->setItem(row,0,item0);
        ui->tableWidget->setItem(row,1,item1);
        ui->tableWidget->setItem(row,2,item2);

        i.second->setRow(row);

        row++;
    }
}

void socketserver::stopTcpserver(){
    ui->pushButton_startServer->setEnabled(false);
    ui->pushButton_startServer->update();
    ui->tabWidget->setCurrentIndex(0);

    ui->textBrowser->append("Waiting for "+QString::number(socketdata.size())+"connection(s) end.");
    for(auto i:socketdata){
        QTcpSocket* to_be_closed = i.first;
        disconnect(to_be_closed,SIGNAL(disconnected()),this,SLOT(removeConnection()));
        to_be_closed->disconnectFromHost();
        delete i.second;
        delete i.first;
    }
    socketdata.clear();
    cur_sock = nullptr;
    updateTable();
    m_tcpServer->close();
    ui->textBrowser->moveCursor(QTextCursor::End);
    ui->textBrowser->append("Server closed.");

    ui->lineEdit_ip->setEnabled(true);
    ui->lineEdit_port->setEnabled(true);
    disconnect(ui->pushButton_startServer,SIGNAL(clicked(bool)),this,SLOT(stopTcpserver()));
    connect(ui->pushButton_startServer,SIGNAL(clicked(bool)),this,SLOT(startTcpserver()));
    ui->pushButton_startServer->setText("Start");
    ui->pushButton_startServer->setEnabled(true);
    ui->pushButton_startServer->update();

}

void socketserver::startTcpserver()
{
    ui->pushButton_startServer->setEnabled(false);
    ui->pushButton_startServer->update();
    QHostAddress addr = QHostAddress(ui->lineEdit_ip->text());
    quint16 port = ui->lineEdit_port->text().toUShort();
    m_tcpServer = new QTcpServer(this);
    ui->tabWidget->setCurrentIndex(0);

   if(m_tcpServer->listen(addr,port)){
       connect(m_tcpServer,SIGNAL(newConnection()),this,SLOT(newConnect()));
       ui->textBrowser->clear();
       ui->textBrowser->moveCursor(QTextCursor::End);
       ui->textBrowser->append("Server started at "+ui->lineEdit_ip->text()+" port "+QString::number(port));
       ui->lineEdit_ip->setEnabled(false);
       ui->lineEdit_port->setEnabled(false);
       disconnect(ui->pushButton_startServer,SIGNAL(clicked(bool)),this,SLOT(startTcpserver()));
       connect(ui->pushButton_startServer,SIGNAL(clicked(bool)),this,SLOT(stopTcpserver()));
       ui->pushButton_startServer->setText("Stop");
       ui->pushButton_startServer->setEnabled(true);
       ui->pushButton_startServer->update();

   }
   else {
       ui->textBrowser->moveCursor(QTextCursor::End);
       ui->textBrowser->append("Server failed to start at "+ui->lineEdit_ip->text()+" port "+QString::number(port));
       ui->pushButton_startServer->setEnabled(true);
       ui->pushButton_startServer->update();

   }


}

void socketserver::removeConnection(){
     QTcpSocket* sockToRemove = qobject_cast<QTcpSocket*>(this->sender());
     socketData* dataToRemove = socketdata[sockToRemove];
     if(cur_sock == sockToRemove) cur_sock = nullptr;
     delete dataToRemove;
     socketdata.erase(sockToRemove);
     updateTable();
}

void socketserver::newConnect()
{
       QTcpSocket* newConn = m_tcpServer->nextPendingConnection();
       socketData* newData = new socketData(newConn);
       socketdata.emplace(std::pair<QTcpSocket*,socketData*>(newConn,newData));
       connect(newConn,SIGNAL(readyRead()),this,SLOT(newMessage())); //有可读的信息，触发读函数槽
       connect(newConn,SIGNAL(disconnected()),this,SLOT(removeConnection()));
       updateTable();

}

void socketserver::newMessage(){
    QTcpSocket* message_from = qobject_cast<QTcpSocket*>(this->sender());
    auto cur_hist = socketdata[message_from];
    if(message_from == cur_sock){
        ui->textBrowser_2->clear();
        QString data = message_from->readAll();
        ui->textBrowser_2->moveCursor(QTextCursor::End);
        cur_hist->appendChatData(data,cur_hist->getUuid(),&this_id);
        ui->textBrowser_2->insertHtml(cur_hist->getData(&this_id, "对方"));
        ui->textBrowser_2->moveCursor(QTextCursor::End);

    }
    else {
        QString data = message_from->readAll();
        cur_hist->setUnread();
        cur_hist->appendChatData(data,cur_hist->getUuid(),&this_id);
        updateTable();
    }

}

void socketserver::readMessage(QTableWidgetItem *item) //读取信息
{
    ui->textBrowser_2->clear();
    for(auto i:socketdata){
        if(i.second->getRow() == item->row()){
            cur_sock = i.first;
            i.second->unsetUnread();
            ui->label_chatting_with->setText("Chatting with "+cur_sock->peerAddress().toString());
            break;
        }
    }
    if(cur_sock!= nullptr){
        ui->textBrowser_2->clear();
        ui->textBrowser_2->moveCursor(QTextCursor::End);
        ui->textBrowser_2->insertHtml(socketdata[cur_sock]->getData(&this_id, "对方"));
        ui->textBrowser_2->moveCursor(QTextCursor::End);

    }
    else {
        qDebug()<<"null cur_sock error";
    }
    updateTable();
    ui->tabWidget->setCurrentIndex(1);

}

void socketserver::sendMessage() //发送信息
{
   QString strMesg= ui->lineEdit_send->text();
   if(cur_sock != nullptr){
       if(strMesg.length()){
           auto cur_hist = socketdata[cur_sock];
           cur_sock->write(strMesg.toStdString().c_str(),strlen(strMesg.toStdString().c_str()));
           cur_hist->appendChatData(strMesg,&this_id,cur_hist->getUuid());
           ui->textBrowser_2->clear();
           ui->textBrowser_2->moveCursor(QTextCursor::End);
           ui->textBrowser_2->insertHtml(cur_hist->getData(&this_id, "对方"));
           ui->textBrowser_2->moveCursor(QTextCursor::End);

       }
   }
   ui->lineEdit_send->clear();
}
