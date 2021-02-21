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
    int i = 0;
    char rc = 0;

    clrscr();
    printf("--------> Test 1\n");
    buf = dos_get_buffer();
    printf("BUFFER=%p\n", buf);
    rc = dos_verify(slot, drive, 0, "TODELETE");
    printf("VERIFY RC=%X\n", rc);
    rc = dos_delete(slot, drive, 0, "TODELETE");
    printf("DELETE RC=%X\n", rc);
    //rc = dos_rename(slot, drive, 0, "TORENAME", "RENAMED");
    //printf("RENAME RC=%X\n", rc);
    rc = dos_lock(slot, drive, 0, "LOCKED");
    printf("LOCK RC=%X\n", rc);
    rc = dos_unlock(slot, drive, 0, "UNLOCKED");
    printf("UNLOCK RC=%X\n", rc);
    rc = dos_catalog(slot, drive, 0);
    printf("CATALOG RC=%X\n", rc);
    printf("Test 1 Completed ... \nPress a Key to Continue ...\n");
    cgetc();
}
