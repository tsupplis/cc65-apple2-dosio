/* test_catalog.c — verify, rename, delete */
#include <stdio.h>
#include <conio.h>
#include "appledosio.h"
#include "testutil.h"

static int t_pass, t_fail;

static char create_scratch(dos_buffer_t *buf, char slot, char drive, char *name)
{
    char rc;
    rc = dos_open(buf, slot, drive, DOS_DEFAULT_VOLUME, name,
                  DOS_FILE_TYPE_TEXT, 0, DOS_OPEN_CREATE);
    if (rc != DOS_ERR_OK) return rc;
    dos_write_byte(buf, 'X' | 0x80);
    return dos_close(buf);
}

int test_catalog(void)
{
    dos_buffer_t *buf;
    char slot  = DOS_DEFAULT_SLOT;
    char drive = DOS_DEFAULT_DRIVE;
    char rc;

    t_pass = 0; t_fail = 0;
    clrscr();
    printf("== 2/11 test_catalog ==\n");

    buf = dos_get_buffer();
    CHECK("get_buffer",          buf != 0);
    rc = create_scratch(buf, slot, drive, "TCAT1");
    CHECK("create TCAT1",        rc == DOS_ERR_OK);
    rc = dos_verify(slot, drive, DOS_DEFAULT_VOLUME, "TCAT1");
    CHECK("verify TCAT1 exists", rc == DOS_ERR_OK);
    rc = dos_rename(slot, drive, DOS_DEFAULT_VOLUME, "TCAT1", "TCAT2");
    CHECK("rename->TCAT2",       rc == DOS_ERR_OK);
    rc = dos_verify(slot, drive, DOS_DEFAULT_VOLUME, "TCAT2");
    CHECK("verify TCAT2 exists", rc == DOS_ERR_OK);
    rc = dos_verify(slot, drive, DOS_DEFAULT_VOLUME, "TCAT1");
    CHECK("verify TCAT1 gone",   rc != DOS_ERR_OK);
    rc = dos_delete(slot, drive, DOS_DEFAULT_VOLUME, "TCAT2");
    CHECK("delete TCAT2",        rc == DOS_ERR_OK);
    rc = dos_delete(slot, drive, DOS_DEFAULT_VOLUME, "NOFILE");
    CHECK("delete missing->err", rc != DOS_ERR_OK);

    SUMMARY();
    PAUSE();
    return t_fail;
}
