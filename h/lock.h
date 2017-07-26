#ifndef _lock_h_
#define _lock_h_

class Enable {
public:
	static int val;
	static void enableContextSwitch();
	static void disableContextSwitch();
	Enable();
	static int isEnabled();
};

#endif
