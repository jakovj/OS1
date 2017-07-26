#ifndef _idle_h_
#define _idle_h_

#include "thread.h"
#include "pcb.h"

class Idle : public Thread {
public:
	Idle();
	virtual ~Idle();
	virtual void run();

	static Thread * idle;
	static PCB * getIdlePCB();
	static void deleteIdle();
};


#endif
