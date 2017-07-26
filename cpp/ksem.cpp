#include "semaphor.h"
#include "ksem.h"
#include "SCHEDULE.H"
#include "timer.h"
#include "pcb.h"
#include <iostream.h>
#include "lock.h"
#include "semlist.h"
#include "idle.h"


SemList * KernelSem::allSemaphores = new SemList();

KernelSem::KernelSem(int init) : v(init) { waitingThreads = new List(); }
KernelSem::~KernelSem() {
	waitAllThreads();
	if (waitingThreads != 0) {
		delete waitingThreads;
		waitingThreads = 0;
	}
}
SemList * KernelSem::getAllSemaphoreList() { return allSemaphores; }
int KernelSem::wait(Time maxTimeToWait) {
	Enable::disableContextSwitch();
	v--;
	if (v < 0) {
		PCB::running->deblockedBySignal = 1;
		PCB::running->setState(BLOCKED);
		blockOnSemaphore(PCB::running, maxTimeToWait);

		Enable::enableContextSwitch();
		dispatch();
		Enable::disableContextSwitch();


		if (PCB::running->deblockedBySignal == 0) {
			Enable::enableContextSwitch();
			PCB::running->deblockedBySignal = 1;
			return 0;
		}
		else {
			Enable::enableContextSwitch();
			PCB::running->deblockedBySignal = 1;
			return 1;
		}
	}
	Enable::enableContextSwitch();
	return 1;
}
void KernelSem::signal() {
	Enable::disableContextSwitch();
	v++;
	if (v <= 0) {
		PCB * pcb = deblockOneThread();
		if (pcb != 0 ) {
			if (pcb->getState() != FINISHED) {
				pcb->setState(READY);
				Scheduler::put(pcb);
			}
		}
	}
	Enable::enableContextSwitch();
}
int KernelSem::val() const {
	return v;
}
void KernelSem::waitAllThreads() {
	while (v < 0) {
		waitingThreads->first->info->deblockedBySignal = 0;
		signal();
	}
}
void KernelSem::blockOnSemaphore(PCB * pcb, Time maxTimeToWait){
	Enable::disableContextSwitch();
	waitingThreads->addByDiff(pcb, maxTimeToWait);
	Enable::enableContextSwitch();
}
PCB * KernelSem::deblockOneThread() {
	PCB * pcb = waitingThreads->removeFirst();
	return pcb;
}
void KernelSem::updateTimeOnSemaphores() {
	if (allSemaphores != 0){
		SemList::updateTime();
	}

}
List * KernelSem::getWaitingThreadsList() { return waitingThreads; }
void KernelSem::deleteAllSemaphores() {
	if (allSemaphores != 0) {
		delete allSemaphores;
		allSemaphores = 0;
	}

}


