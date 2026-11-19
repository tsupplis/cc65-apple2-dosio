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

```
#define DOS_FILE_TYPE_TEXT 0x00
#define DOS_FILE_TYPE_INTBASIC 0x01
#define DOS_FILE_TYPE_APPLESOFT 0x02
#define DOS_FILE_TYPE_BINARY 0x04
#define DOS_FILE_TYPE_RELOCATABLE 0x08
#define DOS_FILE_TYPE_S 0x10
#define DOS_FILE_TYPE_A 0x20
#define DOS_FILE_TYPE_B 0x40

#define DOS_OPEN_CREATE 0x01
#define DOS_OPEN_NOCREATE 0x00

#define DOS_ERR_OK 0x00
#define DOS_ERR_NOT_USED 0x01
#define DOS_ERR_BAD_CALL_TYPE 0x02
#define DOS_ERR_BAD_SUBCALL_TYPE 0x03
#define DOS_ERR_WRITE_PROTECTED 0x04
#define DOS_ERR_EOD 0x05
#define DOS_ERR_FILE_NO_FOUND 0x06
#define DOS_ERR_DISK_VOLUME_MISMATCH 0x07
#define DOS_ERR_DISK_IO 0x08
#define DOS_ERR_DISK_FULL 0x09
#define DOS_ERR_FILE_LOCKED 0x0A

#define DOS_DEFAULT_SLOT 0
#define DOS_DEFAULT_DRIVE 0
#define DOS_DEFAULT_VOLUME 0

#define DOS_BASIC_INT 1
#define DOS_BASIC_APPLESOFT 2
#define DOS_BASIC_UNKNOWN 0

typedef struct _dos_buffer_t { ... } dos_buffer_t;

dos_buffer_t *dos_get_buffer(void);

char dos_default_slot();
char dos_default_drive();
char dos_active_slot();
char dos_active_drive();
char dos_last_slot();
char dos_last_drive();

char dos_default_maxfiles();
char dos_maxfiles();

char dos_check(void);
char dos_version(void);
char dos_basic_version(void);
char dos_catalog(char slot, char drive, unsigned char volume);
char dos_open(dos_buffer_t *buffer, char slot, char drive, unsigned char volume, 
    char *file, char type, unsigned int record_size, char creat);
char dos_close(dos_buffer_t *buffer);
char dos_delete(char slot, char drive, unsigned char volume, char *file);
char dos_rename(char slot, char drive, unsigned char volume, char *file, char *new_name);
char dos_lock(char slot, char drive, unsigned char volume, char *file);
char dos_unlock(char slot, char drive, unsigned char volume, char *file);
char dos_verify(char slot, char drive, unsigned char volume, char *file);
char dos_init(char slot, char drive, unsigned char volume);
char dos_write_byte(dos_buffer_t *buffer, char b);
char dos_read_byte(dos_buffer_t *buffer, char *b);
char dos_write(dos_buffer_t *buffer, char *b, unsigned int length);
char dos_read(dos_buffer_t *buffer, char *b, unsigned int length);
char dos_write_pos(dos_buffer_t *buffer, char *b, unsigned int length, unsigned int record, unsigned int offset);
char dos_read_pos(dos_buffer_t *buffer, char *b, unsigned int length, unsigned int record, unsigned int offset);
char dos_position(dos_buffer_t *buffer, unsigned int record, unsigned int offset);

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
| 1/11 `test_dosinfo`  | DOS presence, version, slot/drive | ✅ | ✅ | ✅ | ✅ ¹ | ✅ ² |
| 2/11 `test_catalog`  | verify, rename, delete | ✅ | ✅ | ✅ | ✅ | ✅ |
| 3/11 `test_lockfile` | lock, unlock, write-to-locked | ✅ | ✅ | ✅ | ✅ | ✅ |
| 4/11 `test_create`   | OPEN_CREATE new/existing, NOCREATE missing | ✅ | ✅ | ✅ | ✅ | ❌ ³ |
| 5/11 `test_rwbyte`   | write_byte / read_byte integrity | ✅ | ✅ | ✅ | ✅ | ✅ |
| 6/11 `test_rwblock`  | write / read block integrity (BINARY) | ✅ | ✅ | ✅ | ✅ | ❌ ⁴ |
| 7/11 `test_textio`   | text lines write/read/visual | ✅ | ✅ | ✅ | ✅ | ❌ ⁴ |
| 8/11 `test_position` | write_pos, read_pos, position | ✅ | ✅ | ✅ | ✅ | ❌ ⁴ |
| 9/11 `test_stress`   | 100-record sequential write/read | ✅ | ✅ | ✅ | ✅ | ❌ ⁴ |
| 10/11 `test_eod`     | read past end of file | ✅ | ✅ | ✅ | ✅ | ❌ ⁴ |
| 11/11 `test_multifile` | two files open simultaneously | ✅ | ✅ | ✅ | ✅ | ❌ ⁴ |

**Notes**

¹ DOS 3.4: `dos_version()`, `dos_basic_version()`, `dos_maxfiles()` return 0 / garbage — zero-page offsets differ from DOS 3.3. `dos_check()`, slot and drive accessors work correctly.

² ProntoDOS: version and maxfiles accessors return garbage (`mf=A0`), indicating incompatible zero-page layout for those fields.

³ ProntoDOS: `dos_open` with `DOS_OPEN_CREATE` on an existing file does not delete/truncate the file first. The new data is written at position 0, but on re-read the old sector content is returned — the write appears not to persist. The read returns `C1 C2 C3 00` (original data, with last byte `00` due to note ⁴) instead of the expected `D8 D9 DA 8D` (overwrite data). Whether this is a write-caching issue or a sector allocation bug in ProntoDOS requires further investigation.

⁴ ProntoDOS: `dos_read_byte` does not advance the file position on the last byte of a sector — the final byte of any read sequence is always returned as `0x00`. This causes all block-read integrity checks to fail. Root cause is probably a bug in the ProntoDOS File Manager implementation.


