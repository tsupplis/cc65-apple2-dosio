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
    char hello[] = {0x0f, 0x00, 0x0e, 0x08, 0x0a, 0x00, 0xba, 0x22, 0x48, 0x65, 0x6c, 0x6c, 0x6f, 0x22, 0x00, 0x00, 0x00, 0xa2};
    char slot = DOS_DEFAULT_SLOT;
    char drive = DOS_DEFAULT_DRIVE;
    int i = 0;
    char rc = 0;

    clrscr();
    printf("--------> Test 2\n");
    buf = dos_get_buffer();
    printf("BUFFER=%p\n", buf);
    rc = dos_open(buf, slot, drive, 0, "TEST2", DOS_FILE_TYPE_APPLESOFT, 0, 
        DOS_OPEN_CREATE);
    printf("OPEN RC=%X\n", rc);
    for(i=0;i<sizeof(hello);i++) {
        rc = dos_write_byte(buf, hello[i]);
        printf("WRITE_BYTE RC=%X\n", rc);
    }
    //rc = dos_write_pos(buf, hello, sizeof(hello), 0, 0);
    //printf("WRITE_POS RC=%X\n", rc);
    rc = dos_close(buf);
    printf("CLOSE RC=%X\n", rc);
    rc = dos_catalog(slot, drive, 0);
    printf("CATALOG RC=%X\n", rc);
    printf("Test 2 Completed ... \nPress a Key to Continue ...\n");
    cgetc();
}
