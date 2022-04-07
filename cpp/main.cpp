/*
 * main.cpp
 *
 *  Created on: Sep 22, 2021
 *      Author: OS1
 */


#include "timer.h"
#include "thrMain.h"
#include "PCB.h"



int main(int argc, char* argv[]){

	lock;
	Timer* timer = new Timer();
	PCB::running = new PCB(0);
	PCB::running->state = RUNNING;    //tekuca main nit je running
	unlock;


	thrMain* mainThread = new thrMain(defaultStackSize, defaultTimeSlice, argc, argv);
	mainThread->start();

	mainThread->waitToComplete();

	lock;
	int retValue = mainThread->getRet();
	delete mainThread;
	delete timer;
	delete PCB::running;
	PCB::running = 0;
	unlock;

	return retValue;   //srediti ret iz mainThreada
}
