#ifndef _timer_h_
#define _timer_h_

typedef void interrupt (*interruptRoutine)(...);

class Timer {
public:
	static int contextSwitchRequested;
	static int time;
	static int process;

	static interruptRoutine oldTimerRoutine;
	static void interrupt newTimerRoutine(...);

	static void initTimerRoutine();
	static void restoreTimerRoutine();

	static void interrupt timer(...);

private:
	Timer();
};



#endif
