#include <stdio.h>
#include "dump.h"

void dump_hex(const void *data, unsigned int size, char aascii)
{
    char ascii[17];
    size_t i, j;
    ascii[16] = '\0';
    for (i = 0; i < size; ++i)
    {
        char c = ((unsigned char *)data)[i];
        printf("%02X ", c);
        if (aascii)
        {
            c = c & 0x7F;
        }
        if (c >= ' ' && c <= '~')
        {
            ascii[i % 16] = c;
        }
        else
        {
            ascii[i % 16] = '.';
        }
        if ((i + 1) % 8 == 0 || i + 1 == size)
        {
            printf(" ");
            if ((i + 1) % 16 == 0)
            {
                printf("|  %s \n", ascii);
            }
            else if (i + 1 == size)
            {
                ascii[(i + 1) % 16] = '\0';
                if ((i + 1) % 16 <= 8)
                {
                    printf(" ");
                }
                for (j = (i + 1) % 16; j < 16; ++j)
                {
                    printf("   ");
                }
                printf("|  %s \n", ascii);
            }
        }
    }
}
