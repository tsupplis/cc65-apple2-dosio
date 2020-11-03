#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <string.h>
#include <apple2.h>
#include "appledosio.h"
#include "dump.h"

void test1(void)
{
    dos_buffer_t *buf;
    char hello[] = {0x0f, 0x00, 0x0e, 0x08, 0x0a, 0x00, 0xba, 0x22, 0x48, 0x65, 0x6c, 0x6c, 0x6f, 0x22, 0x00, 0x00, 0x00, 0xa2};
    char slot = 6;
    char drive = 2;
    int i = 0;
    int rc = 0;

    clrscr();
    printf("--------> Test 1\n");
    printf("IS_DOS=%c\n", dos_check() ? 'Y' : 'N');
    printf("DOS_VERSION=%X\n", dos_version());
    buf = dos_get_buffer();
    printf("BUFFER=%p\n", buf);
    rc = dos_init(slot, 2, 254);
    printf("INIT(%d,%d) RC=%d\n", slot, drive, rc);
    rc = dos_open(buf, slot, drive, "HELLO", DOS_FILE_TYPE_APPLESOFT, 0, DOS_OPEN_CREATE);
    printf("OPEN RC=%X\n", rc);
    rc = dos_write_pos(buf, hello, sizeof(hello), 0, 0);
    printf("WRITE_POS RC=%X\n", rc);
    rc = dos_close(buf);
    printf("CLOSE RC=%X\n", rc);
    rc = dos_catalog(6, 2);
    printf("CATALOG RC=%X\n", rc);
    cgetc();
}
