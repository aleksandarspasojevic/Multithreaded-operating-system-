/*
 * event.h
 *
 *  Created on: Sep 25, 2021
 *      Author: OS1
 */


 // File: event.h
#ifndef _event_h_
#define _event_h_

#include "PCB.h"

 typedef unsigned char IVTNo;

 class KernelEv;

 class Event {
 public:
	 Event (IVTNo ivtNo);
	 ~Event ();

	 void wait  ();

 protected:
	 friend class KernelEv;
     void signal();           // can call KernelEv

 private:
    KernelEv* myImpl;

 };


 #endif
