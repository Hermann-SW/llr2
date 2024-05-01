#include <stdlib.h>
#include <assert.h>

#include <gmpxx.h>
#include <iostream>

extern "C" {
#include "./gw_utility.h"
}

int main(int argc, char *argv[]) {
    mpz_class r, b, e, p, one = mpz_class("1");

    printf("bytes=%ld\n", mpz_inp_str(p.get_mpz_t(), NULL, 10));

    e = p-1;

    // deterministic fast search for smallest quadratic non-residue
    b = (argc < 2) ? 2 : atoi(argv[1]);
    while (mpz_kronecker(b.get_mpz_t(), p.get_mpz_t()) != -1) {
        mpz_nextprime(b.get_mpz_t(), b.get_mpz_t());
    }
    std::cerr << "smallest quadratic non-residue: " << b << "\n";

    gw_powm(r.get_mpz_t(), b.get_mpz_t(), e.get_mpz_t(), p.get_mpz_t());

    std::cout << (r == one) << "\n";
}
