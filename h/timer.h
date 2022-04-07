/*
 * timer.h
 *
 *  Created on: Sep 21, 2021
 *      Author: OS1
 */

#ifndef TIMER_H_
#define TIMER_H_

void interrupt timer();


class Timer{

public:

	Timer();
	~Timer();

	static volatile int counter;
	static volatile int contextOnDemand;

	static void inic();
	static void restore();


};


#endif /* TIMER_H_ */
