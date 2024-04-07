#ifndef PERMUTATIONS_H_
#define PERMUTATIONS_H_

#include <stdint.h>

#include "api.h"
#include "ascon.h"
#include "config.h"
#include "constants.h"
#include "printstate.h"
#include "round.h"

__forceinline void P12ROUNDS(ascon_state_t* s) {
  ROUND(s, RC0);
  ROUND(s, RC1);
  ROUND(s, RC2);
  ROUND(s, RC3);
  ROUND(s, RC4);
  ROUND(s, RC5);
  ROUND(s, RC6);
  ROUND(s, RC7);
  ROUND(s, RC8);
  ROUND(s, RC9);
  ROUND(s, RCa);
  ROUND(s, RCb);
}

__forceinline void P8ROUNDS(ascon_state_t* s) {
  ROUND(s, RC4);
  ROUND(s, RC5);
  ROUND(s, RC6);
  ROUND(s, RC7);
  ROUND(s, RC8);
  ROUND(s, RC9);
  ROUND(s, RCa);
  ROUND(s, RCb);
}

__forceinline void P6ROUNDS(ascon_state_t* s) {
  ROUND(s, RC6);
  ROUND(s, RC7);
  ROUND(s, RC8);
  ROUND(s, RC9);
  ROUND(s, RCa);
  ROUND(s, RCb);
}

#endif /* PERMUTATIONS_H_ */
