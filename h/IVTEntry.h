/*
 * IVTEntry.h
 *
 *  Created on: Sep 26, 2021
 *      Author: OS1
 */

#ifndef H_IVTENTRY_H_
#define H_IVTENTRY_H_

#include "PCB.h"
#include "KernelEv.h"

typedef unsigned char IVTNo;
typedef void interrupt (*pInterrupt)(...);

class KernelEv;


#define PREPAREENTRY(_ivtNo, _callOld)\
	void interrupt intr##_ivtNo(...);\
	IVTEntry ivt##_ivtNo((unsigned char)_ivtNo, (pInterrupt)intr##_ivtNo);\
	void interrupt intr##_ivtNo(...){\
		if(IVTEntry::getIVTEntry((unsigned char)_ivtNo)){\
			if(_callOld) IVTEntry::getIVTEntry((unsigned char)_ivtNo)->callOldRoutine();\
			if(IVTEntry::getIVTEntry((unsigned char)_ivtNo)->getKernelEvent()) IVTEntry::getIVTEntry((unsigned char)_ivtNo)->getKernelEvent()->signal();\
		}\
	}


class IVTEntry{
public:

	IVTEntry(IVTNo ivtNo, pInterrupt newR);

	~IVTEntry();

	static IVTEntry* getIVTEntry(IVTNo ivtNo);

	void setKernelEvent(KernelEv* kernelEv);

	KernelEv* getKernelEvent();

	void callOldRoutine();

private:

	pInterrupt oldRoutine;
	IVTNo ivtNo;
	static IVTEntry* IVTEntries[256];
	KernelEv* kernelEvent;

};



#endif /* H_IVTENTRY_H_ */



