#include "includes.h" 

FIFOQUEUE FifoQueue;

void QueueInit(FIFOQUEUE *Queue)
{
    Queue->front = Queue->rear;//????????????
    Queue->count = 0;   //?????0
}
// Queue In
u8 QueueIn(FIFOQUEUE *Queue,ElemType sdat) //??????
{
    if((Queue->front == Queue->rear) && (Queue->count == QueueSize))
    {                    // full //????????
        return QueueFull;    //????????
    }else
    {                    // in
        Queue->dat[Queue->rear] = sdat;
        Queue->rear = (Queue->rear + 1) % QueueSize;
        Queue->count = Queue->count + 1;
        return QueueOperateOk;
    }
}
// Queue Out
u8 QueueOut(FIFOQUEUE *Queue,ElemType *sdat)
{
    if((Queue->front == Queue->rear) && (Queue->count == 0))
    {                    // empty
        return QueueEmpty;
    }else
    {                    // out
        *sdat = Queue->dat[Queue->front];
        Queue->front = (Queue->front + 1) % QueueSize;
        Queue->count = Queue->count - 1;
        return QueueOperateOk;
    }
}

