#ifndef _TESTUTIL_H_
#define _TESTUTIL_H_

/* Simple pass/fail reporting for Apple II test suite.
   t_pass / t_fail  — declared static in each test file.
   t_step           — global flag set by test.c:
                        1 = pause after each test (step-by-step)
                        0 = run all tests without pausing
*/

extern char t_step;

/* t_pass and t_fail are declared static in each test file */
#define CHECK(label, expr) \
    do { \
        if (expr) { t_pass++; printf("[OK] " label "\n"); } \
        else      { t_fail++; printf("[!!] " label "\n"); } \
    } while(0)

#define SUMMARY() \
    printf("--- %d/%d passed", t_pass, t_pass+t_fail); \
    if (t_fail == 0) printf(" OK\n"); else printf(" FAIL\n")

#define PAUSE() \
    do { if (t_step) { printf("Press a key\n"); cgetc(); } } while(0)

/* Print up to 16 bytes of a buffer as hex, prefixed with label */
#define HEXDUMP(label, buf, len) \
    do { \
        unsigned int _i; unsigned int _n = (len) > 16 ? 16 : (len); \
        printf("  " label ":"); \
        for (_i = 0; _i < _n; _i++) printf(" %02X", (unsigned char)(buf)[_i]); \
        printf("\n"); \
    } while(0)

/* CHECK with hex dump of got/exp on failure */
#define CHECK_MEM(label, got, exp, len) \
    do { \
        if (memcmp((got),(exp),(len)) == 0) { \
            t_pass++; printf("[OK] " label "\n"); \
        } else { \
            t_fail++; printf("[!!] " label "\n"); \
            HEXDUMP("exp", (exp), (len)); \
            HEXDUMP("got", (got), (len)); \
        } \
    } while(0)

#endif /* _TESTUTIL_H_ */
