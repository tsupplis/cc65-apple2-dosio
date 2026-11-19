/* test_stress.c — repeated write/read, data integrity across 100 passes */
#include <stdio.h>
#include <conio.h>
#include "appledosio.h"
#include "testutil.h"

static char read_exact(dos_buffer_t *buf, char *dst, unsigned int len) { unsigned int i; char rc; for(i=0;i<len;i++){rc=dos_read_byte(buf,dst+i);if(rc!=DOS_ERR_OK)return rc;} return DOS_ERR_OK; }

static int t_pass, t_fail;

#define ITERATIONS 100
#define DATA_LEN    8

int test_stress(void)
{
    dos_buffer_t *buf;
    char slot  = DOS_DEFAULT_SLOT;
    char drive = DOS_DEFAULT_DRIVE;
    char wdata[DATA_LEN], rdata[DATA_LEN];
    char rc;
    int iter, i, write_fails, read_fails;

    t_pass = 0; t_fail = 0;
    clrscr();
    printf("== 9/11 test_stress (%d iters) ==\n", ITERATIONS);

    buf = dos_get_buffer();
    CHECK("get_buffer", buf != 0);

    /* --- write phase: one file, 100 sequential records --- */
    rc = dos_open(buf, slot, drive, DOS_DEFAULT_VOLUME, "TSTRESS",
                  DOS_FILE_TYPE_BINARY, 0, DOS_OPEN_CREATE);
    CHECK("open for write", rc == DOS_ERR_OK);

    write_fails = 0;
    for (iter = 0; iter < ITERATIONS; iter++) {
        for (i = 0; i < DATA_LEN; i++)
            wdata[i] = (char)((iter + i) & 0xFF);
        rc = dos_write(buf, wdata, DATA_LEN);
        if (rc != DOS_ERR_OK) write_fails++;
        if ((iter % 10) == 9) printf("w");
    }
    printf("\n");
    CHECK("write phase ok", write_fails == 0);
    rc = dos_close(buf);
    CHECK("close write",    rc == DOS_ERR_OK);

    /* --- read phase: reopen, read back all 100 records --- */
    rc = dos_open(buf, slot, drive, DOS_DEFAULT_VOLUME, "TSTRESS",
                  DOS_FILE_TYPE_BINARY, 0, DOS_OPEN_NOCREATE);
    CHECK("open for read",  rc == DOS_ERR_OK);

    read_fails = 0;
    for (iter = 0; iter < ITERATIONS; iter++) {
        for (i = 0; i < DATA_LEN; i++)
            wdata[i] = (char)((iter + i) & 0xFF);
        rc = read_exact(buf, rdata, DATA_LEN);
        if (rc != DOS_ERR_OK) { read_fails++; continue; }
        for (i = 0; i < DATA_LEN; i++) {
            if (rdata[i] != wdata[i]) {
                if (read_fails == 0) {   /* dump only first mismatch */
                    HEXDUMP("exp", wdata, DATA_LEN);
                    HEXDUMP("got", rdata, DATA_LEN);
                }
                read_fails++;
                break;
            }
        }
        if ((iter % 10) == 9) printf("r");
    }
    printf("\n");
    CHECK("read phase ok",  read_fails == 0);
    rc = dos_close(buf);
    CHECK("close read",     rc == DOS_ERR_OK);

    if (write_fails || read_fails)
        printf("  w:%d r:%d errors\n", write_fails, read_fails);

    dos_delete(slot, drive, DOS_DEFAULT_VOLUME, "TSTRESS");

    SUMMARY();
    PAUSE();
    return t_fail;
}
