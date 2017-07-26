#ifndef _pcb_h_
#define _pcb_h_

#include "list.h"
#include "thread.h"


enum State {NEW, RUNNING, BLOCKED, READY, FINISHED, PAUSED};

enum MaskingState {UNMASKED, MASKED};   // added for
enum BlockingState {UNBLOCKED, BLOCKD};// signal implementation

#define SIGNALNUM 16

class SigList;

class PCB {
public:
	static PCB * running;

	unsigned * stack;
	unsigned ss;
	unsigned sp;
	unsigned bp;



	static void wrapper();

	StackSize getStackSize();
	Time getTimeSlice();
	State getState();
	ID getId();
	Thread * getThread();
	static List * getPCBList();
	static List * getActivePCBList();
	static void deletePCBStaticLists();

	void setState(State state);
	void initStack();

	List * getWaitingList();
	void activateWaiting();

	static ID mainId, idleId; // reserved Ids

	int deblockedBySignal;
	Time maxTimeToWait;
	void notDeblockedBySignal();

	PCB(Thread * thread, StackSize stackSize, Time timeSlice);	// regular constructor
	PCB(Thread * thread); 										// init PCB constructor
	~PCB();

	//added for Signal implementation
	void processSignals();
	ID getParentId();

protected:
	friend class Thread;
	friend class SigList;
private:
	Thread * myThread;
	StackSize myThreadStackSize;
	Time myThreadTimeSlice;
	State myThreadState;
	ID myThreadId;

	static int Id;

	static List * PCBList;
	static List * activePCBList;
	List * waitingList ;

	//added for Signal implementation
	PCB * parent;
	SigList * receivedSignals;
	SignalHandler myHandlers[SIGNALNUM];


	MaskingState masked[SIGNALNUM];
	static MaskingState maskedGlobally[SIGNALNUM];

	BlockingState blocked[SIGNALNUM];
	static BlockingState blockedGlobally[SIGNALNUM];

	static void signal0Handler();


};

#endif
