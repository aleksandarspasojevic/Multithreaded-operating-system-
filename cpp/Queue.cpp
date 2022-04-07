/*
 * Queue.cpp
 *
 *  Created on: Sep 24, 2021
 *      Author: OS1
 */



#include "Queue.h"


Queue::Queue(){
	lock;

	this->head = this->tail = this->curr = 0;
	this->sz = 0;

	unlock;
}


Queue::~Queue(){
	lock;

	while(head){                //nismo pozivali destruktor pcb-ijeva
		Elem* old = head;
		head = head->next;
		delete old;
	}
	this->sz=0;
	this->curr = this->tail = 0;

	unlock;
}

void Queue::put(KernelSem* kSem, PCB* pcb, Time maxTimeToWait){
	lock;

	this->sz++;
	Elem* newElem = new Elem(kSem, pcb, maxTimeToWait);

	if(head == 0){
		tail = head = newElem;
	}else{
		tail->next = newElem;
		tail = tail->next;
	}

	unlock;
}


Queue::Elem* Queue::get(){     //izlancavanje od head-a
	lock;

	Elem* t = head;

	if(head == 0){
		unlock;
		return 0;
	}else{           //head postoji
		this->sz--;
		if(head == tail){
			tail = head = 0;
		}else{
			head = head->next;
		}
	}

	unlock;
	return t;

}


void Queue::remove(PCB* pcb){
	lock;

	Elem* it = 0;
	Elem* prev = 0;

	for(it = this->head; it!=0; prev = it, it=it->next){
		if(it->pcb == pcb){
			this->sz--;

			if(this->curr == it) this->curr = this->curr->next;

			if(prev == 0){           //prvi element se brise
				head = it->next;
				if(head == 0) tail = 0;

			}else{
				prev->next = it->next;
				if(it->next == 0) tail = prev;

			}
			delete it;

		}

	}

	unlock;
}




int Queue::size()const { return this->sz; }

void Queue::resetCursor(){
	lock;
	this->curr = this->head;
	unlock;
}


int Queue::hasNext(){
	return this->curr != 0;
}

Queue::Elem* Queue::seekCurrElem(){
	lock;
	Queue::Elem* ret = 0;
	if(curr != 0) ret = curr;
	curr = curr->next;
	unlock;
	return ret;
}








