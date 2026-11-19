/* test_lockfile.c — lock, unlock, write-to-locked detection */
#include <stdio.h>
#include <conio.h>
#include "appledosio.h"
#include "testutil.h"

static int t_pass, t_fail;

int test_lockfile(void)
{
    dos_buffer_t *buf;
    char slot  = DOS_DEFAULT_SLOT;
    char drive = DOS_DEFAULT_DRIVE;
    char rc;

    t_pass = 0; t_fail = 0;
    clrscr();
    printf("== 3/11 test_lockfile ==\n");

    buf = dos_get_buffer();
    CHECK("get_buffer",          buf != 0);

    rc = dos_open(buf, slot, drive, DOS_DEFAULT_VOLUME, "TLOCK",
                  DOS_FILE_TYPE_TEXT, 0, DOS_OPEN_CREATE);
    CHECK("create TLOCK",        rc == DOS_ERR_OK);
    dos_write_byte(buf, 'A' | 0x80);
    rc = dos_close(buf);
    CHECK("close TLOCK",         rc == DOS_ERR_OK);

    rc = dos_lock(slot, drive, DOS_DEFAULT_VOLUME, "TLOCK");
    CHECK("lock TLOCK",          rc == DOS_ERR_OK);
    rc = dos_verify(slot, drive, DOS_DEFAULT_VOLUME, "TLOCK");
    CHECK("verify locked exists", rc == DOS_ERR_OK);

    /* ProntoDOS allows open of locked file; write must fail */
    rc = dos_open(buf, slot, drive, DOS_DEFAULT_VOLUME, "TLOCK",
                  DOS_FILE_TYPE_TEXT, 0, DOS_OPEN_NOCREATE);
    CHECK("open locked ok",      rc == DOS_ERR_OK);
    rc = dos_write_byte(buf, 'Z' | 0x80);
    CHECK("write locked->err",   rc != DOS_ERR_OK);
    dos_close(buf);

    rc = dos_unlock(slot, drive, DOS_DEFAULT_VOLUME, "TLOCK");
    CHECK("unlock TLOCK",        rc == DOS_ERR_OK);

    rc = dos_open(buf, slot, drive, DOS_DEFAULT_VOLUME, "TLOCK",
                  DOS_FILE_TYPE_TEXT, 0, DOS_OPEN_NOCREATE);
    CHECK("open after unlock",   rc == DOS_ERR_OK);
    rc = dos_write_byte(buf, 'B' | 0x80);
    CHECK("write after unlock",  rc == DOS_ERR_OK);
    rc = dos_close(buf);
    CHECK("close after unlock",  rc == DOS_ERR_OK);

    dos_delete(slot, drive, DOS_DEFAULT_VOLUME, "TLOCK");
    CHECK("delete TLOCK",        rc == DOS_ERR_OK);

    SUMMARY();
    PAUSE();
    return t_fail;
}
