#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <string.h>
#include <apple2.h>

void test0(void);
void test1(void);
void test2(void);
void test3(void);

int main()
{
    clrscr();
#if defined(__APPLE2ENH__)
    videomode(VIDEOMODE_80COL);
#endif
    test0();
    test1();
    test2();
    test3();
    printf("END\n");
    return 0;
}
