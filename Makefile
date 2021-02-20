DOS_VERSION=dos
#DOS_VERSION=images/dos34_2018_01
#DOS_VERSION=images/dos33_1983_08
#DOS_VERSION=images/diversidos_4.1c
#DOS_VERSION=images/prontodos_1984_08
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


disks: dosprog.dsk blank.dsk

blank.dsk: 
	$(ACMD) -dos140 blank.dsk

dosprog.dsk: test.bin minicat.bin \
        Makefile sample.bas textio.bas sample.txt
	rm -f dosprog.dsk
	cp $(DOS_VERSION).dsk dosprog.dsk
	$(ACMD) -as dosprog.dsk BOOT B < test.bin
	$(ACMD) -as dosprog.dsk TEST B < test.bin
	$(ACMD) -as dosprog.dsk MINICAT B < minicat.bin
	$(ACMD) -u dosprog.dsk TEST
	$(ACMD) -bas dosprog.dsk SAMPLE < sample.bas
	$(ACMD) -bas dosprog.dsk TEXTIO < textio.bas
	$(ACMD) -pt dosprog.dsk TEXT < sample.txt
	$(ACMD) -l dosprog.dsk

minicat.bin: minicat.o appledosio.o dump.o
	$(LD) -o $@ -m $@.map $^ $(LDFLAGS)

test.bin: test.o test0.o test1.o test2.o test3.o appledosio.o dump.o
	$(LD) -o $@ -m $@.map $^ $(LDFLAGS)

dump.o: dump.c

minicat.o: minicat.c appledosio.h
test.o: test.c appledosio.h
test0.o: test0.c appledosio.h
test1.o: test1.c appledosio.h
test2.o: test2.c appledosio.h
test3.o: test3.c appledosio.h

appledosio.o: appledosio.c appledosio.h

clean:
	rm -f *.system *.bin *.prg *.o *.map blank.dsk dosprog.dsk *.[1-9]
