#ifndef RANDOMMSGKEYIV_H
#define RANDOMMSGKEYIV_H

#include <stddef.h>
#include <stdint.h>

void randomMsg(size_t mlen,uint8_t msg[mlen]);
void randomKey(uint32_t key[8]);
uint64_t randomNonce();

#endif