/* test_init.c — dos_init (destructive: asks before running) */
#include <stdio.h>
#include <conio.h>
#include "appledosio.h"
#include "testutil.h"

static int t_pass, t_fail;

int test_init(void)
{
    char slot  = DOS_DEFAULT_SLOT;
    char drive = DOS_DEFAULT_DRIVE;
    char drive2;
    char rc;
    int  ch;

    t_pass = 0; t_fail = 0;
    clrscr();
    printf("== 10/10 test_init ==\n");
    printf("WARNING: formats the OTHER drive.\n");

    /* Only ask in step-by-step mode; skip silently in automated run */
    if (!t_step) {
        printf("  skipped (auto mode)\n");
        return 0;
    }

    printf("Run this test? [y/N] ");
    ch = cgetc();
    printf("\n");

    if (ch != 'y' && ch != 'Y') {
        printf("  skipped\n");
        PAUSE();
        return 0;
    }

    drive2 = dos_default_drive();
    /* format whichever drive is NOT the boot drive */
    if (drive2 == 1) drive2 = 2; else drive2 = 1;

    printf("Formatting slot%d drive%d ...\n", slot, drive2);
    rc = dos_init(slot, drive2, DOS_DEFAULT_VOLUME);
    CHECK("dos_init ok", rc == DOS_ERR_OK);

    SUMMARY();
    PAUSE();
    return t_fail;
}
