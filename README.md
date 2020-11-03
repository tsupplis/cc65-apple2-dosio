# CC65 Apple II DOS 3.3 I/O Library

## Synopsis

apple2-dosio is an experimental Apple 2 DOS 3.3 file access library. It closes a functional gap of the CC65 Standard Library. It uses the DOS 3.3 File Manager API internally.

The Reference used is [Beneath Apple DOS](https://mirrors.apple2.org.za/Apple%20II%20Documentation%20Project/Books/Beneath%20Apple%20DOS.pdf) , File Manager Section pp 6.7-6.17.

Most of the calls are usable, with a limitation of the catalog function that displays content rather than giving access to the directory content.


## API

Further documentation will be provided but examples of usage can be found in test*.c files for the time being.

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

typedef struct _dos_buffer_t { ... } dos_buffer_t;

dos_buffer_t *dos_get_buffer(void);

int dos_check(void);
int dos_version(void);
int dos_catalog(char slot, char drive);
int dos_open(dos_buffer_t *buffer, char slot, char drive, char *file, 
    char type, unsigned int record_size, char creat);
int dos_close(dos_buffer_t *buffer);
int dos_delete(char slot, char drive, char *file);
int dos_rename(char slot, char drive, char *file, char *new_name);
int dos_lock(char slot, char drive, char *file);
int dos_unlock(char slot, char drive, char *file);
int dos_verify(char slot, char drive, char *file);
int dos_init(char slot, char drive, char volume);
int dos_write_byte(dos_buffer_t *buffer, char b);
int dos_read_byte(dos_buffer_t *buffer, char *b);
int dos_write(dos_buffer_t *buffer, char *b, unsigned int length);
int dos_read(dos_buffer_t *buffer, char *b, unsigned int length);
int dos_write_pos(dos_buffer_t *buffer, char *b, unsigned int length, 
    unsigned int record, unsigned int offset);
int dos_read_pos(dos_buffer_t *buffer, char *b, unsigned int length, 
    unsigned int record, unsigned int offset);
int dos_position(dos_buffer_t *buffer, unsigned int record, 
    unsigned int offset);
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
- [DOS 3.4 (2018/01)](https://brutaldeluxe.fr/projects/dos34/index.html)
