/*
 * listPCB.cpp
 *
 *  Created on: Sep 16, 2021
 *      Author: OS1
 */


#include "listPCB.h"


listPCB::listPCB(){

	head = 0;
	id = 0;

}


int listPCB::addPCB(PCB* pcb){
	lock;

	Elem* newElem = new Elem(pcb, id);

	if(!this->head){
		head = newElem;
	}else{
		newElem->next = head;
		head = newElem;
	}

	unlock;

	return this->id++;
}


PCB* listPCB::getPCB(int id){
	lock;

	Elem* curr = 0;

	for(curr = this->head; curr!=0; curr=curr->next){
		if(curr->id == id) break;
	}

	unlock;
	if(curr)
		return curr->data;
	return 0;

}




void listPCB::removePCB(int id){
	lock;

	if(id<0) return;

	Elem* curr = 0;
	Elem* prev = 0;

	for(curr = this->head; curr!=0; prev = curr, curr=curr->next){
		if(curr->id == id){

			if(prev == 0){
				head = curr->next;
				delete curr;
			}else{
				prev->next = curr->next;
				delete curr;
			}

		}

	}

	unlock;

}


listPCB::~listPCB(){
	lock;

	Elem* curr = this->head;

	while(curr){
		Elem* old = curr;
		curr = curr->next;
		delete old;
	}

	unlock;

}





