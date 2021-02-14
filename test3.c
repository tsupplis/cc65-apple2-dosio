#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <string.h>
#include <apple2.h>
#include "appledosio.h"
#include "dump.h"

void test3(void)
{
    dos_buffer_t *buf;
    char hello[] = {0x0f, 0x00, 0x0e, 0x08, 0x0a, 0x00, 0xba, 0x22, 0x48, 0x65, 0x6c, 0x6c, 0x6f, 0x22, 0x00, 0x00, 0x00, 0xa2};
    char slot = DOS_DEFAULT_SLOT;
    char drive = DOS_DEFAULT_DRIVE;
    int i = 0;
    char rc = 0;

    clrscr();
    printf("--------> Test 3\n");
    buf = dos_get_buffer();
    printf("BUFFER=%p\n", buf);
    rc = dos_open(buf, slot, drive, 0, "DELETED", DOS_FILE_TYPE_TEXT, 0, DOS_OPEN_CREATE);
    printf("OPEN RC=%X\n", rc);
    rc = dos_close(buf);
    printf("CLOSE RC=%X\n", rc);
    rc = dos_delete(slot, drive, 0, "DELETED");
    printf("DELETE RC=%X\n", rc);
    rc = dos_open(buf, slot, drive, 0, "TESTX", DOS_FILE_TYPE_TEXT, 0, DOS_OPEN_CREATE);
    printf("OPEN RC=%X\n", rc);
    rc = dos_position(buf, 0, 0);
    printf("POSITION RC=%X\n", rc);
    rc = dos_write(buf, "BOBON", 4);
    printf("WRITE(BOBO) RC=%X\n", rc);
    rc = dos_write(buf, "BABAN", 4);
    printf("WRITE(BABA) RC=%X\n", rc);
    rc = dos_write_pos(buf, "BIX", 2, 0, 4);
    printf("WRITE_POS(BI) RC=%X\n", rc);
    rc = dos_position(buf, 0, 0);
    printf("POSITION RC=%X\n", rc);
    rc = dos_write(buf, "BIX", 2);
    printf("WRITE(BI) RC=%X\n", rc);
    rc = dos_close(buf);
    printf("CLOSE RC=%X\n", rc);
    rc = dos_rename(slot, drive, 0, "TESTX", "TEST3");
    printf("RENAME RC=%X\n", rc);
    rc = dos_catalog(slot, drive, 0);
    printf("CATALOG RC=%X\n", rc);
    printf("Test 3 Completed ... \nPress a Key to Continue ...\n");
    cgetc();
}
