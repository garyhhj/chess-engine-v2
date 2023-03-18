#include "randomNum.h"

/********************
*
*Random
*
*********************/

Random& Random::get() {
	static Random instance;
	return instance;
}


uint64_t Random::getRandomUint64() {
	return Random::get().IgetRandomUint64(); 
}

uint64_t Random::IgetRandomUint64Helper() {
	uint64_t num1, num2, num3, num4;

	num1 = Random::IgetRandomUint32() & 0xffff;
	num2 = Random::IgetRandomUint32() & 0xffff;
	num3 = Random::IgetRandomUint32() & 0xffff;
	num4 = Random::IgetRandomUint32() & 0xffff;

	return num1 | num2 << 16 | num3 << 32 | num4 << 48;
}

uint64_t Random::IgetRandomUint64() {
	return Random::IgetRandomUint64Helper() & Random::IgetRandomUint64Helper() & Random::IgetRandomUint64Helper(); 
}

uint64_t Random::getRandomUint32() {
	return Random::get().IgetRandomUint32(); 
}

uint64_t Random::IgetRandomUint32() {
	uint32_t nxtState = Random::currState;

	//xor shifts 
	nxtState ^= nxtState << 13;
	nxtState ^= nxtState >> 17;
	nxtState ^= nxtState << 5;

	Random::currState = nxtState;
	return nxtState;
}