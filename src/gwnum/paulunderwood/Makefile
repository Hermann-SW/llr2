CFLAGS = -I.. -O3
CCFLAGS = -I.. -O3
LDFLAGS = -lgmp -L../linux64 -l:gwnum.a -lm -lpthread
LDDFLAGS = -lgmp -lgmpxx -L../linux64 -l:gwnum.a -lm -lpthread

all: prg.o gw_utility.o sqrtm1.o
	gcc -o prg prg.o gw_utility.o $(LDFLAGS)
	g++ -o sqrtm1 sqrtm1.o gw_utility.o $(LDDFLAGS)

prg.o: prg.c 
	gcc -c $< $(CFLAGS)

sqrtm1.o: sqrtm1.cc 
	g++ -c $< $(CFLAGS)


cpplint: sqrtm1.cc
	cpplint --filter=-legal/copyright $<

cppcheck: sqrtm1.cc
	cppcheck --enable=all --suppress=missingIncludeSystem $< --check-config

clean:
	rm -f prg sqrtm1 *.o
