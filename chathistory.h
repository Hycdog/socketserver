#ifndef CHATHISTORY_H
#define CHATHISTORY_H
#include"chatmessage.h"
#include<QDebug>
class chatHistory{

public:
    chatHistory();
    ~chatHistory();
    chatHistory(chatMessage* message);
    chatHistory* appendMessage(chatMessage* next);
    void setMessage(chatMessage* mesg);
    chatMessage* getThisMessage();
    bool setMessageNext(chatMessage* mesg);
    QString getMessage(QUuid* me,QString name_of_the_other);
//    void bindUuid(QUuid* identity, QString* name);
    chatHistory* getnext();
private:
    chatMessage* this_msg;
    chatHistory* next_msg;
//    std::map<QUuid*,QString*> uuidList;
};

#endif // CHATHISTORY_H
