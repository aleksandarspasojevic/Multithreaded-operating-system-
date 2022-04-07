/*
 * thrMain.h
 *
 *  Created on: Sep 22, 2021
 *      Author: OS1
 */

#ifndef THRMAIN_H_
#define THRMAIN_H_

#include "thread.h"




int userMain(int argc, char* argv[]);

class thrMain : public Thread{
private:
	int retVal;
	StackSize stackSize;
	Time timeSlice;
	int argc;
	char** argv;

public:

	thrMain(unsigned long stackSize = defaultStackSize, unsigned int timeSlice = defaultTimeSlice, int argc, char* argv[]): Thread(stackSize, timeSlice){
		this->stackSize = stackSize;
		this->timeSlice = timeSlice;
		this->argc = argc;
		this->argv = argv;
		this->retVal = 0;
	}

	int getRet(){
		return this->retVal;
	}

	virtual void run(){
		this->retVal = userMain(this->argc, this->argv);
	}


};


#endif /* THRMAIN_H_ */
