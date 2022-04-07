/*
 * semaphor.cpp
 *
 *  Created on: Sep 22, 2021
 *      Author: OS1
 */


#include "semaphor.h"
#include "KSem.h"

Semaphore::Semaphore(int init){
	lock;
	myImpl = new KernelSem(init);
	unlock;
}


Semaphore::~Semaphore(){
	lock;
	delete this->myImpl;
	unlock;
}


void Semaphore::signal(){
	myImpl->signal();
}


int Semaphore::wait(Time maxTimeToWait){
	return myImpl->wait(maxTimeToWait);      //ne okruzivati sa lock i unlock
}

int Semaphore::val()const { return myImpl->value(); }
