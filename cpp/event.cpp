/*
 * event.cpp
 *
 *  Created on: Sep 25, 2021
 *      Author: OS1
 */

#include "event.h"
#include "KernelEv.h"

Event::Event (IVTNo ivtNo){
	lock;
	this->myImpl = new KernelEv(ivtNo);
	unlock;
}



Event::~Event(){
	lock;
	delete this->myImpl;
	unlock;
}


void Event::wait(){
	this->myImpl->wait();
}


void Event::signal(){
	this->myImpl->signal();
}
