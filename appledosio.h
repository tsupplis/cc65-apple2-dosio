#ifndef _APPLEDOSIO_H_
#define _APPLEDOSIO_H_

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

#endif
