/*
 * Idle.h
 *
 *  Created on: Sep 24, 2021
 *      Author: OS1
 */

#ifndef H_IDLE_H_
#define H_IDLE_H_

#include "thread.h"



class Idle : public Thread{
public:

	Idle();

	virtual void run();

};



#endif /* H_IDLE_H_ */
