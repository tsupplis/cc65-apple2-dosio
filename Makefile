#DOS_VERSION=images/dos34_2018_01
#DOS_VERSION=images/dos33_1983_08
#DOS_VERSION=images/diversidos_4.1c
DOS_VERSION=images/prontodos_1984_08
APPLE_TYPE=apple2
CC=cl65
AS=ca65
ACMD=java -jar acmd.jar
LD=cl65
LDAS=ld65
CFLAGS=-Os -t $(APPLE_TYPE) 
LDFLAGS=-t $(APPLE_TYPE) 

all: dosprog.dsk blank.dsk

blank.dsk: 
	$(ACMD) -dos140 blank.dsk

dosprog.dsk: test.bin \
        Makefile sample.bas textio.bas sample.txt
	rm -f dosprog.dsk
	cp $(DOS_VERSION).dsk dosprog.dsk
	$(ACMD) -as dosprog.dsk BOOT B < test.bin
	$(ACMD) -as dosprog.dsk TEST B < test.bin
	$(ACMD) -bas dosprog.dsk SAMPLE < sample.bas
	$(ACMD) -bas dosprog.dsk TEXTIO < textio.bas
	$(ACMD) -pt dosprog.dsk TEXT < sample.txt
	$(ACMD) -l dosprog.dsk

test.bin: test.o test1.o test2.o test3.o appledosio.o dump.o
	$(LD) -o $@ -m $@.map $^ $(LDFLAGS)

dump.o: dump.c

test.o: test.c appledosio.h
test1.o: test1.c appledosio.h
test2.o: test2.c appledosio.h
test3.o: test3.c appledosio.h

appledosio.o: appledosio.c appledosio.h

clean:
	rm -f *.system *.bin *.prg *.o *.map *.dsk *.[1-9]
