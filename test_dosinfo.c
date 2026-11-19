/* test_dosinfo.c — verify DOS is present and reports sane metadata */
#include <stdio.h>
#include <conio.h>
#include "appledosio.h"
#include "testutil.h"

static int t_pass, t_fail;

int test_dosinfo(void)
{
    char v, bv, mf, dmf, sl, dr;

    t_pass = 0; t_fail = 0;
    clrscr();
    printf("== 1/11 test_dosinfo ==\n");

    CHECK("dos_check",           dos_check() != 0);
    v  = dos_version();
    CHECK("version nonzero",     v != 0);
    bv = dos_basic_version();
    CHECK("basic version valid", bv == DOS_BASIC_INT   ||
                                 bv == DOS_BASIC_FPROM ||
                                 bv == DOS_BASIC_FPRAM);
    mf  = dos_maxfiles();
    dmf = dos_default_maxfiles();
    CHECK("maxfiles >= 1",         mf  >= 1);
    CHECK("default maxfiles >= 1", dmf >= 1);
    sl = dos_default_slot();
    dr = dos_default_drive();
    CHECK("default slot 1-7",   sl >= 1 && sl <= 7);
    CHECK("default drive 1-2",  dr >= 1 && dr <= 2);
    printf("  DOS v%d  BASIC v%d  slot%d drv%d  maxfiles=%d\n",
           v, bv, sl, dr, mf);

    SUMMARY();
    PAUSE();
    return t_fail;
}
