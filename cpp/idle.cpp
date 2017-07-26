#include "idle.h"
#include <iostream.h>
Thread * Idle::idle = 0;

Idle::Idle() : Thread(4096, 1) {
	if (idle == 0) {
		PCB::idleId = getId();
		Idle::idle = this;
		this->start();
	}
}
Idle::~Idle() { }

void Idle::run() {
	while(1);
}
PCB * Idle::getIdlePCB() { return idle->myPCB; }
void Idle::deleteIdle() {
	if (idle != 0) {
		delete idle;
		idle = 0;
	}

}
