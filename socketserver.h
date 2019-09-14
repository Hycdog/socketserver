#ifndef SOCKETSERVER_H
#define SOCKETSERVER_H

#include<QtWidgets>
#include <QMainWindow>
#include <QtNetwork/QTcpServer>
#include <QtNetwork/QTcpSocket>
#include <QtNetwork/QHostAddress>
#include<map>
#include<QTimer>
#include<QTableWidget>
#include<QUuid>
#include"socketdata.h"

namespace Ui {
class socketserver;
}

class socketserver : public QMainWindow
{
    Q_OBJECT


private slots:
    void startTcpserver();
    void stopTcpserver();
    void newConnect();
    void readMessage(QTableWidgetItem *item);
    void sendMessage();
    void updateTable();
    void removeConnection();
    void newMessage();

public:
    explicit socketserver(QWidget *parent = nullptr);
    ~socketserver() override;
    void regulateInput();
    void init();
    void initTable();


private:
    Ui::socketserver *ui;
    QTcpServer* m_tcpServer;
    QTcpSocket* cur_sock;
    std::map<QTcpSocket*,socketData*> socketdata;
//    QTimer* update_timer;
    QUuid this_id;
};

#endif // SOCKETSERVER_H
