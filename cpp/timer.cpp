#include "timer.h"
#include "pcb.h"
#include "thread.h"
#include <dos.h>
#include "idle.h"
#include "SCHEDULE.H"
#include "lock.h"
#include <iostream.h>
#include "ksem.h"

extern void tick();

int Timer::contextSwitchRequested = 0;
int Timer::time = 0;
int Timer::process = 0;

interruptRoutine Timer::oldTimerRoutine = 0;

void interrupt Timer::newTimerRoutine(...) {
	static unsigned tsp, tss, tbp;

	if (Enable::isEnabled() && (Timer::contextSwitchRequested || (PCB::running->getTimeSlice() != 0 && PCB::running->getTimeSlice() <= Timer::time))) {
#ifndef BCC_BLOCK_IGNORE
		asm {
			mov tsp, sp
			mov tss, ss
			mov tbp, bp
		}
#endif

		PCB::running->sp = tsp;
		PCB::running->ss = tss;
		PCB::running->bp = tbp;

		if (PCB::running->getState() == RUNNING && PCB::running != Idle::getIdlePCB()) {
			PCB::running->setState(READY);
			Scheduler::put(PCB::running);
		}

		PCB::running = Scheduler::get();

		if (PCB::running == 0 || PCB::running->getState() == FINISHED) { // PAZI NA OVO!!!!
			PCB::running = Idle::getIdlePCB();
		}
		//////

		PCB::running->setState(RUNNING);

		tsp = PCB::running->sp;
		tss = PCB::running->ss;
		tbp = PCB::running->bp;

#ifndef BCC_BLOCK_IGNORE
		asm {
			mov sp, tsp
			mov ss, tss
			mov bp, tbp
		}
#endif

		Timer::time = 0;
	}
	if (Timer::contextSwitchRequested == 0) {

		Timer::time++;
		KernelSem::updateTimeOnSemaphores();
		Timer::oldTimerRoutine();
		tick();
	}
	if (Timer::contextSwitchRequested == 1 && Enable::isEnabled()) {
		Timer::contextSwitchRequested = 0;
		process = 1;
	}
	if (PCB::running != Idle::getIdlePCB() && process == 1) {
		Enable::disableContextSwitch();
		asm pushf
		asm sti
		PCB::running->processSignals();
		process = 0;
		asm popf
		Enable::enableContextSwitch();
	}
}

void Timer::initTimerRoutine() {
#ifndef BCC_BLOCK_IGNORE

	Timer::oldTimerRoutine = getvect(8);
	setvect(8, Timer::newTimerRoutine);

#endif
}

void Timer::restoreTimerRoutine() {
#ifndef BCC_BLOCK_IGNORE

	setvect(8, Timer::oldTimerRoutine);

#endif
}
