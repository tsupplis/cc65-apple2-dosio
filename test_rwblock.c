/* test_rwblock.c — dos_write / dos_read block data integrity */
#include <stdio.h>
#include <conio.h>
#include <string.h>
#include "appledosio.h"
#include "testutil.h"

static char read_exact(dos_buffer_t *buf, char *dst, unsigned int len) { unsigned int i; char rc; for(i=0;i<len;i++){rc=dos_read_byte(buf,dst+i);if(rc!=DOS_ERR_OK)return rc;} return DOS_ERR_OK; }

static int t_pass, t_fail;

static char wbuf[16] = {
    'A'|0x80,'B'|0x80,'C'|0x80,'D'|0x80,
    'E'|0x80,'F'|0x80,'G'|0x80,'H'|0x80,
    'I'|0x80,'J'|0x80,'K'|0x80,'L'|0x80,
    'M'|0x80,'N'|0x80,'O'|0x80, 0x8D
};
#define WBUF_LEN 16

int test_rwblock(void)
{
    dos_buffer_t *buf;
    char slot  = DOS_DEFAULT_SLOT;
    char drive = DOS_DEFAULT_DRIVE;
    char rbuf[WBUF_LEN];
    char rc;

    t_pass = 0; t_fail = 0;
    clrscr();
    printf("== 6/11 test_rwblock ==\n");

    buf = dos_get_buffer();
    CHECK("get_buffer",            buf != 0);

    /* single block write then read */
    rc = dos_open(buf, slot, drive, DOS_DEFAULT_VOLUME, "TRWBLK",
                  DOS_FILE_TYPE_BINARY, 0, DOS_OPEN_CREATE);
    CHECK("open for write",        rc == DOS_ERR_OK);
    rc = dos_write(buf, wbuf, WBUF_LEN);
    CHECK("write block",           rc == DOS_ERR_OK);
    rc = dos_close(buf);
    CHECK("close after write",     rc == DOS_ERR_OK);

    rc = dos_open(buf, slot, drive, DOS_DEFAULT_VOLUME, "TRWBLK",
                  DOS_FILE_TYPE_BINARY, 0, DOS_OPEN_NOCREATE);
    CHECK("open for read",         rc == DOS_ERR_OK);
    rc = read_exact(buf, rbuf, WBUF_LEN);
    CHECK("read block",            rc == DOS_ERR_OK);
    CHECK_MEM("block data match",  rbuf, wbuf, WBUF_LEN);
    rc = dos_close(buf);
    CHECK("close after read",      rc == DOS_ERR_OK);

    /* two sequential writes read back as one */
    rc = dos_open(buf, slot, drive, DOS_DEFAULT_VOLUME, "TRWBLK2",
                  DOS_FILE_TYPE_BINARY, 0, DOS_OPEN_CREATE);
    CHECK("open TRWBLK2",          rc == DOS_ERR_OK);
    rc = dos_write(buf, wbuf,      8);
    CHECK("write first half",      rc == DOS_ERR_OK);
    rc = dos_write(buf, wbuf + 8,  8);
    CHECK("write second half",     rc == DOS_ERR_OK);
    rc = dos_close(buf);
    CHECK("close TRWBLK2",         rc == DOS_ERR_OK);

    rc = dos_open(buf, slot, drive, DOS_DEFAULT_VOLUME, "TRWBLK2",
                  DOS_FILE_TYPE_BINARY, 0, DOS_OPEN_NOCREATE);
    CHECK("open TRWBLK2 read",     rc == DOS_ERR_OK);
    rc = read_exact(buf, rbuf, WBUF_LEN);
    CHECK("read joined block",     rc == DOS_ERR_OK);
    CHECK_MEM("joined data match", rbuf, wbuf, WBUF_LEN);
    rc = dos_close(buf);
    CHECK("close TRWBLK2 read",    rc == DOS_ERR_OK);

    dos_delete(slot, drive, DOS_DEFAULT_VOLUME, "TRWBLK");
    dos_delete(slot, drive, DOS_DEFAULT_VOLUME, "TRWBLK2");

    SUMMARY();
    PAUSE();
    return t_fail;
}
