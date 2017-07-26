
#ifndef _siglist_h_
#define _siglist_h_

typedef unsigned SignalId;

class SigList {
private:
	struct Element {
		SignalId info;
		Element * next;
		Element(SignalId i);
		~Element() { }
	};


	Element * first, * last;
	friend class PCB;

public:
	SigList();
	~SigList();

	void add(SignalId el);
	void processSignals(PCB * pcb);
};

#endif

