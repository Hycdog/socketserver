
#include"chathistory.h"
chatHistory::chatHistory(){
    this_msg = nullptr;
    next_msg = nullptr;
}

chatHistory::~chatHistory(){
   if(next_msg)
       delete next_msg;
}
chatHistory::chatHistory(chatMessage* message){
    this_msg = message;
    next_msg = nullptr;
}

chatHistory* chatHistory::getnext(){
    return next_msg;
}

chatHistory* chatHistory::appendMessage(chatMessage* next){
    chatHistory* p=this;
    if(p->this_msg == nullptr){
        p->this_msg = next;
    }
    else {
        while(p->getnext() != nullptr){
            p = p->getnext();
        }
        p->setMessageNext(next);
    }
    return p;
}

void chatHistory::setMessage(chatMessage* mesg){
    if(this_msg == nullptr){
        this_msg = mesg;
    }
    else {
        delete this_msg;
        this_msg = mesg;
    }
}

bool chatHistory::setMessageNext(chatMessage *mesg){
    next_msg = new chatHistory(mesg);
    return true;
}

QString chatHistory::getMessage(QUuid* me,QString name_of_the_other){
    chatHistory* head = this;
    QString result = "";
    while(head){
        chatMessage* message_body = head->getThisMessage();
        if(message_body == nullptr) break;
        if(message_body->getSenderUUID() == me){
            result.append("<font color=\"#AAAAFF\">");
            result.append("me at ");
            result.append(message_body->getTimeSent().toString());
            result.append(":<br>");
            result.append(message_body->getContent());
            result.append("<br></font>");
        }
        else if(message_body->getReceiverUUID() == me){
            result.append("<font color=\"#00DD00\">");
            result.append(name_of_the_other+" at ");
            result.append(message_body->getTimeSent().toString());
            result.append(":<br>");
            result.append(message_body->getContent());
            result.append("<br></font>");
        }
        head = head->getnext();
    }
    return result;
}

chatMessage* chatHistory::getThisMessage(){
    return this_msg;
}
//void chatHistory::bindUuid(QUuid* identity, QString* name){
//    uuidList.emplace(std::pair<QUuid*,QString*>(identity,name));
//}
