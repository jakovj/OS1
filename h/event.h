#ifndef _event_h_
#define _event_h_

#include "ivtentry.h"

typedef unsigned char IVTNo;
class KernelEv;
class PCB;

class Event {
public:
	Event(IVTNo ivtNo);
	~Event();

	void wait();

	PCB * getEventCreator();
protected:
	friend class KernelEv;
	void signal();
private:
	KernelEv * myImpl;
	PCB * eventCreator;
};
#endif
