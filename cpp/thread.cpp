/*
 * thread.cpp
 *
 *  Created on: Sep 16, 2021
 *      Author: OS1
 */


#include "thread.h";
#include "PCB.h"
#include <dos.h>
#include "SCHEDULE.H"
#include "timer.h"



Thread::Thread(StackSize stackSize, Time timeSlice)
{
	lock;

	this->myPCB = new PCB(this, stackSize, timeSlice);

	unlock;
}


Thread::~Thread(){
	lock;

	waitToComplete();
	delete myPCB;

	unlock;

}


void Thread::start(){

	lock;

	if(this->myPCB->state == STARTING){    //samo ako je startujuca niti, proglasiti je spremnom i ubaciti je u SCHEDULER
		this->myPCB->state = READY;
		Scheduler::put(this->myPCB);
	}

	unlock;
}


ID Thread::getRunningId(){
	return PCB::running->id;
}


ID Thread::getId(){
	return this->myPCB->id;
}

Thread* Thread::getThreadById(ID id){
	lock;
	PCB* pcb = PCB::getPCBbyID(id);
	if(pcb!=0)
		unlock;
		return pcb->thread;

	unlock;
	return 0;
}


void Thread::waitToComplete(){

	if(myPCB->state!= STARTING && myPCB->state!= DONE && PCB::running != this)
	{
		lock;
		myPCB->headBlockedList = new PCB::Elem((PCB*)PCB::running,myPCB->headBlockedList);  //ulancavanje PCBa tekuce niti u listu blokiranih
		PCB::running->state = BLOCKED; //nit je blokirana
		unlock;

		dispatch();  //dajemo procesor drugoj niti
	}

}


//extern volatile unsigned contextOnDemand;

void dispatch(){ // sinhrona promena konteksta
	lock;

	Timer::contextOnDemand = 1;
	timer();

	unlock;
}












