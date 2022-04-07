/*
 * PCB.h
 *
 *  Created on: Sep 16, 2021
 *      Author: OS1
 */

#ifndef PCB_H_
#define PCB_H_

#include "thread.h"
#include "Idle.h"


#define lock;  \
	asm pushf; \
	asm cli;

#define unlock; asm popf;

enum State {
	STARTING, READY, RUNNING, BLOCKED, DONE
};

class PCB{

public:

	PCB(Thread* th, StackSize stackSize = defaultStackSize, Time timeSlice = defaultTimeSlice);
	~PCB();

	unsigned sp;
	unsigned ss;
	unsigned bp;
	Thread* thread;
	unsigned* stack;
	StackSize stackSize;
	Time timeSlice;
	volatile State state;
	ID id;
	friend class Thread;


	static volatile PCB* running;
	static volatile Idle* idleThread;

	static PCB* getPCBbyID(int id);

	static void wrapper();

	static void setIdle();

	void exitPCB();

	void initStack();


	typedef struct Elem{
		PCB* data;
		struct Elem* next;

		Elem(PCB* pcb, Elem* nxt = 0){
			data = pcb;
			next = nxt;
		}

	}Elem;

	Elem* headBlockedList;

};




#endif /* PCB_H_ */
