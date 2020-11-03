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
    char slot = 6;
    char drive = 2;
    int i = 0;
    int rc = 0;

    clrscr();
    printf("--------> Test 3\n");
    buf = dos_get_buffer();
    printf("BUFFER=%p\n", buf);
    rc = dos_open(buf, slot, drive, "DELETED", DOS_FILE_TYPE_TEXT, 0, DOS_OPEN_CREATE);
    printf("OPEN RC=%X\n", rc);
    rc = dos_close(buf);
    printf("CLOSE RC=%X\n", rc);
    rc = dos_delete(slot, drive, "DELETED");
    printf("DELETE RC=%X\n", rc);
    rc = dos_open(buf, slot, drive, "TESTX", DOS_FILE_TYPE_TEXT, 0, DOS_OPEN_CREATE);
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
    rc = dos_rename(slot, drive, "TESTX", "TEST3");
    printf("RENAME RC=%X\n", rc);
    rc = dos_catalog(6, 2);
    printf("CATALOG RC=%X\n", rc);
    cgetc();
}
