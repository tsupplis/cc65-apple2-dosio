/* test_create.c — DOS_OPEN_CREATE vs DOS_OPEN_NOCREATE behaviour */
#include <stdio.h>
#include <conio.h>
#include <string.h>
#include "appledosio.h"
#include "testutil.h"

static int t_pass, t_fail;

int test_create(void)
{
    dos_buffer_t *buf;
    char slot  = DOS_DEFAULT_SLOT;
    char drive = DOS_DEFAULT_DRIVE;
    char wdata[4] = {'A'|0x80,'B'|0x80,'C'|0x80, 0x8D};
    char wdata2[4]= {'X'|0x80,'Y'|0x80,'Z'|0x80, 0x8D};
    char rdata[4];
    char rc;

    t_pass = 0; t_fail = 0;
    clrscr();
    printf("== 4/11 test_create ==\n");

    buf = dos_get_buffer();
    CHECK("get_buffer", buf != 0);

    /* make sure file doesn't exist from a previous run */
    dos_delete(slot, drive, DOS_DEFAULT_VOLUME, "TCREAT");

    /* --- NOCREATE on missing file must fail --- */
    rc = dos_open(buf, slot, drive, DOS_DEFAULT_VOLUME, "TCREAT",
                  DOS_FILE_TYPE_TEXT, 0, DOS_OPEN_NOCREATE);
    CHECK("nocreate missing->err", rc != DOS_ERR_OK);

    /* --- CREATE when file does not exist --- */
    rc = dos_open(buf, slot, drive, DOS_DEFAULT_VOLUME, "TCREAT",
                  DOS_FILE_TYPE_TEXT, 0, DOS_OPEN_CREATE);
    CHECK("create new file",       rc == DOS_ERR_OK);
    rc = dos_write(buf, wdata, sizeof(wdata));
    CHECK("write to new file",     rc == DOS_ERR_OK);
    rc = dos_close(buf);
    CHECK("close new file",        rc == DOS_ERR_OK);
    rc = dos_verify(slot, drive, DOS_DEFAULT_VOLUME, "TCREAT");
    CHECK("verify new file",       rc == DOS_ERR_OK);

    /* --- CREATE when file already exists (overwrite) --- */
    rc = dos_open(buf, slot, drive, DOS_DEFAULT_VOLUME, "TCREAT",
                  DOS_FILE_TYPE_TEXT, 0, DOS_OPEN_CREATE);
    CHECK("create overwrite",      rc == DOS_ERR_OK);
    rc = dos_write(buf, wdata2, sizeof(wdata2));
    CHECK("write overwrite data",  rc == DOS_ERR_OK);
    rc = dos_close(buf);
    CHECK("close overwrite",       rc == DOS_ERR_OK);

    /* --- read back and confirm overwrite data, not original --- */
    rc = dos_open(buf, slot, drive, DOS_DEFAULT_VOLUME, "TCREAT",
                  DOS_FILE_TYPE_TEXT, 0, DOS_OPEN_NOCREATE);
    CHECK("open after overwrite",  rc == DOS_ERR_OK);
    rc = dos_read(buf, rdata, sizeof(rdata));
    CHECK("read overwrite",        rc == DOS_ERR_OK);
    CHECK("overwrite data match",  memcmp(rdata, wdata2, sizeof(wdata2)) == 0);
    rc = dos_close(buf);
    CHECK("close read",            rc == DOS_ERR_OK);

    dos_delete(slot, drive, DOS_DEFAULT_VOLUME, "TCREAT");

    SUMMARY();
    PAUSE();
    return t_fail;
}
