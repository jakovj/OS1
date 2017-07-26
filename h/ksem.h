#ifndef KSEM_H_
#define KSEM_H_

class PCB;
class List;

typedef unsigned int Time;
class KernelSem {
private:
	int v;
	List * waitingThreads;

	void waitAllThreads();
	void blockOnSemaphore(PCB * pcb, Time maxTimeToWait);
	PCB * deblockOneThread();

	int wait(Time maxTimeToWait);
	void signal();
	int val() const;

	KernelSem(int init);
	~KernelSem();
	friend class Semaphore;
	friend class List;
	friend class SemList;



public:
	SemList * getAllSemaphoreList();
	List * getWaitingThreadsList();
	static void updateTimeOnSemaphores();
	static void deleteAllSemaphores();

	static SemList * allSemaphores;


};

#endif
