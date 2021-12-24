#ifndef CPOSIXQUEUE_H
#define CPOSIXQUEUE_H

#include <mqueue.h>
#include "message_queue.h"

class CPosixQueue : public IMessageQueue
{
public:
     CPosixQueue() = default;
     CPosixQueue(const CPosixQueue&) = delete;
     CPosixQueue & operator=(const CPosixQueue&) = delete;
    ~CPosixQueue();

public:
    virtual bool openQueue(std::string,
                           unsigned int,
                           unsigned int) final;
    virtual bool closeQueue() final;
    virtual bool deleteQueue() final;
    virtual bool readFromQueue(char*) final;
    virtual bool writeToQueue(char*,
                              unsigned int) final;

private:
    mqd_t m_tMsgQDesc;
    struct mq_attr m_tMsgQAttr;
    bool m_bIsMsgQOpened = {false};
    std::string m_strQueueName;

};

#endif // CPOSIXQUEUE_H
