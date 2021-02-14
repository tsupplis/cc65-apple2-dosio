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
    printf("Last Current Slot: %d\n",dos_last_slot());
    printf("Last Current Drive: %d\n",dos_last_drive());
    cgetc();
}
