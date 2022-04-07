/*
 * ListPCB.h
 *
 *  Created on: Sep 16, 2021
 *      Author: OS1
 */

#ifndef LISTPCB_H_
#define LISTPCB_H_

#include "PCB.h"
#include <iostream.h>

class listPCB{

public:

	listPCB();

    int addPCB(PCB* elem);

    PCB* getPCB(int id);

    void removePCB(int id);

    ~listPCB();

private:

    typedef struct Elem{
		PCB* data;
		struct Elem* next;
		int id;           //cuvamo i jedinstveni id niti

		Elem(PCB* pcb, int id1, Elem* nxt = 0){
			data = pcb;
			next = nxt;
			id = id1;
		}

	}Elem;


	Elem* head;
	int id;

};


#endif /* LISTPCB_H_ */
