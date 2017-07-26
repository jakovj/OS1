#include "kevent.h"
#include "event.h"
#include "pcb.h"
#include "SCHEDULE.H"
#include "ivtentry.h"
#include "lock.h"


KernelEv::KernelEv() : waitingThread(0), val(0) {

}
KernelEv::~KernelEv() {

}
void KernelEv::signal() {
	Enable::disableContextSwitch();
	if (val == -1) {
		if (waitingThread->getState() != FINISHED) {
			waitingThread->setState(READY);
			Scheduler::put(waitingThread);
		}
		val = 0;
		waitingThread = 0;
	}
	Enable::enableContextSwitch();
}

void KernelEv::wait() {
	if (val == 0) {
		Enable::disableContextSwitch();
		waitingThread = PCB::running;
		PCB::running->setState(BLOCKED);
		val = -1;
		Enable::enableContextSwitch();
		dispatch();
	}
}

KernelEv * KernelEv::getKernelEvent(IVTNo ivtNo) { return IVTEntry::getKernelEvent(ivtNo); }
