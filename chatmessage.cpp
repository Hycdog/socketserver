#include"chatmessage.h"

chatMessage::chatMessage(){
    text_message = nullptr;
    _sender = nullptr;
    _receiver = nullptr;

}

chatMessage::chatMessage(QString msg, QTime time_sent,QUuid* sender,QUuid* receiver){
    text_message = msg;
    timestamp = time_sent;
    _sender = sender;
    _receiver = receiver;
}

chatMessage::~chatMessage(){
}

QTime chatMessage::getTimeSent(){
    return timestamp;
}

QString chatMessage::getContent(){

    return text_message;
}

QUuid* chatMessage::getSenderUUID(){
    return _sender;
}

QUuid* chatMessage::getReceiverUUID(){
    return _receiver;
}
