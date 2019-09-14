#ifndef CHATMESSAGE_H
#define CHATMESSAGE_H
#include<QString>
#include<QTime>
#include<QUuid>
#include<map>

class chatMessage{

public:
    chatMessage();
    chatMessage(QString msg, QTime time_sent,QUuid* sender,QUuid* receiver);
    QTime getTimeSent();
    QString getContent();
    QUuid* getSenderUUID();
    QUuid* getReceiverUUID();
    ~chatMessage();

private:
    QString text_message;
    QTime timestamp;
    QUuid* _sender;
    QUuid* _receiver;
};




#endif // CHATMESSAGE_H
