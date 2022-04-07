/*
 * Idle.cpp
 *
 *  Created on: Sep 24, 2021
 *      Author: OS1
 */



#include "Idle.h"

Idle::Idle(): Thread(defaultStackSize, 1){}    //timeSlice na 1

void Idle::run(){
	while(1);
}

