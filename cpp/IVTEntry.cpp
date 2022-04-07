/*
 * IVTEntry.cpp
 *
 *  Created on: Sep 26, 2021
 *      Author: OS1
 */



#include "IVTEntry.h"
#include <dos.h>


IVTEntry* IVTEntry::IVTEntries[256] = {0};


IVTEntry::IVTEntry(IVTNo ivtNo, pInterrupt newR){
	lock;

	this->ivtNo = ivtNo;
	IVTEntry::IVTEntries[ivtNo] = this;
	this->kernelEvent = 0;

#ifndef BCC_BLOCK_IGNORE
	this->oldRoutine = getvect(ivtNo);
	setvect(this->ivtNo, newR);
#endif

	unlock;
}


IVTEntry::~IVTEntry(){
	lock;

	IVTEntry::IVTEntries[ivtNo] = 0;

#ifndef BCC_BLOCK_IGNORE
	setvect(this->ivtNo, this->oldRoutine);
#endif

	unlock;
}


IVTEntry* IVTEntry::getIVTEntry(IVTNo ivtNo){
	return IVTEntry::IVTEntries[ivtNo];
}


void IVTEntry::setKernelEvent(KernelEv* kernelEv){
	lock;
	this->kernelEvent = kernelEv;
	unlock;
}

KernelEv* IVTEntry::getKernelEvent(){
	return this->kernelEvent;
}

void IVTEntry::callOldRoutine(){
	lock;
	if(this->oldRoutine != 0) this->oldRoutine();
	unlock;
}





