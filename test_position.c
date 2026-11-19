/* test_position.c — dos_write_pos, dos_read_pos, dos_position */
#include <stdio.h>
#include <conio.h>
#include <string.h>
#include "appledosio.h"
#include "testutil.h"

static int t_pass, t_fail;

int test_position(void)
{
    dos_buffer_t *buf;
    char slot  = DOS_DEFAULT_SLOT;
    char drive = DOS_DEFAULT_DRIVE;
    char rec0[10], rec1[10], rbuf[10];
    char rc;
    int i;

    t_pass = 0; t_fail = 0;
    clrscr();
    printf("== 8/11 test_position ==\n");

    buf = dos_get_buffer();
    CHECK("get_buffer",           buf != 0);

    for (i = 0; i < 10; i++) rec0[i] = ('A' + i) | 0x80;  /* A-J */
    for (i = 0; i < 10; i++) rec1[i] = ('K' + i) | 0x80;  /* K-T */

    /* write two records with write_pos at different offsets */
    rc = dos_open(buf, slot, drive, DOS_DEFAULT_VOLUME, "TPOS",
                  DOS_FILE_TYPE_BINARY, 0, DOS_OPEN_CREATE);
    CHECK("open for write",       rc == DOS_ERR_OK);
    rc = dos_write_pos(buf, rec0, 10, 0, 0);
    CHECK("write_pos offset 0",   rc == DOS_ERR_OK);
    rc = dos_write_pos(buf, rec1, 10, 0, 10);
    CHECK("write_pos offset 10",  rc == DOS_ERR_OK);
    rc = dos_close(buf);
    CHECK("close after write",    rc == DOS_ERR_OK);

    /* read each record back independently */
    rc = dos_open(buf, slot, drive, DOS_DEFAULT_VOLUME, "TPOS",
                  DOS_FILE_TYPE_BINARY, 0, DOS_OPEN_NOCREATE);
    CHECK("open for read",        rc == DOS_ERR_OK);
    rc = dos_read_pos(buf, rbuf, 10, 0, 0);
    CHECK("read_pos offset 0",    rc == DOS_ERR_OK);
    CHECK("rec0 data match",      memcmp(rec0, rbuf, 10) == 0);
    rc = dos_read_pos(buf, rbuf, 10, 0, 10);
    CHECK("read_pos offset 10",   rc == DOS_ERR_OK);
    CHECK("rec1 data match",      memcmp(rec1, rbuf, 10) == 0);
    rc = dos_close(buf);
    CHECK("close after read",     rc == DOS_ERR_OK);

    /* seek with dos_position then overwrite rec0 slot */
    rc = dos_open(buf, slot, drive, DOS_DEFAULT_VOLUME, "TPOS",
                  DOS_FILE_TYPE_BINARY, 0, DOS_OPEN_NOCREATE);
    CHECK("open for seek-write",  rc == DOS_ERR_OK);
    rc = dos_position(buf, 0, 0);
    CHECK("position to 0,0",      rc == DOS_ERR_OK);
    rc = dos_write(buf, rec1, 10);
    CHECK("write at position",    rc == DOS_ERR_OK);
    rc = dos_close(buf);
    CHECK("close seek-write",     rc == DOS_ERR_OK);

    rc = dos_open(buf, slot, drive, DOS_DEFAULT_VOLUME, "TPOS",
                  DOS_FILE_TYPE_BINARY, 0, DOS_OPEN_NOCREATE);
    CHECK("open verify",          rc == DOS_ERR_OK);
    rc = dos_read_pos(buf, rbuf, 10, 0, 0);
    CHECK("overwrite match",      rc == DOS_ERR_OK && memcmp(rec1, rbuf, 10) == 0);
    rc = dos_close(buf);
    CHECK("close verify",         rc == DOS_ERR_OK);

    dos_delete(slot, drive, DOS_DEFAULT_VOLUME, "TPOS");

    SUMMARY();
    PAUSE();
    return t_fail;
}
