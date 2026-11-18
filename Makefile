DOS_VERSION=dos
#DOS_VERSION=images/dos34_2018_01
#DOS_VERSION=images/dos33_1983_08
#DOS_VERSION=images/diversidos_4.1c
DOS_VERSION=images/prontodos_1984_08
#DOS_VERSION=images/davidos_1984
APPLE_TYPE=apple2
CC=cl65
AS=ca65
ACMD=java -jar acmd.jar
LD=cl65
LDAS=ld65
CFLAGS=-Os -t $(APPLE_TYPE) 
LDFLAGS=-t $(APPLE_TYPE) 

all: test.bin minicat.bin disks

.PHONY: test
test: test.bin


disks: dosprog.do blank.do

blank.do: 
	$(ACMD) -dos140 blank.do

dosprog.do: test.bin minicat.bin \
        Makefile sample.bas textio.bas sample.txt
	rm -f dosprog.do
	cp $(DOS_VERSION).do dosprog.do
	$(ACMD) -as dosprog.do BOOT B < test.bin
	$(ACMD) -as dosprog.do TEST B < test.bin
	$(ACMD) -as dosprog.do MINICAT B < minicat.bin
	$(ACMD) -u dosprog.do TEST
	$(ACMD) -bas dosprog.do SAMPLE < sample.bas
	$(ACMD) -bas dosprog.do TEXTIO < textio.bas
	$(ACMD) -pt dosprog.do TEXT < sample.txt
	$(ACMD) -pt dosprog.do TODELETE < sample.txt
	$(ACMD) -u dosprog.do TODELETE 
	$(ACMD) -bas dosprog.do TORENAME < sample.bas
	$(ACMD) -u dosprog.do TORENAME 
	$(ACMD) -pt dosprog.do UNLOCKED < sample.txt
	$(ACMD) -pt dosprog.do LOCKED < sample.txt
	$(ACMD) -u dosprog.do LOCKED 
	$(ACMD) -l dosprog.do

minicat.bin: minicat.o appledosio.o dump.o
	$(LD) $(LDFLAGS) -o $@ -m $@.map $^

test.bin: test.o test0.o test1.o test2.o test3.o test4.o test5.o appledosio.o dump.o
	$(LD) $(LDFLAGS) -o $@ -m $@.map $^

dump.o: dump.c

minicat.o: minicat.c appledosio.h
test.o: test.c appledosio.h
test0.o: test0.c appledosio.h
test1.o: test1.c appledosio.h
test2.o: test2.c appledosio.h
test3.o: test3.c appledosio.h
test4.o: test4.c appledosio.h
test5.o: test5.c appledosio.h

appledosio.o: appledosio.c appledosio.h

clean:
	rm -f *.system *.bin *.prg *.o *.map blank.do dosprog.do *.[1-9]
