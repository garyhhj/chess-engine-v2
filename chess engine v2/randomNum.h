#pragma once
#include <stdint.h>


class Random {
	
public: 
	Random() = default; 
	Random(const Random&) = delete; 
	Random operator=(const Random&) = delete; 

	static Random& get(); 

	static uint64_t getRandomUint64(); 
	static uint64_t getRandomUint32(); 

private: 
	//uint64_t IgetRandomUint64Helper(); 
	uint64_t IgetRandomUint64();
	uint64_t IgetRandomUint32();

	inline static uint32_t currState = 1804289383;
};