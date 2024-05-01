## Motivation

Initial code drop was done from this forum posting of [paulunderwood](https://www.mersenneforum.org/member.php?u=156):  
[https://www.mersenneforum.org/showthread.php?p=641952#post641946](https://www.mersenneforum.org/showthread.php?p=641952#post641946)  
It contained his gw_utility code, that allows for eg. <kbd>gw_powm ( r, b, e, n )</kbd> making use of superefficient gwnum library, to produce result of libgmp <kbd>powm( r, b, e, n )</kbd>.

"gw_powm()" is used in "sqrtm1.cc" code to compute "sqrt(-1) (mod p)", as done until now with Jean Penné's (patched for writing 2nd last computed value) LLR software for computing that, eg. for largest known 11.9million digit non-Mersenne prime:  
[https://github.com/Hermann-SW/11887192-digit-prime](https://github.com/Hermann-SW/11887192-digit-prime)

## Build

```
make
```
When linking on AMD CPUs under Ubuntu 22.04, there is a ld relocation warning.

There are "clean", "cpplint" and "cppcheck" targets as well.

## Verification of example_usage
```
$ echo 1001111 | ./prg
35621
$ gp -q < <(echo "Mod(3,1001111)^(1001111-2)")
Mod(35621, 1001111)
$
```

```
$ echo -n "s=" > out
$ gp -q < <(echo "print1(289*2^18502+1)") | ./sqrtm1 >> out
smallest quadratic non-residue prime: 3
$ echo ";" >> out
$ gp -q
? p=289*2^18502+1;
? readvec("out");
? [#digits(p),#digits(s)]
[5573, 5573]
? p-1==lift(Mod(s,p)^2)
1
? 
```

## Runtimes

On all 3 CPUs currently sqrtm1 runs at 100% (single core) only:
```
hermann@i7-11850H$ gp -q < <(echo "print1(289*2^18502+1)") | time ./sqrtm1 >> out
smallest quadratic non-residue prime: 3
23.70user 0.00system 0:23.63elapsed 100%CPU (0avgtext+0avgdata 6784maxresident)k
0inputs+16outputs (0major+478minor)pagefaults 0swaps
hermann@i7-11850H$ 
```

```
hermann@7600x:~/llr2/src/gwnum/paulunderwood$ gp -q < <(echo "print1(289*2^18502+1)") | time ./sqrtm1 >> out
smallest quadratic non-residue prime: 3
5.83user 0.00system 0:05.71elapsed 102%CPU (0avgtext+0avgdata 19584maxresident)k
0inputs+16outputs (0major+3429minor)pagefaults 0swaps
hermann@7600x:~/llr2/src/gwnum/paulunderwood$ 
```

```
hermann@7950x:~/llr2/src/gwnum/paulunderwood$ gp -q < <(echo "print1(289*2^18502+1)") | time ./sqrtm1 >> out
smallest quadratic non-residue prime: 3
5.72user 0.10system 0:05.46elapsed 106%CPU (0avgtext+0avgdata 18944maxresident)k
0inputs+8outputs (0major+3496minor)pagefaults 0swaps
hermann@7950x:~/llr2/src/gwnum/paulunderwood$ 
```

## prp

New prp.cc allows to test arbitrary numbers for PRP (which cannot be tested with eg. mprime or LLR).

Script doit4 proves that 490,001 decimal digit p+2 with p being palindrome prime is not PRP-2 and threfore not prime. Therefore p is not a twin prime in addition. This if the bigger of only 2 candidates for twin prime of 20 palindrom primes in t5k.org:  
https://gist.github.com/Hermann-SW/fbcfe5ea27aa0d36a1f118439154119b?permalink_comment_id=5043232#gistcomment-5043232

Computation was done with around 430% CPU of AMD 7950X CPU:  
```
hermann@7950x:~/llr2/src/gwnum/paulunderwood$ ./doit4
bytes=490001
smallest quadratic non-residue: 2
0
2977.68user 227.18system 12:26.35elapsed 429%CPU (0avgtext+0avgdata 32480maxresident)k
0inputs+0outputs (0major+34388minor)pagefaults 0swaps
hermann@7950x:~/llr2/src/gwnum/paulunderwood$
```

It is important for gwnum lib performance to fixate all threads onto chiplet0 of AMD 7950X CPU:  
```
$ cat doit4 
#!/bin/bash
gp -q < <(echo "print1(10^490000+3*(10^7383-1)/9*10^241309+3)") | time taskset -c 0-7,16-23 ./prp $1
$
```
