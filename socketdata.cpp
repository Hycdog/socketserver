#include "socketdata.h"

socketData::socketData(QTcpSocket* sock){
    unread = false;
    row = -1;
    uuid = QUuid::createUuid();
    this_socket = sock;
    socket_chat_history = new chatHistory();
    unread_num = 0;
}


socketData::~socketData(){
    delete socket_chat_history;
}

QTcpSocket* socketData::getSocket(){
    return this_socket;
}

bool socketData::unRead(){
    return unread;
}

void socketData::setUnread(){
    unread = true;
}

void socketData::unsetUnread(){
    unread = false;
    unread_num = 0;
}

void socketData::setRow(int row){
    this->row = row;
}

QUuid* socketData::getUuid(){
    return &uuid;
}
int socketData::getRow(){
    return row;
}

void socketData::appendChatData(QString newmsg,QUuid* sender,QUuid* receiver){
    chatMessage* msg = new chatMessage(newmsg,QTime::currentTime(),sender, receiver);
    socket_chat_history->appendMessage(msg);
    if(unread && (*sender) == uuid){
        unread_num ++;
    }
}

QString socketData::getData(QUuid* me,QString name_of_the_other){
    return socket_chat_history->getMessage(me,name_of_the_other);

}

int socketData::getUnreadNum(){
    return unread_num;
}
