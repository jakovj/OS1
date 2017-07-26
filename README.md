# Operating Systems 1 Project

## Getting Started

Implementation of a kernel which support multithreading and timw sharing. Also, concepts like semaphores, events, 
signals (thread-thread communication) are covered.

## Prerequisites

For testing purposes the 32-bit Windows is needed.

## Built With
* Eclipse (with bcc toolchain)

## Tests
```
Semaphore * s = new Semaphore();

void T::run() {
	for (unsigned long i = 0; i < 1000; i++){
		Enable::disableContextSwitch();
		if (i % 100 == 0) cout << "THREAD_" << PCB::running->getId() << "(" << i << ")" << endl;
		Enable::enableContextSwitch();
		if (i == 500) s->wait(400);
		for (int j=0;j<1000;j++){
			for (int k=0;k<1000;k++);
		}
	}
}

int userMain(int argc, char * argv[]) {
	T * t1 = new T(4096,1);
	T * t2 = new T(4096,2);
	T * t3 = new T(4096,8);
	t1->start();
	t2->start();
	t3->start();

	return 0;
}

void tick() { }
```

## Authors

Jakov Jezdić

## License

All rights reserved.
Do not reuse! Write your own.



