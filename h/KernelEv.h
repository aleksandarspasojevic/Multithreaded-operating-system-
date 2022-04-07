/*
 * KernelEv.h
 *
 *  Created on: Sep 25, 2021
 *      Author: OS1
 */

#ifndef H_KERNELEV_H_
#define H_KERNELEV_H_

#include "PCB.h"

typedef unsigned char IVTNo;

class KernelEv{

public:
	KernelEv(IVTNo ivtNo);

	~KernelEv();

	void wait();

	void signal();

private:

	volatile int val;
	PCB* ownerPCB;
	IVTNo ivtNo;
	int flag;

};



#endif /* H_KERNELEV_H_ */
