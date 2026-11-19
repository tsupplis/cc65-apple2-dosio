#include <stdio.h>
#include <conio.h>
#include <apple2.h>
#include "testutil.h"

char t_step = 0;

int test_dosinfo(void);   /*  1/11 */
int test_catalog(void);   /*  2/11 */
int test_lockfile(void);  /*  3/11 */
int test_create(void);    /*  4/11 */
int test_rwbyte(void);    /*  5/11 */
int test_rwblock(void);   /*  6/11 */
int test_textio(void);    /*  7/11 */
int test_position(void);  /*  8/11 */
int test_stress(void);    /*  9/11 */
int test_eod(void);       /* 10/11 */
int test_multifile(void); /* 11/11 */

int main()
{
    int total_pass = 0;
    int total_fail = 0;
    int f;
    int ch;

    clrscr();
#if defined(__APPLE2ENH__)
    videomode(VIDEOMODE_80COL);
#endif

    printf("Step by step? [y/N] ");
    ch = cgetc();
    printf("\n");
    t_step = (ch == 'y' || ch == 'Y') ? 1 : 0;

    f=test_dosinfo();   total_fail+=f; if(!f) total_pass++;
    f=test_catalog();   total_fail+=f; if(!f) total_pass++;
    f=test_lockfile();  total_fail+=f; if(!f) total_pass++;
    f=test_create();    total_fail+=f; if(!f) total_pass++;
    f=test_rwbyte();    total_fail+=f; if(!f) total_pass++;
    f=test_rwblock();   total_fail+=f; if(!f) total_pass++;
    f=test_textio();    total_fail+=f; if(!f) total_pass++;
    f=test_position();  total_fail+=f; if(!f) total_pass++;
    f=test_stress();    total_fail+=f; if(!f) total_pass++;
    f=test_eod();       total_fail+=f; if(!f) total_pass++;
    f=test_multifile(); total_fail+=f; if(!f) total_pass++;

    clrscr();
#if defined(__APPLE2ENH__)
    videomode(VIDEOMODE_80COL);
#endif
    printf("================================\n");
    printf("SUITE RESULTS\n");
    printf("  Passed : %d/11\n", total_pass);
    printf("  Failed : %d\n", total_fail);
    if (total_fail == 0)
        printf("  Status : ALL OK\n");
    else
        printf("  Status : %d ASSERTION(S) FAILED\n", total_fail);
    printf("================================\n");
    printf("Press a key to exit\n");
    cgetc();
    return 0;
}
