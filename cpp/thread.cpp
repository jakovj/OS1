#include "thread.h"
#include "list.h"
#include "pcb.h"
#include "SCHEDULE.H"
#include "lock.h"
#include "timer.h"
#include <iostream.h>
#include "idle.h"
#include "siglist.h"

ID Thread::getId () { return myPCB->getId(); }

ID Thread::getParentId() { return myPCB->parent->getId(); }

ID Thread::getRunningId () { return PCB::running->getId(); }

Thread * Thread::getThreadById(ID id) {
	PCB * pcb = PCB::PCBList->findById(id);
	return pcb->getThread();
}

Thread::Thread(StackSize stackSize, Time timeSlice) {
	myPCB = new PCB(this, stackSize, timeSlice);
}
Thread::Thread(int i) {
	myPCB = new PCB(this);
}

void Thread::start() {
	if (myPCB->getState() == NEW) {
		myPCB->initStack();
		myPCB->setState(READY);
		if (myPCB->getId() != PCB::idleId)
			Scheduler::put(myPCB);
	}
}
Thread::~Thread () {
	waitToComplete();
	delete myPCB;
	myPCB = 0;
}
void Thread::waitToComplete() {
	if (myPCB->getState() != FINISHED && PCB::running != Idle::getIdlePCB() && PCB::running != this->myPCB) {

		Enable::disableContextSwitch();
		PCB::running->setState(BLOCKED);
		myPCB->getWaitingList()->add(PCB::running);

		Enable::enableContextSwitch();

		dispatch();
	}
}

void dispatch() {

	Timer::contextSwitchRequested = 1;
	asm int 8h;

}

//****************************************************
//****************************************************
//				Signal associated
//****************************************************
//****************************************************

void Thread::signal(SignalId signal) {
	Enable::disableContextSwitch();
	if (signal == 2 && myPCB->getState() == FINISHED && getHandler(signal) != 0) {
		getHandler(signal)();
		Enable::enableContextSwitch();
		return;
	}

	if (signal < SIGNALNUM && myPCB != PCB::running) {
		if (myPCB->getState() == PAUSED && myPCB->masked[signal] == UNMASKED && myPCB->maskedGlobally[signal] == UNMASKED) {
			myPCB->setState(READY);
			Scheduler::put(myPCB);
		}
		if (myPCB->masked[signal] != MASKED && myPCB->masked[signal] != MASKED) {
			myPCB->receivedSignals->add(signal);
		}
	}
	Enable::enableContextSwitch();
}
void Thread::pause() {

	PCB::running->setState(PAUSED);
	dispatch();

}
void Thread::registerHandler(SignalId signal, SignalHandler handler) {
	if (signal < SIGNALNUM) {
		myPCB->myHandlers[signal] = handler;
	}
}
SignalHandler Thread::getHandler(SignalId signal) {
	if (signal < SIGNALNUM) {
		return myPCB->myHandlers[signal];
	}
	return 0;
}


// ************************************************
// Signal masking
void Thread::maskSignal(SignalId signal) {
	if (signal < SIGNALNUM) {
		myPCB->masked[signal] = MASKED;
	}
}
void Thread::unmaskSignal(SignalId signal) {
	if (signal < SIGNALNUM) {
			myPCB->masked[signal] = UNMASKED;
		}
}
void Thread::maskSignalGlobally(SignalId signal) {
	if (signal < SIGNALNUM) {
		PCB::maskedGlobally[signal] = MASKED;
	}
}
void Thread::unmaskSignalGlobally(SignalId signal) {
	if (signal < SIGNALNUM) {
		PCB::maskedGlobally[signal] = UNMASKED;
	}
}
//**************************************************
//Signal blocking
void Thread::blockSignal(SignalId signal) {
	if (signal < SIGNALNUM) {
			myPCB->blocked[signal] = BLOCKD;
	}
}
void Thread::unblockSignal(SignalId signal) {
	if (signal < SIGNALNUM) {
			myPCB->blocked[signal] = UNBLOCKED;
	}
}
void Thread::blockSignalGlobally(SignalId signal) {
	if (signal < SIGNALNUM) {
			PCB::blockedGlobally[signal] = BLOCKD;
	}
}
void Thread::unblockSignalGlobally(SignalId signal) {
	if (signal < SIGNALNUM) {
			PCB::blockedGlobally[signal] = UNBLOCKED;
	}
}
//***************************************************
