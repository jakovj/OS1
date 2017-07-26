#include "lock.h"
int Enable::val = 1;
Enable::Enable() { }
void Enable::enableContextSwitch() {
	if (val < 1)
		val++;
}
void Enable::disableContextSwitch() {
	if (val <= 1)
		val--;
}
int Enable::isEnabled() {
	if (val == 1) return 1;
	return 0;
 }



