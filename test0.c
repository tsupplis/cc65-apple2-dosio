#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <string.h>
#include <apple2.h>
#include "appledosio.h"
#include "dump.h"
#include <peekpoke.h>

void test0(void)
{
    printf("--------> Test 0\n");
    printf("DOS Check: %d\n",dos_check());
    if(dos_check()) {
        printf("DOS Version: %d\n",dos_version());
        printf("Basic Version: %d\n",dos_basic_version());
        printf("Max Files: %d\n",dos_maxfiles());
        printf("Default Max Files: %d\n",dos_default_maxfiles());
        printf("Active Slot: %d\n",dos_active_slot());
        printf("Active Drive: %d\n",dos_active_drive());
        printf("Last Current Slot: %d\n",dos_last_slot());
        printf("Last Current Drive: %d\n",dos_last_drive());
        printf("Default Slot: %d\n",dos_default_slot());
        printf("Default Drive: %d\n",dos_default_drive());
    }
    printf("Test 0 Completed ... \nPress a Key to Continue ...\n");
    cgetc();
}
