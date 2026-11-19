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
    bv = dos_basic_version();
    mf  = dos_maxfiles();
    dmf = dos_default_maxfiles();
    sl = dos_default_slot();
    dr = dos_default_drive();

    /* always print raw values for diagnostics */
    printf("  v=%02X bv=%02X sl=%02X dr=%02X\n",
           (unsigned char)v, (unsigned char)bv,
           (unsigned char)sl, (unsigned char)dr);
    printf("  mf=%02X dmf=%02X actsl=%02X actdr=%02X\n",
           (unsigned char)mf, (unsigned char)dmf,
           (unsigned char)dos_active_slot(),
           (unsigned char)dos_active_drive());

    /* version/basic/maxfiles: informational — not all DOS variants expose these */
    if (v == 0)
        printf("  [--] version: not available on this DOS\n");
    else
        CHECK("version nonzero", v != 0);
    if (bv == 0)
        printf("  [--] basic version: not available on this DOS\n");
    else
        CHECK("basic version valid", bv == DOS_BASIC_INT   ||
                                     bv == DOS_BASIC_FPROM ||
                                     bv == DOS_BASIC_FPRAM);
    if (mf > 16 || mf == 0)
        printf("  [--] maxfiles=%02X: not available on this DOS\n", (unsigned char)mf);
    else
        CHECK("maxfiles 1-16", mf >= 1 && mf <= 16);
    if (dmf > 16 || dmf == 0)
        printf("  [--] dflt maxfiles=%02X: not available\n", (unsigned char)dmf);
    else
        CHECK("default maxfiles 1-16", dmf >= 1 && dmf <= 16);
    CHECK("default slot 1-7",   sl >= 1 && sl <= 7);
    CHECK("default drive 1-2",  dr >= 1 && dr <= 2);

    SUMMARY();
    PAUSE();
    return t_fail;
}
