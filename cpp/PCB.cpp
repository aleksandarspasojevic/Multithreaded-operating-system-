/*
 * PCB.cpp
 *
 *  Created on: Sep 21, 2021
 *      Author: OS1
 */

#include <dos.h>

#include "PCB.h"
#include "ListPCB.h"
#include "SCHEDULE.H"

listPCB* list = new listPCB();
volatile PCB* PCB::running = 0;
volatile Idle* PCB::idleThread = new Idle();


PCB::PCB(Thread* th, StackSize stackSize, Time timeSlice){
	lock;

	this->thread = th;
	this->stackSize = stackSize;
	this->timeSlice = timeSlice;
	this->headBlockedList = 0;

	initStack();
	this->id = list->addPCB(this);   //dobijanje jedinstvenog id-ja za kreirani PCB

	this->state = STARTING;          //markiranje niti kao startujuce

	unlock;
}


PCB::~PCB(){
	lock;

	this->thread = 0;
	if(!this->stack) delete[] stack;
	this->stack = 0;

	list->removePCB(this->id);   //uklanjanje PCBa iz globalne liste PCB-ijeva

	unlock;
}

PCB* PCB::getPCBbyID(int id){
	return list->getPCB(id);
}

void PCB::exitPCB(){
	lock;

	Elem* old = headBlockedList;
	for(Elem* curr = headBlockedList; curr!=0; old=curr, curr=curr->next, delete old){    //odblokiranje niti iz liste i ubacivanje u SCHEDULER
		curr->data->state = READY;         //markirati kao ready pre ubacivanja u SCHEDULER
		Scheduler::put(curr->data);

	}

	PCB::running->state = DONE;    //oznaciti tekucu nit kao zavrsenu
	unlock;

	dispatch();

}


void PCB::initStack(){

	int stackSize = this->stackSize;
	if(stackSize>65535)                   //osigurati maximalnu velicinu steka
		stackSize=65535;

	stackSize/=sizeof(unsigned);          //velicina steka u sizeof(unsigned) jedinicama

	this->stack = new unsigned[stackSize];

	this->stack[stackSize-1] =0x200;      //maskiran I bit
#ifndef BCC_BLOCK_IGNORE
		stack[stackSize-2] = FP_SEG(PCB::wrapper);
		stack[stackSize-3] = FP_OFF(PCB::wrapper);
		sp = FP_OFF(stack+stackSize-12);
		ss = FP_SEG(stack+stackSize-12);
		bp = sp;
		stack[stackSize-12]=FP_OFF(stack+stackSize);

#endif

}


void PCB::setIdle(){
	PCB::running = PCB::idleThread->myPCB;   //PCB ima Thread za prijateljsku funkciju te moze pristpuiti myPCB-iju
}


void PCB::wrapper(){
	((PCB*)PCB::running)->thread->run();
	((PCB*)PCB::running)->exitPCB();            //wrapper fja ne sme da se vrati sa returnom, jer nema aktivacioni blok poziva
	                                            //exitPCB poziva despatch() koji bira sledecu
}

