# CC65 Apple II DOS 3.3 I/O Library


## *Warning* 
The API is work in progress: The following functions are still not working

- Multibyte Read/Write
- File Rename

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


