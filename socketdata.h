#ifndef SOCKETDATA_H
#define SOCKETDATA_H
#include<QString>
//#include"chatmessage.h"
#include<QUuid>
#include<QTcpSocket>
#include"chathistory.h"

class socketData{

public:
    socketData(QTcpSocket* sock);
    ~socketData();
    bool unRead();
    void setUnread();
    void unsetUnread();
    void setRow(int row);
    int getRow();
    int getUnreadNum();
    QTcpSocket* getSocket();
    void appendChatData(QString newmsg,QUuid* sender,QUuid* receiver);
    QUuid* getUuid();
    QString getData(QUuid* me,QString name_of_the_other);

private:
    bool unread;
    int unread_num;
    int row;
    QUuid uuid;
    QTcpSocket* this_socket;
    chatHistory* socket_chat_history;
};

#endif // SOCKETDATA_H
