/*
 * KernelEv.cpp
 *
 *  Created on: Sep 25, 2021
 *      Author: OS1
 */


#include "KernelEv.h"
#include "SCHEDULE.h"
#include "IVTEntry.h"

KernelEv::KernelEv(IVTNo ivtNo){
	lock;

	this->ivtNo = ivtNo;
	this->ownerPCB = (PCB*)PCB::running;
	this->val = 0;
	this->flag = -1;

	IVTEntry* ivtEntr = IVTEntry::getIVTEntry(this->ivtNo);
	if(ivtEntr != 0) ivtEntr->setKernelEvent(this);

	unlock;
}

KernelEv::~KernelEv(){
	lock;
	this->ownerPCB = 0;
	unlock;
}


void KernelEv::wait(){     //wait binarnog semafora
	lock;

	if(this->ownerPCB != (PCB* )PCB::running){
		unlock;
		return;    //nit se blokira samo ukoliko je pozivajuca nit vlasnik
	}

	if(this->val == 0){
		this->ownerPCB->state = BLOCKED;
		this->flag = 1;
		unlock;
		dispatch();
	}
	else{
		this->val = 0;
		unlock;
	}

}


void KernelEv::signal(){
	lock;

	if(this->val == 0 ){
		if(this->ownerPCB->state == BLOCKED){
			if(this->flag != 1){
				unlock;
				return;
			}
			this->ownerPCB->state = READY;
			Scheduler::put(this->ownerPCB);
			this->flag = 0;
			unlock;
			dispatch();
		}else{
			this->val = 1;
			unlock;
		}
	}else{
		unlock;
	}


}















