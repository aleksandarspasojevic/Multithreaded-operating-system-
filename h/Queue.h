/*
 * Queue.h
 *
 *  Created on: Sep 24, 2021
 *      Author: OS1
 */

#ifndef H_QUEUE_H_
#define H_QUEUE_H_

#include "PCB.h"

typedef unsigned int Time;


class KernelSem;

class Queue{
public:
	Queue();

	~Queue();

	typedef struct Elem{
		PCB* pcb;
		Time maxTimeToWait;
		Elem* next;
		KernelSem* kernelSem;

		Elem(KernelSem* kSem, PCB* pcb, Time maxTimeToWait){
			this->kernelSem = kSem;
			this->pcb = pcb;
			this->maxTimeToWait = maxTimeToWait;
			this->next = 0;
		}
	}Elem;

	void put(KernelSem* kSem, PCB* pcb, Time maxTimeToWait);

	Elem* get();

	void remove(PCB* pcb);

	void resetCursor();      //funkcije za iteraciju kroz elemente reda

	int hasNext();

	Elem* seekCurrElem();

	int size()const;

private:
	int sz;

	Elem* head, *tail, *curr;

};



#endif /* H_QUEUE_H_ */
