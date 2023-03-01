#pragma once
#include <stdint.h>

class Movelist {
	Movelist(); 
	Movelist(const Movelist& rhs);
	Movelist& operator=(Movelist& rhs);
	~Movelist(); 

private:
	void swap(Movelist& m1, Movelist& m2); 

	uint64_t* movelist;
	int index; 
};