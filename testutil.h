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

#endif /* _TESTUTIL_H_ */
