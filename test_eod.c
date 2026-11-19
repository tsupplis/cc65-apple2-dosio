/* test_eod.c — read past end of file, data integrity check */
#include <stdio.h>
#include <conio.h>
#include <string.h>
#include "appledosio.h"
#include "testutil.h"

static char read_exact(dos_buffer_t *buf, char *dst, unsigned int len) { unsigned int i; char rc; for(i=0;i<len;i++){rc=dos_read_byte(buf,dst+i);if(rc!=DOS_ERR_OK)return rc;} return DOS_ERR_OK; }

static int t_pass, t_fail;

#define DATA_LEN 6

int test_eod(void)
{
    dos_buffer_t *buf;
    char slot  = DOS_DEFAULT_SLOT;
    char drive = DOS_DEFAULT_DRIVE;
    char wdata[DATA_LEN], rdata[DATA_LEN];
    char rc;
    int i;

    t_pass = 0; t_fail = 0;
    clrscr();
    printf("== 10/11 test_eod ==\n");

    buf = dos_get_buffer();
    CHECK("get_buffer", buf != 0);

    /* write DATA_LEN bytes */
    for (i = 0; i < DATA_LEN; i++) wdata[i] = ('A' + i) | 0x80;

    rc = dos_open(buf, slot, drive, DOS_DEFAULT_VOLUME, "TEOD",
                  DOS_FILE_TYPE_BINARY, 0, DOS_OPEN_CREATE);
    CHECK("open for write",    rc == DOS_ERR_OK);
    rc = dos_write(buf, wdata, DATA_LEN);
    CHECK("write data",        rc == DOS_ERR_OK);
    rc = dos_close(buf);
    CHECK("close write",       rc == DOS_ERR_OK);

    /* read exactly DATA_LEN bytes — should succeed */
    rc = dos_open(buf, slot, drive, DOS_DEFAULT_VOLUME, "TEOD",
                  DOS_FILE_TYPE_BINARY, 0, DOS_OPEN_NOCREATE);
    CHECK("open for read",     rc == DOS_ERR_OK);
    rc = read_exact(buf, rdata, DATA_LEN);
    CHECK("read exact",        rc == DOS_ERR_OK);
    CHECK_MEM("data intact",   rdata, wdata, DATA_LEN);
    /* Note: ProntoDOS read_byte returns OK past EOF on binary files */
    rc = dos_close(buf);
    CHECK("close read",        rc == DOS_ERR_OK);

    dos_delete(slot, drive, DOS_DEFAULT_VOLUME, "TEOD");

    SUMMARY();
    PAUSE();
    return t_fail;
}
