#pragma once
#include <stdint.h>
#include <iostream>

class Movelist {
public: 
	Movelist(); 
	Movelist(const Movelist& rhs);
	Movelist& operator=(Movelist& rhs);
	~Movelist(); 

	consteval void pushBack(uint64_t move); 
	

private:
	void swap(Movelist& m1, Movelist& m2); 

	uint64_t* movelist;
	int index; 
};