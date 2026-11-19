/* test_multifile.c — two files open simultaneously with separate buffers */
#include <stdio.h>
#include <conio.h>
#include <string.h>
#include "appledosio.h"
#include "testutil.h"

static char read_exact(dos_buffer_t *buf, char *dst, unsigned int len) { unsigned int i; char rc; for(i=0;i<len;i++){rc=dos_read_byte(buf,dst+i);if(rc!=DOS_ERR_OK)return rc;} return DOS_ERR_OK; }

static int t_pass, t_fail;

static char data1[8] = {
    'F'|0x80,'I'|0x80,'L'|0x80,'E'|0x80,
    '1'|0x80,'!'|0x80,'!'|0x80,'!'|0x80
};
static char data2[8] = {
    'F'|0x80,'I'|0x80,'L'|0x80,'E'|0x80,
    '2'|0x80,'?'|0x80,'?'|0x80,'?'|0x80
};
#define DLEN 8

int test_multifile(void)
{
    dos_buffer_t *buf1, *buf2;
    char slot  = DOS_DEFAULT_SLOT;
    char drive = DOS_DEFAULT_DRIVE;
    char r1[DLEN], r2[DLEN];
    char rc;

    t_pass = 0; t_fail = 0;
    clrscr();
    printf("== 11/11 test_multifile ==\n");

    buf1 = dos_get_buffer();
    CHECK("get_buffer 1", buf1 != 0);
    buf2 = dos_get_buffer();
    CHECK("get_buffer 2", buf2 != 0);

    /* --- create both files --- */
    rc = dos_open(buf1, slot, drive, DOS_DEFAULT_VOLUME, "TMULTI1",
                  DOS_FILE_TYPE_BINARY, 0, DOS_OPEN_CREATE);
    CHECK("open TMULTI1 write", rc == DOS_ERR_OK);
    rc = dos_open(buf2, slot, drive, DOS_DEFAULT_VOLUME, "TMULTI2",
                  DOS_FILE_TYPE_BINARY, 0, DOS_OPEN_CREATE);
    CHECK("open TMULTI2 write", rc == DOS_ERR_OK);

    /* --- write to both while both are open --- */
    rc = dos_write(buf1, data1, DLEN);
    CHECK("write TMULTI1",      rc == DOS_ERR_OK);
    rc = dos_write(buf2, data2, DLEN);
    CHECK("write TMULTI2",      rc == DOS_ERR_OK);

    rc = dos_close(buf1);
    CHECK("close TMULTI1",      rc == DOS_ERR_OK);
    rc = dos_close(buf2);
    CHECK("close TMULTI2",      rc == DOS_ERR_OK);

    /* --- reopen both and read back simultaneously --- */
    rc = dos_open(buf1, slot, drive, DOS_DEFAULT_VOLUME, "TMULTI1",
                  DOS_FILE_TYPE_BINARY, 0, DOS_OPEN_NOCREATE);
    CHECK("open TMULTI1 read",  rc == DOS_ERR_OK);
    rc = dos_open(buf2, slot, drive, DOS_DEFAULT_VOLUME, "TMULTI2",
                  DOS_FILE_TYPE_BINARY, 0, DOS_OPEN_NOCREATE);
    CHECK("open TMULTI2 read",  rc == DOS_ERR_OK);

    rc = read_exact(buf1, r1, DLEN);
    CHECK("read TMULTI1",       rc == DOS_ERR_OK);
    rc = read_exact(buf2, r2, DLEN);
    CHECK("read TMULTI2",       rc == DOS_ERR_OK);

    CHECK_MEM("TMULTI1 data match", r1, data1, DLEN);
    CHECK_MEM("TMULTI2 data match", r2, data2, DLEN);

    rc = dos_close(buf1);
    CHECK("close TMULTI1 read", rc == DOS_ERR_OK);
    rc = dos_close(buf2);
    CHECK("close TMULTI2 read", rc == DOS_ERR_OK);

    dos_delete(slot, drive, DOS_DEFAULT_VOLUME, "TMULTI1");
    dos_delete(slot, drive, DOS_DEFAULT_VOLUME, "TMULTI2");

    SUMMARY();
    PAUSE();
    return t_fail;
}
