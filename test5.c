#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <string.h>
#include <apple2.h>
#include "appledosio.h"
#include "dump.h"

void test5(void)
{
    char slot = DOS_DEFAULT_SLOT;
    char drive = DOS_DEFAULT_DRIVE;
    char drive2;
    int i = 0;
    char rc = 0;

    clrscr();
    printf("--------> Test 5\n");
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
    printf("Test 5 Completed ... \nPress a Key to Continue ...\n");
    cgetc();
}
