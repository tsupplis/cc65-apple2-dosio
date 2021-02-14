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
#define dos_buffer_next(p) (dos_buffer_t *)(p->next_fn_addr - offsetof(dos_buffer_t, file_name))
#define dos_buffer_is_last(p) !(p->next_fn_addr)
#define dos_buffer_first() (dos_buffer_t *)(*((int *)(((char *)0x3D2)[0] << 8)) - offsetof(dos_buffer_t, file_name))
#define dos_buffer_is_free(p) (p->file_name[0] == 0)
#define dos_buffer_reset(p)                   \
    do                                        \
    {                                         \
        memset(p, 0, sizeof(dos_buffer_t));   \
        p->list_sector_addr = p->list_sector; \
        p->work_area_addr = p->work_area;     \
        p->data_sector_addr = p->data_sector; \
    } while (0)
dos_buffer_t *dos_get_buffer(void);

char dos_last_slot();
char dos_last_drive();
int dos_check(void);
int dos_version(void);
int dos_catalog(char slot, char drive, unsigned char volume);
int dos_open(dos_buffer_t *buffer, char slot, char drive, unsigned char volume, char *file, 
    char type, unsigned int record_size, char creat);
int dos_close(dos_buffer_t *buffer);
int dos_delete(char slot, char drive, unsigned char volume, char *file);
int dos_rename(char slot, char drive, unsigned char volume, char *file, char *new_name);
int dos_lock(char slot, char drive, unsigned char volume, char *file);
int dos_unlock(char slot, char drive, unsigned char volume, char *file);
int dos_verify(char slot, char drive, unsigned char volume, char *file);
int dos_init(char slot, char drive, unsigned char volume);
int dos_write_byte(dos_buffer_t *buffer, char b);
int dos_read_byte(dos_buffer_t *buffer, char *b);
int dos_write(dos_buffer_t *buffer, char *b, unsigned int length);
int dos_read(dos_buffer_t *buffer, char *b, unsigned int length);
int dos_write_pos(dos_buffer_t *buffer, char *b, unsigned int length, unsigned int record, unsigned int offset);
int dos_read_pos(dos_buffer_t *buffer, char *b, unsigned int length, unsigned int record, unsigned int offset);
int dos_position(dos_buffer_t *buffer, unsigned int record, unsigned int offset);

void dos_dump_buffers(void);

#endif
