#include "movelist.h"


/********************
*
*Movelist
*
*********************/

Movelist::Movelist() : movelist(new uint64_t[256]), index(0) {}


//rule of 3.5
Movelist::Movelist(const Movelist& rhs) : movelist(new uint64_t[256]), index(rhs.index) {
	for (int i = 0; i < index; ++i) {
		movelist[i] = rhs.movelist[i]; 
	}
}

Movelist& Movelist::operator=(Movelist& rhs) {
	Movelist::swap(*this, rhs); 
	rhs.movelist = nullptr; 
}


Movelist::~Movelist() {
	delete[] movelist; 
}

void Movelist::swap(Movelist& m1, Movelist& m2) {
	uint64_t* ptr = m1.movelist; 
	m1.movelist = m2.movelist; 
	m2.movelist = ptr; 

	int temp = m1.index; 
	m1.index = m2.index; 
	m2.index = temp; 
}
