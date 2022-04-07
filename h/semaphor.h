/*
 * semaphor.h
 *
 *  Created on: Sep 22, 2021
 *      Author: OS1
 */

#ifndef SEMAPHOR_H_
#define SEMAPHOR_H_


typedef unsigned int Time;


class KernelSem;

 class Semaphore {

public:
	Semaphore (int init=1);
	virtual ~Semaphore ();

    virtual int wait (Time maxTimeToWait);
    virtual void signal();

    int val () const;  // Returns the current value of the semaphore


 private:
    KernelSem* myImpl;

 };



#endif /* SEMAPHOR_H_ */
