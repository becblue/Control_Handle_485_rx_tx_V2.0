#ifndef FIFO_H
#define FIFO_H

#include "includes.h"


#define ElemType       u8
#define QueueSize      20 //fifo?????
#define QueueFull      0  //fifo??0
#define QueueEmpty     1  //FIFO??1
#define QueueOperateOk 2  //?????? ???2

typedef struct _FIFOQUEUE
{
    u16 front;     //???
    u16 rear;        //???
    u16 count;       //????
    ElemType dat[QueueSize];
}FIFOQUEUE;

extern FIFOQUEUE FifoQueue;

extern void QueueInit(FIFOQUEUE *Queue);
// Queue In
extern u8 QueueIn(FIFOQUEUE *Queue,ElemType sdat);
// Queue Out
extern u8 QueueOut(FIFOQUEUE *Queue,ElemType *sdat); 

#endif
