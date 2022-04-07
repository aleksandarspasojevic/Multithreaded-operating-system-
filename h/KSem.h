/*
 * KSem.h
 *
 *  Created on: Sep 24, 2021
 *      Author: OS1
 */

#ifndef H_KSEM_H_
#define H_KSEM_H_

#include "PCB.h"
#include "Queue.h"

typedef unsigned int Time;

class KernelSem{
public:
	KernelSem(int init=1);

	~KernelSem();

	int wait(Time maxTimeToWait);

	void signal();

	int value() const;

	static void updateWaitingThreads();


protected:

	void block(Time maxTimeToWait);

	void unblock();

private:

	int val;
	Queue* blocked;
	static Queue* blockedOnWaiting;
	int unblockingState;          //indikator nacina deblokiranja niti (signal ili istek vremena)
};



#endif /* H_KSEM_H_ */
