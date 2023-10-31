#include <stdlib.h>
#include <assert.h>

#include <gmpxx.h>
#include <iostream>

extern "C" {
#include "./gw_utility.h"
}

int main() {
    mpz_class r, b, e, p;

    mpz_inp_str(p.get_mpz_t(), NULL, 10);
    assert(p % 4 == 1);
    assert(0 != mpz_probab_prime_p(p.get_mpz_t(), 30));

    e = p / 4;

    // deterministic fast search for smallest quadratic non-residue
    b = 2;
    while (mpz_kronecker(b.get_mpz_t(), p.get_mpz_t()) != -1) {
        mpz_nextprime(b.get_mpz_t(), b.get_mpz_t());
    }
    std::cerr << "smallest quadratic non-residue prime: " << b << "\n";

    gw_powm(r.get_mpz_t(), b.get_mpz_t(), e.get_mpz_t(), p.get_mpz_t());

    std::cout << r;
}
