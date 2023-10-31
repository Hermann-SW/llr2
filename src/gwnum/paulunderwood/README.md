## Motivation

Initial code drop was done from this forum posting of [paulunderwood](https://www.mersenneforum.org/member.php?u=156):  
[https://www.mersenneforum.org/showthread.php?p=641952#post641946](https://www.mersenneforum.org/showthread.php?p=641952#post641946)  
It contained his gw_utility code, that allows for eg. <kbd>gw_powm ( r, b, e, n )</kbd> making use of superefficient gwnum library, to produce result of libgmp <kbd>powm( r, b, e, n )</kbd>.

"gw_powm()" will be used in "sqrtm1.c" code to compute "sqrt(-1) (mod p)", as done until now with Jean Penné's LLR software for computing that, eg. for largest known 11.9million digit non-Mersenne prime:  
[https://github.com/Hermann-SW/11887192-digit-prime](https://github.com/Hermann-SW/11887192-digit-prime)

## Build

```
bash sh_make
```

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
