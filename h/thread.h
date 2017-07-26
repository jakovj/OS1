#ifndef _thread_h_
#define _thread_h_


typedef unsigned long StackSize;
const StackSize defaultStackSize = 4096;
typedef unsigned int Time; // time, x 55ms
const Time defaultTimeSlice = 2; // default = 2*55ms
typedef int ID;

// added for Signal implementation
typedef void (*SignalHandler)();

typedef unsigned SignalId;


class PCB; // Kernel's implementation of a user's thread

class Thread {
public:
	void start();
	void waitToComplete();
	virtual ~Thread();
	Thread (int i);

	ID getId();

	// ********************************************************
	// added for Signal implementation
	ID getParentId();
	void signal(SignalId signal);

	void registerHandler(SignalId signal, SignalHandler handler);
	SignalHandler getHandler(SignalId);

	void maskSignal(SignalId signal);
	static void maskSignalGlobally(SignalId signal);
	void unmaskSignal(SignalId signal);
	static void unmaskSignalGlobally(SignalId signal);

	void blockSignal(SignalId signal);
	static void blockSignalGlobally(SignalId signal);
	void unblockSignal(SignalId signal);
	static void unblockSignalGlobally(SignalId signal);

	static void pause();

	// *******************************************************


	static ID getRunningId();
	static Thread * getThreadById(ID id);
protected:
	friend class PCB;
	friend class Idle;
	Thread (StackSize stackSize = defaultStackSize, Time timeSlice = defaultTimeSlice);

	virtual void run() { }
private:
	PCB* myPCB;
};
void dispatch ();
#endif
