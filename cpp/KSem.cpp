/*
 * KSem.cpp
 *
 *  Created on: Sep 24, 2021
 *      Author: OS1
 */

#include "KSem.h"
#include "SCHEDULE.h"


Queue* KernelSem::blockedOnWaiting = new Queue();


KernelSem::KernelSem(int init){
	lock;

	this->val = init;
	this->blocked = new Queue();
	this->unblockingState = 1;

	unlock;
}


KernelSem::~KernelSem(){
	lock;

	blocked->resetCursor();
	while(blocked->hasNext()){                       //sve blokirane niti vracamo u SCHEDULER
		Queue::Elem* currElem = blocked->seekCurrElem();

		Scheduler::put(currElem->pcb);

	}

	delete blocked;

	unlock;
}


void KernelSem::block(Time maxTimeToWait){
	lock;
	//ukoliko je maxWait=0, ovaj metod ubacuje u blocked listu, u suprotnom ubacuje i u globalnu blockedOnWaiting listu

	PCB::running->state = BLOCKED;
	blocked->put(this, (PCB*) PCB::running, maxTimeToWait);   //tekucu nit ubacujemo su red blokiranih i markiramo nit kao BLOCKED

	if(maxTimeToWait > 0) KernelSem::blockedOnWaiting->put(this, (PCB*) PCB::running, maxTimeToWait);

	unlock;
	dispatch();

}

void KernelSem::unblock(){
	lock;

	Queue::Elem* elem = blocked->get();       //nit se vadi iz reda blokiranih, markira kao READY i ubacuje u SCHEDULER
	PCB* pcb = elem->pcb;

	if(elem->maxTimeToWait > 0){
		KernelSem::blockedOnWaiting->remove(pcb);     //datu nit je odblokirao sigal metod --> brisemo iz globalne liste
	}
	this->unblockingState = 1;

	pcb->state = READY;
	Scheduler::put(pcb);

	delete elem;
	unlock;
}


int KernelSem::value()const { return this->val; }

void KernelSem::signal(){
	lock;

	if(this->val < 0 && blocked->size()>0){
		this->unblock();
	}
	this->val++;

	unlock;
}


int KernelSem::wait(Time maxTimeToWait){
	lock;

	if(maxTimeToWait < 0){ //greska
		unlock;
		return -1;
	}

	this->val--;

	if(this->val < 0){
		block(maxTimeToWait);
	}else this->unblockingState = 1;


	unlock;

	return this->unblockingState;
}


void KernelSem::updateWaitingThreads(){
	lock;

	KernelSem::blockedOnWaiting->resetCursor();
	while(KernelSem::blockedOnWaiting->hasNext()){         //iteracija kroz sve niti koje moze odblokirati timer u nekom trenutku
		Queue::Elem* currElem = KernelSem::blockedOnWaiting->seekCurrElem();

		currElem->maxTimeToWait--;

		if(currElem->maxTimeToWait == 0){
			currElem->kernelSem->val++;

			KernelSem* kSem = currElem->kernelSem;
			kSem->blocked->remove(currElem->pcb);          //brisemo odblokiranu nit iz liste kod njenog kernelSemafora
			kSem->unblockingState = 0;

			currElem->pcb->state = READY;
			Scheduler::put(currElem->pcb);                 //vracamo u SCHEDULER niti kojima je isteklo vreme cekanja
			KernelSem::blockedOnWaiting->remove(currElem->pcb);
		}

	}

	unlock;
}















