/* test_textio.c — write ASCII text lines, read back, visual comparison */
#include <stdio.h>
#include <conio.h>
#include <string.h>
#include "appledosio.h"
#include "testutil.h"

static char read_exact(dos_buffer_t *buf, char *dst, unsigned int len) { unsigned int i; char rc; for(i=0;i<len;i++){rc=dos_read_byte(buf,dst+i);if(rc!=DOS_ERR_OK)return rc;} return DOS_ERR_OK; }

static int t_pass, t_fail;

/* Apple II text: printable chars have bit 7 set, line end = 0x8D */
#define CR 0x8D
static char line1[] = {
    'H'|0x80,'E'|0x80,'L'|0x80,'L'|0x80,'O'|0x80,' '|0x80,
    'A'|0x80,'P'|0x80,'P'|0x80,'L'|0x80,'E'|0x80,' '|0x80,
    'I'|0x80,'I'|0x80, CR
};
static char line2[] = {
    'D'|0x80,'O'|0x80,'S'|0x80,' '|0x80,
    'T'|0x80,'E'|0x80,'X'|0x80,'T'|0x80,' '|0x80,
    'I'|0x80,'/'|0x80,'O'|0x80, CR
};
static char line3[] = {
    'T'|0x80,'E'|0x80,'S'|0x80,'T'|0x80,' '|0x80,
    'L'|0x80,'I'|0x80,'N'|0x80,'E'|0x80,' '|0x80,
    '3'|0x80, CR
};
#define LINE1_LEN (sizeof(line1))
#define LINE2_LEN (sizeof(line2))
#define LINE3_LEN (sizeof(line3))
#define TOTAL_LEN (LINE1_LEN + LINE2_LEN + LINE3_LEN)

int test_textio(void)
{
    dos_buffer_t *buf;
    char slot  = DOS_DEFAULT_SLOT;
    char drive = DOS_DEFAULT_DRIVE;
    char rbuf[TOTAL_LEN];
    char rc;
    unsigned int j;

    t_pass = 0; t_fail = 0;
    clrscr();
    printf("== 7/11 test_textio ==\n");

    buf = dos_get_buffer();
    CHECK("get_buffer", buf != 0);

    /* --- write 3 text lines --- */
    rc = dos_open(buf, slot, drive, DOS_DEFAULT_VOLUME, "TTEXTIO",
                  DOS_FILE_TYPE_TEXT, 0, DOS_OPEN_CREATE);
    CHECK("open for write",  rc == DOS_ERR_OK);
    rc = dos_write(buf, line1, LINE1_LEN);
    CHECK("write line1",     rc == DOS_ERR_OK);
    rc = dos_write(buf, line2, LINE2_LEN);
    CHECK("write line2",     rc == DOS_ERR_OK);
    rc = dos_write(buf, line3, LINE3_LEN);
    CHECK("write line3",     rc == DOS_ERR_OK);
    rc = dos_close(buf);
    CHECK("close write",     rc == DOS_ERR_OK);

    /* --- read back and check --- */
    rc = dos_open(buf, slot, drive, DOS_DEFAULT_VOLUME, "TTEXTIO",
                  DOS_FILE_TYPE_TEXT, 0, DOS_OPEN_NOCREATE);
    CHECK("open for read",   rc == DOS_ERR_OK);
    rc = read_exact(buf, rbuf, TOTAL_LEN);
    CHECK("read all lines",  rc == DOS_ERR_OK);
    rc = dos_close(buf);
    CHECK("close read",      rc == DOS_ERR_OK);

    {
        char expected[TOTAL_LEN];
        memcpy(expected,                         line1, LINE1_LEN);
        memcpy(expected+LINE1_LEN,               line2, LINE2_LEN);
        memcpy(expected+LINE1_LEN+LINE2_LEN,     line3, LINE3_LEN);
        CHECK_MEM("data roundtrip", rbuf, expected, TOTAL_LEN);
    }

    /* --- visual display: strip bit 7, 0x8D -> newline --- */
    printf("--- visual check ---\n");
    for (j = 0; j < TOTAL_LEN; j++) {
        if ((unsigned char)rbuf[j] == 0x8D) printf("\n");
        else                                putchar(rbuf[j] & 0x7F);
    }

    dos_delete(slot, drive, DOS_DEFAULT_VOLUME, "TTEXTIO");

    SUMMARY();
    PAUSE();
    return t_fail;
}
