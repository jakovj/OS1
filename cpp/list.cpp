#include "list.h"
#include "pcb.h"
#include "ksem.h"
#include "SCHEDULE.h"
#include <iostream.h>
#include "lock.h"

List::List() : first(0), last(0) { }
List::Element::Element(PCB * i) {
	next = 0;
	info = i;
}
void List::add(PCB * el) {
	Element * newElement = new Element(el);
	if (first == 0)
		first = newElement;
	else
		last->next = newElement;
	last = newElement;
}
void List::remove(PCB * el) {
	Element * temp = first, * prev = 0;
	if (temp == 0) return;
	while (temp->info != el) {
		prev = temp;
		temp = temp->next;
		if (temp == 0)
			return;
	}
	if (prev == 0){
		first = first->next;
		if (temp != 0)
			delete temp;
	}
	else {
		if (temp != 0) {
			if (temp == last)
				last = prev;
			prev->next = temp->next;
			delete temp;
		}
	}
}
PCB * List::findById(ID id) {
	Element * temp = first;

	while (temp != 0) {
		if (temp->info->getId() == id){
			return temp->info;
		}
		temp = temp->next;
	}
	return 0;
}
List::~List() {
	while (first != 0) {
		Element * old = first;
		first = first->next;
		old->info = 0;
		delete old;
	}
}

PCB * List::removeFirst() {
	if (first != 0){
		Element * old = first;
		PCB * ret= first->info;
		first = first->next;
		if (first == 0)
			last = 0;

		old->info = 0;
		delete old;
		return ret;
	}
	return 0;
}

void List::addByDiff(PCB * pcb, Time maxTimeToWait) {
	Element * temp = first, * prev = 0;
	pcb->maxTimeToWait = maxTimeToWait;
	Element * newEl = new Element(pcb);
	
	if (temp == 0) {
		first = newEl;
		last = first;
		return;
	}
	
	if (pcb->maxTimeToWait == 0xFFFF) {
		last->next = newEl;
		last = newEl;
		return;
	}
	
	if (pcb->maxTimeToWait < first->info->maxTimeToWait) {
		newEl->next = first;
		first->info->maxTimeToWait -= newEl->info->maxTimeToWait;
		first = newEl;
		return;
	}
	
	
	while (temp != 0) {
		prev = temp;
		if (pcb->maxTimeToWait > temp->info->maxTimeToWait) {
			pcb->maxTimeToWait -= temp->info->maxTimeToWait;
			temp = temp->next;
		}
		else {
			temp = temp->next;
			break;
		}
	}
	
	if (temp == 0) { // dodaje se skroz na kraj
		last->next = newEl;
		last = newEl;
		return;
	}
	
	// dodaje se negde u sredinu
	
	prev->next = newEl;
	newEl->next = temp;
	
	temp->info->maxTimeToWait -= newEl->info->maxTimeToWait;


}
void List::updateTimeOnSemaphore(KernelSem * sem) {
	Element * temp = first;

	if (temp->info != 0 && temp->info->maxTimeToWait != 0xFFFF && temp->info->maxTimeToWait != 0) {
		temp->info->maxTimeToWait--;
	}
	while (temp->info != 0 && temp->info->maxTimeToWait == 0) {

		temp->info->notDeblockedBySignal();
		temp = temp->next;

		sem->signal();

		if (temp == 0)
			return;
	}
}
