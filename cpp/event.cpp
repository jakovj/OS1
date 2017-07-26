#include "event.h"
#include "kevent.h"
#include "pcb.h"

Event::Event(IVTNo ivtNo) : eventCreator(PCB::running) {
	myImpl = KernelEv::getKernelEvent(ivtNo);
}
void Event::wait() {
	if (PCB::running == eventCreator)
		myImpl->wait();
}
void Event::signal() {
	myImpl->signal();
}
Event::~Event() { }

PCB * Event::getEventCreator() { return eventCreator; }
