#include "mod_ops.h"
#include <climits>

#if defined(__GNUC__) && INTWORD_MAX == 0xffffffffffffffffu
IntegerWord add_mod(IntegerWord a, IntegerWord b, IntegerWord mod)
{
	return (IntegerWord) (((unsigned __int128) a + b) % mod);
}
IntegerWord mul_mod(IntegerWord a, IntegerWord b, IntegerWord mod)
{
	return (IntegerWord) (((unsigned __int128) a * b) % mod);
}
#elif defined(__GNUC__) && INTWORD_MAX == 0xffffffffu
IntegerWord add_mod(IntegerWord a, IntegerWord b, IntegerWord mod)
{
	return (IntegerWord) (((unsigned __int64) a + b) % mod);
}
IntegerWord mul_mod(IntegerWord a, IntegerWord b, IntegerWord mod)
{
	return (IntegerWord) (((unsigned __int64) a * b) % mod);
}
#elif defined(_MSC_VER) && INTWORD_MAX == 0xffffffffffffffffu
IntegerWord add_mod(IntegerWord a, IntegerWord b, IntegerWord mod)
{
	//unsigned char _addcarry_u64 (unsigned char c_in, unsigned __int64 a, unsigned __int64 b, unsigned __int64 * out)
	IntegerWord result = a + b; //result < 4*w
	if (result < a)
		return (result % mod + -mod % mod) % mod;
	return result % mod;
}
#include <intrin.h>
IntegerWord mul_mod(IntegerWord a, IntegerWord b, IntegerWord mod)
{
	unsigned __int64 result_high, result_low = _umul128((unsigned __int64) (a % mod), (unsigned __int64) (b % mod), &result_high) % mod;
	if (result_high != 0)
		result_low = add_mod(result_low, mul_mod(result_high, -mod % mod, mod), mod);
	return result_low;
}
#elif defined(_MSC_VER) && INTWORD_MAX == 0xffffffffu
IntegerWord add_mod(IntegerWord a, IntegerWord b, IntegerWord mod)
{
	return (IntegerWord) (((unsigned __int64) a + b) % mod);
}
IntegerWord mul_mod(IntegerWord a, IntegerWord b, IntegerWord mod)
{
	return (IntegerWord) (((unsigned __int64) a * b) % mod);
}
#else
IntegerWord add_mod(IntegerWord a, IntegerWord b, IntegerWord mod)
{
	IntegerWord result = a + b;
	if (result < a)
		//No overflow since: result % mod + -mod % mod = result % mod + (sizeof(IntegerWord) * CHAR_BIT - mod) % mod <= (mod - 1) + (sizeof(IntegerWord) * CHAR_BIT - mod) = sizeof(IntegerWord) * CHAR_BIT - 1
		return (result % mod + -mod % mod) % mod;
	return result % mod;
}
IntegerWord mul_mod(IntegerWord a, IntegerWord b, IntegerWord mod)
{
	a %= mod;
	b %= mod;
	IntegerWord x1 = a >> sizeof(IntegerWord) * CHAR_BIT / 2; //high half
	IntegerWord x0 = a & (IntegerWord) -1 >> sizeof(IntegerWord) * CHAR_BIT / 2; //low half
	IntegerWord y1 = b >> sizeof(IntegerWord) * CHAR_BIT / 2; //high half
	IntegerWord y0 = b & (IntegerWord) -1 >> sizeof(IntegerWord) * CHAR_BIT / 2; //low half
	IntegerWord res_lo = x0 * y0;
	IntegerWord res_hi = x1 * y1;
	IntegerWord interm1 = x0 * y1;
	IntegerWord interm2 = x1 * y0;
	IntegerWord tmp = interm1 + interm2;
	if (interm1 >= -interm2 && interm2 != 0)
		interm1 = (tmp >> sizeof(IntegerWord) * CHAR_BIT / 2) + ((IntegerWord) 1 << sizeof(IntegerWord) * CHAR_BIT / 2);
	else
		interm1 = tmp >> sizeof(IntegerWord) * CHAR_BIT / 2;
	interm2 = tmp << sizeof(IntegerWord) * CHAR_BIT / 2;
	res_hi += interm1;
	if (interm2 >= -res_lo && res_lo != 0)
		++res_hi;
	res_lo += interm2;
	if (res_hi)
		return add_mod(res_lo % mod, times_word(res_hi, mod), mod);
	return  res_lo % mod;
}
#endif
