#include "posix_queue.h"

CPosixQueue::~CPosixQueue()
{
    //if message queue was created then close it
    if(m_bIsMsgQOpened)
    {
        mq_close(m_tMsgQDesc);
        mq_unlink(m_strQueueName.c_str());
    }
}

bool CPosixQueue::openQueue(std::string p_strQName,
                            unsigned int p_uiMaxMsgSize,
                            unsigned int p_uiMaxNoOfMsgsInQueue)
{
    //check if queue is already opened
    if(m_bIsMsgQOpened)
        return m_bIsMsgQOpened;

    //add backslash to the name
    m_strQueueName = "/" + p_strQName;
      
    //initialise queue attributes
    m_tMsgQAttr.mq_flags = O_NONBLOCK; //this flag is ignored for mq_open
    m_tMsgQAttr.mq_maxmsg = p_uiMaxNoOfMsgsInQueue;
    m_tMsgQAttr.mq_msgsize = p_uiMaxMsgSize;
    m_tMsgQAttr.mq_curmsgs = 0;

    //open the queue
    m_tMsgQDesc = mq_open(m_strQueueName.c_str(),
                          O_CREAT | O_RDWR,
                          0660,
                          &m_tMsgQAttr);
    if((mqd_t)-1 != m_tMsgQDesc)
        m_bIsMsgQOpened = true;

    return m_bIsMsgQOpened;
}

bool CPosixQueue::closeQueue()
{
    //check if queue is closed
    if(!m_bIsMsgQOpened)
        return false;

    mq_close(m_tMsgQDesc);  
    return true;
}

bool CPosixQueue::deleteQueue()
{
    mq_unlink(m_strQueueName.c_str());
    return true;
}

bool CPosixQueue::readFromQueue(char* p_chDataToRead)
{
    if(!m_bIsMsgQOpened || NULL == p_chDataToRead)
        return false;
    
    if (-1 != mq_receive(m_tMsgQDesc,
                         p_chDataToRead,
                         m_tMsgQAttr.mq_msgsize,
                         NULL))    
        return true;
    else
        return false;   
}

bool CPosixQueue::writeToQueue(char* p_chDataToSend,
                               unsigned int p_uiSizeOfData)
{

    //queue is not opened or parameters are not valid
    if(!m_bIsMsgQOpened || NULL == p_chDataToSend || 0 == p_uiSizeOfData)
        return false;
 
    if(-1 != mq_send(m_tMsgQDesc,
                     p_chDataToSend,
                     p_uiSizeOfData,
                     0))
        return true;
    else
        return false;
}
