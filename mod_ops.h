#pragma once
typedef unsigned long long IntegerWord;

IntegerWord add_mod(IntegerWord a, IntegerWord b, IntegerWord m); //(a + b) mod m
IntegerWord mul_mod(IntegerWord a, IntegerWord b, IntegerWord m); //(a * b) mod m
#define times_word(x, mod) mul_mod(x, -mod, mod) //(a * w) mod m
