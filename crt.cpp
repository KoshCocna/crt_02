// x^ p mod m with crt
#include<iostream>
#include <vector>
#include <cassert>
#include <bit>
#include <cstdlib>
#include <climits>
#include "mod_ops.h"
using namespace std;

#define verify(f) do {if (!(f)) {std::cerr << "Verification failure at " << __FILE__ << ':' << __LINE__ << '\n'; std::abort();}}while(0)

typedef unsigned long long ull;

vector<ull> primeFactors(ull n)
{
    vector<ull> v;
    if (!(n & 1))
	{
		auto pw2 = std::countr_zero(n);
		n >>= pw2;
		v.push_back(1ull << pw2);
	}

	auto floor_sqrt_n_plus_1 = n >> (sizeof n * CHAR_BIT - 1 - std::countl_zero(n)) / 2;
    for (ull i = 3; i < floor_sqrt_n_plus_1; i = i + 2)
    {
        if (n % i == 0)
        {
            auto base_num = i;
			while (!((n /= i) % i))
				base_num *= i;
            v.push_back(base_num);
        }
    }

    if (n > 2)
        v.push_back(n);
    return v;
}

ull gcdExtended(ull a, ull b, ull* x, ull* y);

ull modInverse(ull A, ull M)
{
    ull x, y;
    ull g = gcdExtended(A, M, &x, &y);
	verify(g == 1);
    // m is added to handle negative x
	ull res = ((long long) x < 0?x+M:x) % M;
    return res;
}

ull gcdExtended(ull a, ull b, ull* x, ull* y)
{

    // Base Case
    if (a == 0) {
        *x = 0, * y = 1;
        return b;
    }

    // To store results of recursive call
    ull x1, y1;
    ull gcd = gcdExtended(b % a, a, &x1, &y1);

    // Update x and y using results of recursive
    // call
    *x = y1 - (b / a) * x1;
    *y = x1;

    return gcd;
}

ull pow_mod(ull x, ull n, ull m)
{
	ull acc = x, res = 1;
	while (!!n)
	{
		if (x & 1)
			res = mul_mod(res, acc, m);
		acc = mul_mod(acc, acc, m);
		n >>= 1;
	}
	return res;
}

ull crt(ull x, ull p, ull mod) {
    auto m_i = primeFactors(mod); //factorization ::: getting m_i
    ull res = 0ull;
    ull b_i, b_i_inverse, a_i=0ull;
    ull xx = 1ull;
    for (auto it = m_i.begin(); it != m_i.end(); ++it) {

        b_i = mod / (*it);
        b_i_inverse = modInverse((mod / (*it)), (*it));
        a_i = pow_mod(x % *it, p, *it);
        res = add_mod(res, mul_mod(a_i, mul_mod(b_i, b_i_inverse, mod), mod), mod);
    }
    return res;
}

int main() {
	cout << "pow(12345, 456789, 1000000000) = " << pow_mod(12345, 456789, 10000000000ull) << '\n';
    ull res = crt(12345ull, 456789ull, 10000000000ull);
    cout << "answer is " << res << endl;

    return 0;
}
