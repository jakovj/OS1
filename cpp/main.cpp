#include "Timer.h"
#include "Idle.h"
#include "SCHEDULE.H"
#include "lock.h"
#include <iostream.h>
//#include "user.h"
#include "list.h"
#include "ksem.h"


extern int userMain(int argc, char* argv[]);

int main(int argc, char* argv[]) {
	new Thread(0);
	new Idle();

	Timer::initTimerRoutine();

	int retVal = userMain(argc, argv);

	List * l = PCB::getActivePCBList();
	PCB * temp = (PCB*)(l->removeFirst());

	/*while (temp != 0){
		if (temp->getId() != PCB::mainId && temp->getId() != PCB::idleId) {
			temp->getThread()->waitToComplete();
		}
		temp = (PCB*)(l->removeFirst());
	}*/
	cout << retVal << endl;

	PCB::deletePCBStaticLists();
	KernelSem::deleteAllSemaphores();
	Timer::restoreTimerRoutine();
	Idle::deleteIdle();
	return retVal;
}


