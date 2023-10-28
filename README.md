LLR2 is a primality testing program for numbers of several specific forms. It uses Gwnum library by George Woltman, original LLR code by Jean Penné, hardware error check by Robert Gerbicz and test verification scheme by Pavel Atnashev with verifiable delay function by Krzysztof Pietrzak.

Main differences from the original LLR:
- Gerbicz check in Proth test.
- Gerbicz check in Fermat PRP test for b=2.
- Gerbicz check in Fermat PRP test for b!=2 using Sliding Window exponentiation method (limits performance penalty to 15-20%).
- Native APRCL test (no need to link additional libraries or have satellite programs).
- [Proth/PRP test verification scheme](https://www.mersenneforum.org/showthread.php?t=25323) with certification and certificate validation (fast double-check).
- [Pietrzak VDF](https://eprint.iacr.org/2018/627.pdf) for more efficient test verification (saves on bandwidth).
- Updated checkpoint file format with test fingerprint.
- Checkpoint write through file cache (on Windows).
- Minor changes and fixes.
 
 
## Changes needed
In order to compile under Ubuntu 22.04, from @rebirther in [this forum posting](https://www.mersenneforum.org/showthread.php?t=28681&p=641702]):  
* CFLAGS <kbd>... -std=c99 </kbd> to <kbd>... -std=gnu99 -D__EXTENSIONS</kbd>
* additional <kbd>#include <sys/types.h></kbd> in src/linux64/make64

## Why is llr2 important for AMD Zen4 CPUs?  
gwnum library 30.6 in LLR 4.0.5 does not work for numbers greater than 2^100000.  
gwnum library 30.11 used in llr2 does work on Zen4 CPUs:
```
hermann@7600x:~/llr2$ grep "ine GWNUM_VERSION" src/gwnum/gwnum.h 
#define GWNUM_VERSION		"30.11"
hermann@7600x:~/llr2$ 
```

## Build:  
```
hermann@7600x:~/llr2$ cd src/linux64/
hermann@7600x:~/llr2/src/linux64$ make 2>/dev/null
make -f make64
make[1]: Entering directory '/home/hermann/llr2/src/linux64'
gcc -I.. -I../gwnum -DX86_64 -DTESTLLR -O2 -std=gnu99 -D__EXTENSIONS -c ../lprime.c
make[1]: Leaving directory '/home/hermann/llr2/src/linux64'
gcc  -o llr2 lprime.o ../gwnum/linux64/gwnum.ld ../gwnum/linux64/gwnum.a ../gwnum/linux64/gwnum.ld -lm  -lpthread -lstdc++ -static
hermann@7600x:~/llr2/src/linux64$
```

## Sample run:  
```
hermann@7600x:~/llr2/src/linux64$ nohup ./llr2 -d -q"90825*2^90825+1"
nohup: ignoring input and appending output to 'nohup.out'
hermann@7600x:~/llr2/src/linux64$ sed "s/^M/\n/g" nohup.out
Starting Proth prime test of 90825*2^90825+1
Using all-complex AVX-512 FFT length 7K, a = 17, L2 = 86*66
90825*2^90825+1, bit: 10000 / 90824 [11.01%], 5676 checked.  Time per bit: 0.010 ms.
90825*2^90825+1, bit: 20000 / 90824 [22.02%], 17028 checked.  Time per bit: 0.009 ms.
90825*2^90825+1, bit: 30000 / 90824 [33.03%], 28380 checked.  Time per bit: 0.009 ms.
90825*2^90825+1, bit: 40000 / 90824 [44.04%], 39732 checked.  Time per bit: 0.009 ms.
90825*2^90825+1, bit: 50000 / 90824 [55.05%], 45408 checked.  Time per bit: 0.009 ms.
90825*2^90825+1, bit: 60000 / 90824 [66.06%], 56760 checked.  Time per bit: 0.009 ms.
90825*2^90825+1, bit: 70000 / 90824 [77.07%], 68112 checked.  Time per bit: 0.009 ms.
90825*2^90825+1, bit: 80000 / 90824 [88.08%], 79464 checked.  Time per bit: 0.009 ms.
90825*2^90825+1, bit: 90000 / 90824 [99.09%], 85140 checked.  Time per bit: 0.009 ms.
                                                                                                   
                                                                                                   
90825*2^90825+1 is prime! (27347 decimal digits)  Time : 862.905 ms.
hermann@7600x:~/llr2/src/linux64$ 
```
