#include "semlist.h"
#include "ksem.h"
#include "list.h"

SemList::SemList() : first(0), last(0) { }
SemList::Element::Element(KernelSem * i) {
	next = 0;
	info = i;
}
void SemList::add(KernelSem * el) {
	Element * newElement = new Element(el);
	if (first == 0)
		first = newElement;
	else
		last->next = newElement;
	last = newElement;
}
void SemList::remove(KernelSem * el) {
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
		temp->info = 0;
		delete temp;
	}
	else {
		if (temp != 0) {
			if (temp == last)
				last = prev;
			prev->next = temp->next;
			temp->info = 0;
			delete temp;
		}
	}
}

SemList::~SemList() {
	while (first != 0) {
		Element * old = first;
		first = first->next;
		old->info = 0;
		delete old;
	}
}


void SemList::updateTime() {
	Element * tempSem = KernelSem::allSemaphores->first;

	while (tempSem != 0) {
		tempSem->info->getWaitingThreadsList()->updateTimeOnSemaphore(tempSem->info);
		tempSem = tempSem->next;
	}
}

