#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <string.h>
#include <apple2.h>
#include "appledosio.h"
#include "dump.h"

void test2(void)
{
    dos_buffer_t *buf;
    char content[5];
    char slot = 6;
    char drive = 2;
    int i = 0;
    int rc = 0;

    clrscr();
    printf("--------> Test 2\n");
    buf = dos_get_buffer();
    printf("BUFFER=%p\n", buf);
    rc = dos_open(buf, slot, drive, 0, "TEST2", DOS_FILE_TYPE_TEXT, 0, DOS_OPEN_CREATE);
    printf("OPEN RC=%X\n", rc);
    rc = dos_write_byte(buf, 'A' | 0x80);
    rc = dos_write_byte(buf, 'B' | 0x80);
    rc = dos_write_byte(buf, 'C' | 0x80);
    rc = dos_write_byte(buf, 0x8D);
    rc = dos_close(buf);
    printf("CLOSE RC=%X\n", rc);
    rc = dos_open(buf, slot, drive, 0, "TEST2", DOS_FILE_TYPE_TEXT, 0, DOS_OPEN_CREATE);
    printf("OPEN RC=%X\n", rc);
    rc = dos_read_pos(buf, content, 4, 0, 0);
    printf("READ RC=%X\n", rc);
    content[4] = 0;
    printf("CONTENT=[%X %X %X %X]\n", content[0], content[1], content[2], content[3]);
    rc = dos_close(buf);
    printf("CLOSE RC=%X\n", rc);
    rc = dos_verify(slot, drive, 0, "TEST2");
    printf("VERIFY RC=%X\n", rc);
    rc = dos_lock(slot, drive, 0, "TEST2");
    printf("LOCK RC=%X\n", rc);
    rc = dos_catalog(6, 2, 0);
    printf("CATALOG RC=%X\n", rc);
    cgetc();
}
