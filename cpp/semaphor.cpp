#include "semaphor.h"
#include "ksem.h"
#include "list.h"
#include "semlist.h"
Semaphore::Semaphore(int init) {
	myImpl = new KernelSem(init);
	KernelSem::allSemaphores->add(myImpl);
}
Semaphore::~Semaphore(){
	KernelSem::allSemaphores->remove(myImpl);
	delete myImpl;
}
int Semaphore::wait(Time maxTimeToWait) {
	if (maxTimeToWait == 0)
		maxTimeToWait = 0xFFFF;
	int ret = myImpl->wait(maxTimeToWait);
	return ret;
}
void Semaphore::signal() {
	 myImpl->signal();
}
int Semaphore::val() const {
	return myImpl->val();
}

