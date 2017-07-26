#ifndef _list_h_
#define _list_h_
#include "thread.h"

class PCB;
class KernelSem;


class List {
private:
	struct Element {
		PCB * info;
		Element * next;
		Element(PCB * i);
		~Element() { }
	};


	Element * first, * last;
	friend class KernelSem;
	friend class SemList;
public:
	List();
	~List();

	void add(PCB * el);
	void remove(PCB * el);

	PCB * findById(ID id); // PCB exclusive
	PCB * removeFirst();

	void addByDiff(PCB * pcb, Time maxTimeToWait); //KernelSem exclusive
	void updateTimeOnSemaphore(KernelSem * sem); //KernelSem exclusive



};

#endif

