#ifndef _APPLEDOSIO_H_
#define _APPLEDOSIO_H_

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
#define DOS_BASIC_FPROM 2
#define DOS_BASIC_FPRAM 3
#define DOS_BASIC_UNKNOWN 0

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

#endif
