/* test_rwbyte.c — write_byte / read_byte data integrity */
#include <stdio.h>
#include <conio.h>
#include "appledosio.h"
#include "testutil.h"

static int t_pass, t_fail;

static char payload[] = {
    'H'|0x80, 'e'|0x80, 'l'|0x80, 'l'|0x80, 'o'|0x80, 0x8D
};
#define PAYLOAD_LEN 6

int test_rwbyte(void)
{
    dos_buffer_t *buf;
    char slot  = DOS_DEFAULT_SLOT;
    char drive = DOS_DEFAULT_DRIVE;
    char rc, b;
    int i, mismatches;

    t_pass = 0; t_fail = 0;
    clrscr();
    printf("== 5/11 test_rwbyte ==\n");

    buf = dos_get_buffer();
    CHECK("get_buffer",        buf != 0);

    rc = dos_open(buf, slot, drive, DOS_DEFAULT_VOLUME, "TRWBYTE",
                  DOS_FILE_TYPE_TEXT, 0, DOS_OPEN_CREATE);
    CHECK("open for write",    rc == DOS_ERR_OK);
    for (i = 0; i < PAYLOAD_LEN; i++)
        dos_write_byte(buf, payload[i]);
    rc = dos_close(buf);
    CHECK("close after write", rc == DOS_ERR_OK);

    rc = dos_open(buf, slot, drive, DOS_DEFAULT_VOLUME, "TRWBYTE",
                  DOS_FILE_TYPE_TEXT, 0, DOS_OPEN_NOCREATE);
    CHECK("open for read",     rc == DOS_ERR_OK);
    mismatches = 0;
    for (i = 0; i < PAYLOAD_LEN; i++) {
        rc = dos_read_byte(buf, &b);
        if (rc != DOS_ERR_OK || b != payload[i]) mismatches++;
    }
    CHECK("all bytes match",   mismatches == 0);
    rc = dos_close(buf);
    CHECK("close after read",  rc == DOS_ERR_OK);

    dos_delete(slot, drive, DOS_DEFAULT_VOLUME, "TRWBYTE");

    SUMMARY();
    PAUSE();
    return t_fail;
}
