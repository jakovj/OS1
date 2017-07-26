#include "siglist.h"
#include "pcb.h"
#include "lock.h"

SigList::SigList() : first(0), last(0) { }
SigList::Element::Element(SignalId i) {
	next = 0;
	info = i;
}
void SigList::add(SignalId el) {
	Element * newElement = new Element(el);
	if (first == 0)
		first = newElement;
	else
		last->next = newElement;
	last = newElement;
}

SigList::~SigList() {
	while (first != 0) {
		Element * old = first;
		first = first->next;
		old->info = 0;
		delete old;
	}
}
void SigList::processSignals(PCB * pcb) {
	Element * temp = first, * prev = 0, *old;

	while (temp != 0) {
		if (pcb->blocked[temp->info] == UNBLOCKED && pcb->blockedGlobally[temp->info] == UNBLOCKED && pcb->masked[temp->info] == UNMASKED && pcb->maskedGlobally[temp->info] == UNMASKED) {
			if (pcb->myHandlers[temp->info] != 0) {

				pcb->myHandlers[temp->info]();

				if (temp->info == 0) {
					return;
				}
			}
		}
		if ((pcb->masked[temp->info] == MASKED || pcb->maskedGlobally[temp->info] == MASKED) && (pcb->blocked[temp->info] == UNBLOCKED && pcb->blockedGlobally[temp->info] == UNBLOCKED)) {
			if (prev == 0) {
				old = temp;
				temp = temp->next;
				first = temp;
				old->info = 0;
				delete old;
			}
			else {
				if (temp == last) {
					last = prev;
					return;
				}

				prev->next = temp->next;
				prev = temp;
				temp = temp->next;
			}
		}
		else {
			prev = temp;
			temp = temp->next;
		}
	}

}


