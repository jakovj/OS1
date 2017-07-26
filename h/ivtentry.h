#ifndef _ivtentry_h_
#define _ivtentry_h_
#include "kevent.h"

typedef unsigned char IVTNo;
typedef void interrupt (*interruptRoutine)(...);
class KernelEv;

class IVTEntry {
public:
	IVTEntry(IVTNo ivtNo, interruptRoutine newEntryRoutine);
	~IVTEntry();
	void callOldEntryRoutine();
	KernelEv * getMyKernelEvent();

private:

	IVTNo entryNum;
	interruptRoutine oldEntryRoutine;
	KernelEv * kernelEvent;

	static IVTEntry * IVTable[256];
	static KernelEv * getKernelEvent(IVTNo ivtNo);

	friend class KernelEv;

};


#define PREPAREENTRY(entryNum, callOld)\
	void interrupt interrupt##entryNum(...);\
	IVTEntry IVTEntry##entryNum(entryNum, interrupt##entryNum);\
	void interrupt interrupt##entryNum(...) {\
		IVTEntry##entryNum.getMyKernelEvent()->signal();\
		if (callOld == 1) IVTEntry##entryNum.callOldEntryRoutine();\
	}\

#endif
