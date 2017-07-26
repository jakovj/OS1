#ifndef _kevent_h_
#define _kevent_h_

typedef unsigned char IVTNo;
class PCB;

class KernelEv {
public:
	void signal();
private:
	KernelEv();
	~KernelEv();

	static KernelEv * getKernelEvent(IVTNo ivtNo);

	void wait();

	PCB * waitingThread;

	int val;
	friend class Event;
	friend class IVTEntry;
};





#endif
