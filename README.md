# CC65 Apple II DOS 3.3 I/O Library


## *Warning* 
The API is work in progress.

Note: when `dos_open` creates a new file, the File Manager itself answers FILE NOT FOUND (6) although the file is open; `dos_open` reports `DOS_ERR_OK` in that case, as DOS's own OPEN and BSAVE commands do.

## Synopsis

apple2-dosio is an experimental Apple 2 DOS 3.3 file access library. It closes a functional gap compared to ProDOS support in the CC65 Standard Library. It uses the DOS 3.3 File Manager API internally. We could imagine creating a specific implementation of the standard c library but for size and simplicity sake, it is its own API.

The Reference used is [Beneath Apple DOS](https://mirrors.apple2.org.za/Apple%20II%20Documentation%20Project/Books/Beneath%20Apple%20DOS.pdf), File Manager Section pp 6.7-6.17.

Most of the calls are usable, with a limitation of the catalog function that displays content rather than giving access to the directory content.

Also, the code is fairly lean but no strong optimisation has been attempted. 

## API

Further documentation will be provided but examples of usage can be found in test*.c files for the time being. The API reflects as closely as possible the File Manager API ([Beneath Apple DOS](https://mirrors.apple2.org.za/Apple%20II%20Documentation%20Project/Books/Beneath%20Apple%20DOS.pdf) , File Manager Section pp 6.7-6.17). 

```c
/* File types */
#define DOS_FILE_TYPE_TEXT       0x00
#define DOS_FILE_TYPE_INTBASIC   0x01
#define DOS_FILE_TYPE_APPLESOFT  0x02
#define DOS_FILE_TYPE_BINARY     0x04
#define DOS_FILE_TYPE_RELOCATABLE 0x08
#define DOS_FILE_TYPE_S          0x10
#define DOS_FILE_TYPE_A          0x20
#define DOS_FILE_TYPE_B          0x40

/* Open mode */
#define DOS_OPEN_CREATE   0x01  /* create if absent */
#define DOS_OPEN_NOCREATE 0x00  /* fail if absent   */

/* Error codes */
#define DOS_ERR_OK                  0x00
#define DOS_ERR_NOT_USED            0x01
#define DOS_ERR_BAD_CALL_TYPE       0x02
#define DOS_ERR_BAD_SUBCALL_TYPE    0x03
#define DOS_ERR_WRITE_PROTECTED     0x04
#define DOS_ERR_EOD                 0x05  /* end of data */
#define DOS_ERR_FILE_NO_FOUND       0x06
#define DOS_ERR_DISK_VOLUME_MISMATCH 0x07
#define DOS_ERR_DISK_IO             0x08
#define DOS_ERR_DISK_FULL           0x09
#define DOS_ERR_FILE_LOCKED         0x0A

/* Use defaults for slot/drive/volume */
#define DOS_DEFAULT_SLOT   0
#define DOS_DEFAULT_DRIVE  0
#define DOS_DEFAULT_VOLUME 0

/* BASIC interpreter type (returned by dos_basic_version) */
#define DOS_BASIC_INT     1  /* Integer BASIC   */
#define DOS_BASIC_FPROM   2  /* Applesoft ROM   */
#define DOS_BASIC_FPRAM   3  /* Applesoft RAM   */
#define DOS_BASIC_UNKNOWN 0

/* Per-file I/O buffer (one per open file) */
typedef struct _dos_buffer_t
{
    char data_sector[256];
    char list_sector[256];
    char work_area[45];
    char file_name[30];
    char *work_area_addr;
    char *list_sector_addr;
    char *data_sector_addr;
    char *next_fn_addr;
} dos_buffer_t;

/* Return a pointer to the DOS-allocated buffer chain */
dos_buffer_t *dos_get_buffer(void);

/* Slot/drive of the last/current/default DOS operation */
char dos_default_slot(void);
char dos_default_drive(void);
char dos_active_slot(void);   /* NOTE: unreliable on DOS 3.4 (workspace not at $AA00) */
char dos_active_drive(void);  /* NOTE: unreliable on DOS 3.4 (workspace not at $AA00) */
char dos_last_slot(void);
char dos_last_drive(void);

/* Maximum open files (current and default)
   NOTE: unreliable on DOS 3.4 (workspace not at $AA00) */
char dos_default_maxfiles(void);
char dos_maxfiles(void);

/* Detect DOS, return version byte, return BASIC type
   NOTE: dos_basic_version() unreliable on DOS 3.4 (workspace not at $AA00) */
char dos_check(void);
char dos_version(void);
char dos_basic_version(void);

/* Directory listing to screen (slot/drive/volume, 0 = default) */
char dos_catalog(char slot, char drive, unsigned char volume);

/* Open a file; creat = DOS_OPEN_CREATE or DOS_OPEN_NOCREATE */
char dos_open(dos_buffer_t *buffer, char slot, char drive, unsigned char volume,
              char *file, char type, unsigned int record_size, char creat);

/* Close an open file */
char dos_close(dos_buffer_t *buffer);

/* Delete a file by name */
char dos_delete(char slot, char drive, unsigned char volume, char *file);

/* Rename a file */
char dos_rename(char slot, char drive, unsigned char volume, char *file, char *new_name);

/* Lock / unlock a file (write-protect flag in directory) */
char dos_lock(char slot, char drive, unsigned char volume, char *file);
char dos_unlock(char slot, char drive, unsigned char volume, char *file);

/* Verify a file exists and is readable */
char dos_verify(char slot, char drive, unsigned char volume, char *file);

/* Re-initialize (format) a disk */
char dos_init(char slot, char drive, unsigned char volume);

/* Sequential byte I/O */
char dos_write_byte(dos_buffer_t *buffer, char b);
char dos_read_byte(dos_buffer_t *buffer, char *b);

/* Sequential block I/O (length bytes)
   NOTE: unreliable on ProntoDOS — FM range calls ($3D6/RWN) do not report EOF
   and do not maintain random-access position correctly (ProntoDOS manual p.17) */
char dos_write(dos_buffer_t *buffer, char *b, unsigned int length);
char dos_read(dos_buffer_t *buffer, char *b, unsigned int length);

/* Random-access block I/O (record + byte offset)
   NOTE: unreliable on ProntoDOS — same FM range-call limitation as above */
char dos_write_pos(dos_buffer_t *buffer, char *b, unsigned int length, unsigned int record, unsigned int offset);
char dos_read_pos(dos_buffer_t *buffer, char *b, unsigned int length, unsigned int record, unsigned int offset);

/* Seek to record + byte offset without reading or writing
   NOTE: unreliable on ProntoDOS — same FM range-call limitation as above */
char dos_position(dos_buffer_t *buffer, unsigned int record, unsigned int offset);

/* Dump all DOS buffer chain entries to stdout (debug) */
void dos_dump_buffers(void);

```

## Dependencies

- [CC65 C Compiler](https://github.com/cc65/cc65)
- [AppleCommander 1.6.0](https://github.com/AppleCommander/AppleCommander)
- DOS 3.3 Image Files or compatible (see below)
- A DOS Emulator for testing (e.g. [Virtual II](https://www.virtualii.com) on Mac)

## Versions of DOS tested against

- [DOS 3.3](https://mirrors.apple2.org.za/ftp.apple.asimov.net/images/masters/) (1980/08, 1983/01, 1983/08 ...)
- [ProntoDOS](https://mirrors.apple2.org.za/ftp.apple.asimov.net/images/masters/3rd_party_dos/) (1984/08)
- [DiversiDOS](https://mirrors.apple2.org.za/ftp.apple.asimov.net/images/masters/3rd_party_dos/) (4.1c)
- [DOS 3.4](https://brutaldeluxe.fr/projects/dos34/index.html) (2018/01)
    Some Caveats (maxfiles and active_** call do not work)
- DOS.3.3.Launcher 2.2 + DOS 3.3 and ProntoDOS

## Test Compatibility Matrix

Legend: ✅ pass · ❌ fail · ➖ N/A (feature not supported by this DOS)

| Test | Focus | DOS 3.3 | DiversiDOS | DavidOS | DOS 3.4 | ProntoDOS |
|------|-------|:-------:|:----------:|:-------:|:-------:|:---------:|
| 1/11 `test_dosinfo`  | DOS presence, version, slot/drive | ✅ | ✅ | ✅ | ✅ ¹ | ✅ |
| 2/11 `test_catalog`  | verify, rename, delete | ✅ | ✅ | ✅ | ✅ | ✅ |
| 3/11 `test_lockfile` | lock, unlock, write-to-locked | ✅ | ✅ | ✅ | ✅ | ✅ |
| 4/11 `test_create`   | OPEN_CREATE new/existing, NOCREATE missing | ✅ | ✅ | ✅ | ✅ | ❌ ² |
| 5/11 `test_rwbyte`   | write_byte / read_byte integrity | ✅ | ✅ | ✅ | ✅ | ✅ |
| 6/11 `test_rwblock`  | write / read block integrity (BINARY) | ✅ | ✅ | ✅ | ✅ | ❌ ³ |
| 7/11 `test_textio`   | text lines write/read/visual | ✅ | ✅ | ✅ | ✅ | ❌ ³ |
| 8/11 `test_position` | write_pos, read_pos, position | ✅ | ✅ | ✅ | ✅ | ❌ ³ |
| 9/11 `test_stress`   | 100-record sequential write/read | ✅ | ✅ | ✅ | ✅ | ❌ ³ |
| 10/11 `test_eod`     | read past end of file | ✅ | ✅ | ✅ | ✅ | ❌ ³ |
| 11/11 `test_multifile` | two files open simultaneously | ✅ | ✅ | ✅ | ✅ | ❌ ³ |

**Notes**

¹ DOS 3.4: `dos_version()`, `dos_basic_version()`, `dos_maxfiles()`, `dos_active_slot()`, `dos_active_drive()` return 0 or garbage. Memory scan confirms the DOS workspace is no longer at `$AA00` — DOS 3.4 reorganised its OS code into `$1D00–$35FF` and the metadata fields moved to undocumented addresses. `dos_check()` (`$3D0=4C`), `dos_default_slot()` and `dos_default_drive()` work correctly because they fall back to `$B7F7`/`$B7F8` (LAST_SLOT/LAST_DRIVE) which are unchanged.

² ProntoDOS / `test_create`: `DOS_OPEN_CREATE` on an existing file does not delete or truncate it first — the write goes to position 0 but the old sector content is returned on re-read. This may be related to the File Manager incompatibility described in note ³.

³ ProntoDOS — **documented File Manager incompatibility** (ProntoDOS manual, p.17, Tom Weishaar): ProntoDOS deliberately changes the semantics of the File Manager range read/write operations (`DOS_SUBCALL_RWN`, `DOS_SUBCALL_PRWN`). Specifically: range routines do not report EOF errors, and do not maintain random-access file position correctly. This library calls the File Manager directly through `$3D6` and is therefore in the affected path. Tests 6–11 all use range operations and fail consistently. `test_rwbyte` (single-byte operations, `DOS_SUBCALL_RW1`) passes because ProntoDOS does not alter single-byte read/write behaviour. The last-byte-of-sector `0x00` corruption and the `test_create` overwrite failure may be related symptoms of the same underlying changes, though this has not been confirmed from source.


