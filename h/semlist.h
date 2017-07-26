#ifndef _semlist_h_
#define _semlist_h_

class KernelSem;


class SemList {
private:
	struct Element {
		KernelSem * info;
		Element * next;
		Element(KernelSem * i);
		~Element() { }
	};

	Element * first, * last;
	friend class KernelSem;
	friend class List;
public:
	SemList();
	~SemList();

	void add(KernelSem * el);
	void remove(KernelSem * el);

	KernelSem * removeFirst();
	static void updateTime();
};





#endif
