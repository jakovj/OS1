#include "pcb.h"
#include <dos.h>
#include "thread.h"
#include "list.h"
#include "SCHEDULE.H"
#include <iostream.h>
#include "idle.h"
#include "lock.h"
#include "siglist.h"

PCB * PCB::running = 0;
List * PCB::PCBList = new List();
List * PCB::activePCBList = new List();
int PCB::Id = 0;
int PCB::mainId = PCB::Id + 1;
int PCB::idleId = PCB::Id;


BlockingState PCB::blockedGlobally[SIGNALNUM];
MaskingState PCB::maskedGlobally[SIGNALNUM];

PCB::PCB(Thread * thread) : stack(0), myThreadState(READY), myThreadTimeSlice(1), deblockedBySignal(1), parent(0) {
	myThreadId = PCB::Id ++;
	myThread = thread;
	PCB::mainId = myThreadId;
	PCB::running = this;
	PCBList->add(this);
	activePCBList->add(this);

	receivedSignals = new SigList();
	for (int i = 0; i < SIGNALNUM; i++) {
		masked[i] = UNMASKED;
		blocked[i] = UNBLOCKED;
		myHandlers[i] = 0;
	}

	myHandlers[0] = signal0Handler;  // children will inherit
}
PCB::PCB(Thread * thread, StackSize stackSize, Time timeSlice) : stack(0), myThreadState(NEW),  myThreadTimeSlice(timeSlice), myThread(thread), deblockedBySignal(1) {
	myThreadStackSize = stackSize >> 1;
	myThreadId = PCB::Id++;
	waitingList = new List();
	PCBList->add(this);
	activePCBList->add(this);

	parent = PCB::running;
	receivedSignals = new SigList();
	for (int i = 0; i < SIGNALNUM; i++) {			//*******************************
		masked[i] = parent->masked[i];				//*******************************
		blocked[i] = parent->blocked[i];			// Signal preferences inheritance
		myHandlers[i] = parent->myHandlers[i];		//*******************************
	}
}
PCB::~PCB(){

	myThread->myPCB = 0;
}
void PCB::initStack() {
#ifndef BCC_BLOCK_IGNORE
	stack = new unsigned[myThreadStackSize];

	stack[myThreadStackSize - 1] = 0x200;
	stack[myThreadStackSize - 2] = FP_SEG(PCB::wrapper);
	stack[myThreadStackSize - 3] = FP_OFF(PCB::wrapper);

	ss = FP_SEG(&stack[myThreadStackSize - 12]);
	sp = FP_OFF(&stack[myThreadStackSize - 12]);
	bp = sp;


#endif
}
void PCB::setState(State state) {
	myThreadState = state;
}
StackSize PCB::getStackSize() { return myThreadStackSize; }
void PCB::notDeblockedBySignal() { this->deblockedBySignal = 0; }
Time PCB::getTimeSlice() { return myThreadTimeSlice; }
State PCB::getState() { return myThreadState; }
ID PCB::getId() { return myThreadId; }
Thread * PCB::getThread() { return myThread; }
List * PCB::getPCBList() { return PCBList; }
List * PCB::getActivePCBList() { return activePCBList; }
void PCB::deletePCBStaticLists() {
	delete PCBList;
	delete activePCBList;
}

void PCB::wrapper() {
	PCB::running->myThread->run();

	Enable::disableContextSwitch();

	PCB::running->parent->myThread->signal(1);

	PCB::running->myThreadState = FINISHED;
	PCB::running->myThread->signal(2);
	PCB::running->activateWaiting();

	if (PCB::running->stack != 0) {
		delete [] PCB::running->stack;
		PCB::running->stack = 0;
	}
	if (PCB::running->waitingList != 0) {
		delete PCB::running->waitingList;
		PCB::running->waitingList = 0;
	}
	if (PCB::running->receivedSignals != 0) {
		delete PCB::running->receivedSignals;
		PCB::running->receivedSignals = 0;
	}

	Enable::enableContextSwitch();

	dispatch();
}

List * PCB::getWaitingList() { return waitingList; }

void PCB::activateWaiting() {
	List * l = getWaitingList();

	if (l != 0) {
		PCB * temp = l->removeFirst();
		while (temp != 0) {
			if (temp->getState() != FINISHED) {
				temp->setState(READY);
				temp->deblockedBySignal = 0;
				Scheduler::put(temp);
			}
			temp = l->removeFirst();
		}
	}
}
void PCB::processSignals() {
	receivedSignals->processSignals(this);
}
void PCB::signal0Handler() {

	if (PCB::running->parent != 0)
		PCB::running->parent->myThread->signal(1);

	PCB::running->myThreadState = FINISHED;
	PCB::running->myThread->signal(2);
	PCB::running->activateWaiting();

	if (PCB::running->stack != 0) {
		delete [] PCB::running->stack;
		PCB::running->stack = 0;
	}
	if (PCB::running->waitingList != 0) {
		delete PCB::running->waitingList;
		PCB::running->waitingList = 0;
	}
	if (PCB::running->receivedSignals != 0) {
		delete PCB::running->receivedSignals;
		PCB::running->receivedSignals = 0;
	}


	Enable::enableContextSwitch();

	dispatch();
}
ID PCB::getParentId() {
	return parent->getId();
}

