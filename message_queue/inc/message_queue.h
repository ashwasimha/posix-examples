#ifndef MESSAGE_QUEUE_H
#define MESSAGE_QUEUE_H

#include <string>

class IMessageQueue
{
public:
    virtual bool openQueue(std::string,
                           unsigned int,
                           unsigned int) = 0;
    virtual bool closeQueue() = 0;
    virtual bool deleteQueue() = 0;
    virtual bool readFromQueue(char*) = 0;
    virtual bool writeToQueue(char*,
                              unsigned int) = 0;                              

};

#endif // MESSAGE_QUEUE_H

