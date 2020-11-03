# CC65 Apple II DOS 3.3 I/O Library

## Synopsis

apple2-dosio is an experimental Apple 2 DOS 3.3 file access library. It closes a functional gap of the CC65 Standard Library. It uses the DOS 3.3 File Manager API internally.


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
int dos_open(dos_buffer_t *buffer, char slot, char drive, char *file, char type, unsigned int record_size, char creat);
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
int dos_write_pos(dos_buffer_t *buffer, char *b, unsigned int length, unsigned int record, unsigned int offset);
int dos_read_pos(dos_buffer_t *buffer, char *b, unsigned int length, unsigned int record, unsigned int offset);
int dos_position(dos_buffer_t *buffer, unsigned int record, unsigned int offset);
```

## Dependencies

- CC65 C Compiler
- AppleCommander 1.6.0
- DOS 3.3 Image Files
- A DOS Emulator for testing

## Versions of DOS tested against

- DOS 3.3 (All versions)
- ProntoDOS 1984
- DOS 3.4 (https://brutaldeluxe.fr/projects/dos34/index.html)
