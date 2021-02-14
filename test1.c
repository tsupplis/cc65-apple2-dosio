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
    char slot = DOS_DEFAULT_SLOT;
    char drive = DOS_DEFAULT_DRIVE;
    char drive2;
    int i = 0;
    char rc = 0;

    clrscr();
    printf("--------> Test 1\n");
    buf = dos_get_buffer();
    printf("BUFFER=%p\n", buf);
    drive2=dos_default_drive();
    if(drive2==1) {
        printf("Do you want to Init Drive 2 (Y/N) [N]\n");
        i=cgetc();
        if(i=='Y' || i=='y') {
            rc = dos_init(slot, 2, 0);
            printf("INIT(%d,%d) RC=%d\n", slot, 2, rc);
        }
    }  else {
        printf("Do you want to Init Drive 1 (Y/N) [N]\n");
        i=cgetc();
        if(i=='Y' || i=='y') {
            rc = dos_init(slot, 1, 0);
            printf("INIT(%d,%d) RC=%d\n", slot, 1, rc);
        }
    } 
    rc = dos_open(buf, slot, drive, 0, "TEST1", DOS_FILE_TYPE_APPLESOFT, 0, 
        DOS_OPEN_CREATE);
    printf("OPEN RC=%X\n", rc);
    rc = dos_write_pos(buf, hello, sizeof(hello), 0, 0);
    printf("WRITE_POS RC=%X\n", rc);
    rc = dos_close(buf);
    printf("CLOSE RC=%X\n", rc);
    rc = dos_catalog(7, 2, 2);
    printf("CATALOG RC=%X\n", rc);
    rc = dos_catalog(slot, drive, 0);
    printf("CATALOG RC=%X\n", rc);
    printf("Test 1 Completed ... \nPress a Key to Continue ...\n");
    cgetc();
}
