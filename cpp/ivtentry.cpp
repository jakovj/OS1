#include "ivtentry.h"
#include <dos.h>

IVTEntry * IVTEntry::IVTable[256];

IVTEntry::IVTEntry(IVTNo ivtNo, interruptRoutine newEntryRoutine) : entryNum(ivtNo) {
#ifndef BCC_BLOCK_IGNORE
	oldEntryRoutine = getvect(ivtNo);
	setvect(ivtNo, newEntryRoutine);
#endif
	kernelEvent = new KernelEv();
	IVTable[ivtNo] = this;
}
IVTEntry::~IVTEntry() {
#ifndef BCC_BLOCK_IGNORE
	setvect(entryNum, oldEntryRoutine);
#endif
	oldEntryRoutine = 0;
	delete kernelEvent;
	kernelEvent = 0;
	IVTable[entryNum] = 0;
}
KernelEv * IVTEntry::getKernelEvent(IVTNo ivtNo) { return IVTable[ivtNo]->kernelEvent; }

void IVTEntry::callOldEntryRoutine() {
	oldEntryRoutine();
}
KernelEv * IVTEntry::getMyKernelEvent() { return kernelEvent; }

