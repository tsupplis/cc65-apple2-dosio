#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <string.h>
#include <apple2.h>
#include "appledosio.h"

int main()
{
    char slot = DOS_DEFAULT_SLOT;
    char drive = DOS_DEFAULT_DRIVE;
    int i = 0;
    char rc = 0;

    rc = dos_catalog(slot, drive, 0);
    return 0;
}
