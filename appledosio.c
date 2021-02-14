#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <string.h>
#include <apple2.h>
#include <peekpoke.h>
#include "appledosio.h"
#include "dump.h"

#ifndef __CC65__
#define __asm__(a, ...)
#endif

#define DOS_CALL_OPEN 0x01
#define DOS_CALL_CLOSE 0x02
#define DOS_CALL_READ 0x03
#define DOS_CALL_WRITE 0x04
#define DOS_CALL_DELETE 0x05
#define DOS_CALL_CATALOG 0x06
#define DOS_CALL_LOCK 0x07
#define DOS_CALL_UNLOCK 0x08
#define DOS_CALL_RENAME 0x09
#define DOS_CALL_POSITION 0x0A
#define DOS_CALL_INIT 0x0B
#define DOS_CALL_VERIFY 0x0C

#define DOS_SUBCALL_NOOP 0x00
#define DOS_SUBCALL_RW1 0x01
#define DOS_SUBCALL_RWN 0x02
#define DOS_SUBCALL_PRW1 0x03
#define DOS_SUBCALL_PRWN 0x04

typedef struct _fm_pcb_t
{
    char call_type;
    char subcall_type;
    char params[8];
    char return_code;
    char unused_b;
    char *work_area_addr;
    char *list_sector_addr;
    char *data_sector_addr;
} fm_pcb_t;

#define PARAM_OFFSET_RECORD_SIZE 0x00
#define PARAM_OFFSET_RECORD 0x00
#define PARAM_OFFSET_NEW_NAME 0x00
#define PARAM_OFFSET_OFFSET 0x02
#define PARAM_OFFSET_VOLUME 0x02
#define PARAM_OFFSET_DRIVE 0x03
#define PARAM_OFFSET_BUFFER_LEN 0x04
#define PARAM_OFFSET_SLOT 0x04
#define PARAM_OFFSET_TYPE 0x05
#define PARAM_OFFSET_FILE_NAME 0x06
#define PARAM_OFFSET_SINGLE_BYTE 0x06
#define PARAM_OFFSET_BUFFER 0x06

#define fm_pcb_volume(p) p->params[PARAM_OFFSET_VOLUME]
#define fm_pcb_drive(p) p->params[PARAM_OFFSET_DRIVE]
#define fm_pcb_slot(p) p->params[PARAM_OFFSET_SLOT]
#define fm_pcb_type(p) p->params[PARAM_OFFSET_TYPE]
#define fm_pcb_single_byte(p) p->params[PARAM_OFFSET_SINGLE_BYTE]
#define fm_pcb_call_type(p) p->call_type
#define fm_pcb_subcall_type(p) p->subcall_type
#define fm_pcb_new_name(p) *((char **)(p->params + PARAM_OFFSET_NEW_NAME))
#define fm_pcb_file_name(p) *((char **)(p->params + PARAM_OFFSET_FILE_NAME))
#define fm_pcb_record_size(p) *((unsigned int *)(p->params + PARAM_OFFSET_RECORD_SIZE))
#define fm_pcb_buffer_len(p) *((unsigned int *)(p->params + PARAM_OFFSET_BUFFER_LEN))
#define fm_pcb_record(p) *((unsigned int *)(p->params + PARAM_OFFSET_RECORD))
#define fm_pcb_offset(p) *((unsigned int *)(p->params + PARAM_OFFSET_OFFSET))
#define fm_pcb_buffer(p) *((char **)(p->params + PARAM_OFFSET_BUFFER))
#define fm_pcb_work_area(p) p->work_area_addr
#define fm_pcb_return_code(p) p->return_code
#define fm_pcb_map_buffers(p, b)                   \
    do                                             \
    {                                              \
        p->work_area_addr = b->work_area_addr;     \
        p->list_sector_addr = b->list_sector_addr; \
        p->data_sector_addr = b->data_sector_addr; \
    } while (0)

#define ACTIVE_SLOT 0xAA6A
#define ACTIVE_DRIVE 0xAA68
#define BASIC_FLAG 0xAAB6

static dos_buffer_t _dos_buffer[1];
static char _default_drive=0;
static char _default_slot=0;

char default_slot(char slot) {
    if(slot<1) {
        if(!_default_slot) {
            _default_slot=PEEK(ACTIVE_SLOT);
        }
        return _default_slot;
    } 
    if(slot>7) {
        return 7;
    }
    return slot;
}

char default_drive(char drive) {
    if(drive<1) {
        if(!_default_drive) {
            _default_drive=PEEK(ACTIVE_DRIVE);
        }
        return _default_drive;
    } 
    if(drive>2) {
        return 2;
    }
    return drive;
}

char dos_check(void)
{
    static char _dos_value;
    __asm__("lda $3D0");
    __asm__("sta %v", _dos_value);
    return _dos_value == 0x4C;
}

char dos_basic_version(void)
{
    char v=PEEK(BASIC_FLAG);
    if(v==1) {
        return DOS_BASIC_INT;
    }
    if(v==64) {
        return DOS_BASIC_FPROM;
    }
    if(v==128) {
        return DOS_BASIC_FPRAM;
    }
    return 0;
}

char dos_version(void)
{
    static char _dos_value;
    __asm__("clc");
    __asm__("lda #$0");
    __asm__("adc #$be");
    __asm__("sta $0");
    __asm__("lda $3d2");
    __asm__("adc #$16");
    __asm__("sta $1");
    __asm__("ldy #$0");
    __asm__("lda ($0),y");
    __asm__("sta %v", _dos_value);
    return _dos_value;
}

static fm_pcb_t *dos_pcb()
{
    static char _pcb_a;
    static char _pcb_y;
    __asm__("jsr $3DC");
    __asm__("sta %v", _pcb_a);
    __asm__("sty %v", _pcb_y);
    return (fm_pcb_t *)(((_pcb_a << 8) & 0xFF00) + (_pcb_y));
}

char dos_catalog(char slot, char drive, unsigned char volume)
{
    fm_pcb_t *pcb = dos_pcb();

    dos_buffer_reset(_dos_buffer);
    fm_pcb_call_type(pcb) = DOS_CALL_CATALOG;
    fm_pcb_slot(pcb) = default_slot(slot);
    fm_pcb_drive(pcb) = default_drive(drive);
    fm_pcb_volume(pcb) = volume;
    fm_pcb_work_area(pcb) = _dos_buffer->work_area;
    __asm__("jsr $3D6");
    return fm_pcb_return_code(pcb);
}

void dos_dump_buffers()
{
    dos_buffer_t *p = dos_buffer_first();
    while (1)
    {
        printf("P=B/%04X D/%04X F/%04X\n", p, p->data_sector, p->file_name);
        dump_hex(p->file_name, 30, 1);
        printf("DS=%04X ", p->data_sector_addr);
        printf("LS=%04X ", p->list_sector_addr);
        printf("WA=%04X ", p->work_area_addr);
        printf("NEXTFN=%04X\n", p->next_fn_addr);
        if (dos_buffer_is_last(p))
        {
            break;
        }
        p = dos_buffer_next(p);
    }
}

static void _dos_copy_file_name(char *a, char *b)
{
    char i = 0;
    while (i < 30 && b[i])
    {
        a[i] = 0x80 | b[i];
        i++;
    }
    while (i < 30)
    {
        a[i++] = 0xA0;
    }
}

char dos_open(dos_buffer_t *buffer, char slot, char drive, unsigned char volume, 
    char *file, char type, unsigned int record_size, char creat)
{
    fm_pcb_t *pcb = dos_pcb();
    fm_pcb_call_type(pcb) = DOS_CALL_OPEN;
    fm_pcb_record_size(pcb) = record_size;
    fm_pcb_slot(pcb) = default_slot(slot);
    fm_pcb_drive(pcb) = default_drive(drive);
    fm_pcb_volume(pcb) = volume;
    fm_pcb_type(pcb) = type;
    fm_pcb_file_name(pcb) = buffer->file_name;
    fm_pcb_map_buffers(pcb, buffer);
    _dos_copy_file_name(buffer->file_name, file);
    if (creat)
    {
        __asm__("ldx #$0");
    }
    else
    {
        __asm__("ldx #$1");
    }
    __asm__("jsr $3D6");
    return fm_pcb_return_code(pcb);
}

char dos_delete(char slot, char drive, unsigned char volume, char *file)
{
    fm_pcb_t *pcb = dos_pcb();
    dos_buffer_reset(_dos_buffer);
    fm_pcb_call_type(pcb) = DOS_CALL_DELETE;
    fm_pcb_slot(pcb) = default_slot(slot);
    fm_pcb_drive(pcb) = default_drive(drive);
    fm_pcb_volume(pcb) = volume;
    fm_pcb_file_name(pcb) = _dos_buffer->file_name;
    fm_pcb_map_buffers(pcb, _dos_buffer);
    _dos_copy_file_name(_dos_buffer->file_name, file);
    __asm__("jsr $3D6");
    return fm_pcb_return_code(pcb);
}

static char _dos_rename_name[30];

char dos_rename(char slot, char drive, unsigned char volume, char *file, char *new_name)
{
    fm_pcb_t *pcb = dos_pcb();
    dos_buffer_reset(_dos_buffer);
    fm_pcb_call_type(pcb) = DOS_CALL_RENAME;
    fm_pcb_slot(pcb) = default_slot(slot);
    fm_pcb_drive(pcb) = default_drive(drive);
    fm_pcb_volume(pcb) = volume;
    fm_pcb_file_name(pcb) = _dos_buffer->file_name;
    fm_pcb_new_name(pcb) = _dos_rename_name;
    fm_pcb_map_buffers(pcb, _dos_buffer);
    _dos_copy_file_name(_dos_buffer->file_name, file);
    _dos_copy_file_name(_dos_rename_name, new_name);
    __asm__("jsr $3D6");
    return fm_pcb_return_code(pcb);
}

char dos_lock(char slot, char drive, unsigned char volume, char *file)
{
    fm_pcb_t *pcb = dos_pcb();
    dos_buffer_reset(_dos_buffer);
    fm_pcb_call_type(pcb) = DOS_CALL_LOCK;
    fm_pcb_slot(pcb) = default_slot(slot);
    fm_pcb_drive(pcb) = default_drive(drive);
    fm_pcb_volume(pcb) = volume;
    fm_pcb_file_name(pcb) = _dos_buffer->file_name;
    fm_pcb_map_buffers(pcb, _dos_buffer);
    _dos_copy_file_name(_dos_buffer->file_name, file);
    __asm__("jsr $3D6");
    return fm_pcb_return_code(pcb);
}

char dos_unlock(char slot, char drive, unsigned char volume, char *file)
{
    fm_pcb_t *pcb = dos_pcb();
    dos_buffer_reset(_dos_buffer);
    fm_pcb_call_type(pcb) = DOS_CALL_UNLOCK;
    fm_pcb_slot(pcb) = default_slot(slot);
    fm_pcb_drive(pcb) = default_drive(drive);
    fm_pcb_volume(pcb) = volume;
    fm_pcb_file_name(pcb) = _dos_buffer->file_name;
    fm_pcb_map_buffers(pcb, _dos_buffer);
    _dos_copy_file_name(_dos_buffer->file_name, file);
    __asm__("jsr $3D6");
    return fm_pcb_return_code(pcb);
}

char dos_verify(char slot, char drive, unsigned char volume, char *file)
{
    fm_pcb_t *pcb = dos_pcb();
    dos_buffer_reset(_dos_buffer);
    fm_pcb_call_type(pcb) = DOS_CALL_VERIFY;
    fm_pcb_slot(pcb) = default_slot(slot);
    fm_pcb_drive(pcb) = default_drive(drive);
    fm_pcb_volume(pcb) = volume;
    fm_pcb_file_name(pcb) = _dos_buffer->file_name;
    fm_pcb_map_buffers(pcb, _dos_buffer);
    _dos_copy_file_name(_dos_buffer->file_name, file);
    __asm__("jsr $3D6");
    return fm_pcb_return_code(pcb);
}

char dos_init(char slot, char drive, unsigned char volume)
{
    char **fn = (char **)((((char *)0x3D2)[0] << 8) + 6);
    fm_pcb_t *pcb = dos_pcb();
    fm_pcb_call_type(pcb) = DOS_CALL_INIT;
    fm_pcb_subcall_type(pcb) = ((char *)0x3D2)[0];
    fm_pcb_slot(pcb) = default_slot(slot);
    fm_pcb_drive(pcb) = default_drive(drive);
    fm_pcb_volume(pcb) = volume;
    _dos_copy_file_name(*fn, "HELLO");
    fm_pcb_work_area(pcb) = _dos_buffer->work_area;
    __asm__("jsr $3D6");
    return fm_pcb_return_code(pcb);
}

char dos_close(dos_buffer_t *buffer)
{
    fm_pcb_t *pcb = dos_pcb();
    fm_pcb_call_type(pcb) = DOS_CALL_CLOSE;
    fm_pcb_map_buffers(pcb, buffer);
    __asm__("jsr $3D6");
    buffer->file_name[0] = 0;
    return fm_pcb_return_code(pcb);
}

char dos_write_byte(dos_buffer_t *buffer, char b)
{
    fm_pcb_t *pcb = dos_pcb();
    fm_pcb_call_type(pcb) = DOS_CALL_WRITE;
    fm_pcb_subcall_type(pcb) = DOS_SUBCALL_RW1;
    fm_pcb_single_byte(pcb) = b;
    fm_pcb_map_buffers(pcb, buffer);
    __asm__("jsr $3D6");
    return fm_pcb_return_code(pcb);
}

char dos_read_byte(dos_buffer_t *buffer, char *b)
{
    fm_pcb_t *pcb = dos_pcb();
    fm_pcb_call_type(pcb) = DOS_CALL_READ;
    fm_pcb_subcall_type(pcb) = DOS_SUBCALL_RW1;
    fm_pcb_map_buffers(pcb, buffer);
    __asm__("jsr $3D6");
    *b = fm_pcb_single_byte(pcb);
    return fm_pcb_return_code(pcb);
}

char dos_write(dos_buffer_t *buffer, char *b, unsigned int length)
{
    fm_pcb_t *pcb = dos_pcb();
    if (length == 0)
    {
        return 0;
    }
    fm_pcb_call_type(pcb) = DOS_CALL_WRITE;
    fm_pcb_subcall_type(pcb) = DOS_SUBCALL_RWN;
    fm_pcb_buffer_len(pcb) = length - 1; /* write length -1 */
    fm_pcb_buffer(pcb) = b;
    fm_pcb_map_buffers(pcb, buffer);
    __asm__("jsr $3D6");
    return fm_pcb_return_code(pcb);
}

char dos_read(dos_buffer_t *buffer, char *b, unsigned int length)
{
    fm_pcb_t *pcb = dos_pcb();
    if (length == 0)
    {
        return 0;
    }
    fm_pcb_call_type(pcb) = DOS_CALL_READ;
    fm_pcb_subcall_type(pcb) = DOS_SUBCALL_RWN;
    fm_pcb_buffer_len(pcb) = length;
    fm_pcb_buffer(pcb) = b;
    fm_pcb_map_buffers(pcb, buffer);
    __asm__("jsr $3D6");
    return fm_pcb_return_code(pcb);
}

char dos_write_pos(dos_buffer_t *buffer, char *b, unsigned int length, 
    unsigned int record, unsigned int offset)
{
    fm_pcb_t *pcb = dos_pcb();
    if (length == 0)
    {
        return 0;
    }
    fm_pcb_call_type(pcb) = DOS_CALL_WRITE;
    fm_pcb_subcall_type(pcb) = DOS_SUBCALL_PRWN;
    fm_pcb_record(pcb) = record;
    fm_pcb_offset(pcb) = offset;
    fm_pcb_buffer_len(pcb) = length - 1; /* write length -1 */
    fm_pcb_buffer(pcb) = b;
    fm_pcb_map_buffers(pcb, buffer);
    __asm__("jsr $3D6");
    return fm_pcb_return_code(pcb);
}

char dos_read_pos(dos_buffer_t *buffer, char *b, unsigned int length, unsigned int record, unsigned int offset)
{
    fm_pcb_t *pcb = dos_pcb();
    if (length == 0)
    {
        return 0;
    }
    fm_pcb_call_type(pcb) = DOS_CALL_READ;
    fm_pcb_subcall_type(pcb) = DOS_SUBCALL_PRWN;
    fm_pcb_record(pcb) = record;
    fm_pcb_offset(pcb) = offset;
    fm_pcb_buffer_len(pcb) = length;
    fm_pcb_buffer(pcb) = b;
    fm_pcb_map_buffers(pcb, buffer);
    __asm__("jsr $3D6");
    return fm_pcb_return_code(pcb);
}

char dos_position(dos_buffer_t *buffer, unsigned int record, unsigned int offset)
{
    fm_pcb_t *pcb = dos_pcb();
    fm_pcb_call_type(pcb) = DOS_CALL_POSITION;
    fm_pcb_record(pcb) = record;
    fm_pcb_offset(pcb) = offset;
    fm_pcb_map_buffers(pcb, buffer);
    __asm__("jsr $3D6");
    return fm_pcb_return_code(pcb);
}

dos_buffer_t *dos_get_buffer(void)
{
    dos_buffer_t *buf = dos_buffer_first();
    while (!dos_buffer_is_free(buf) && !dos_buffer_is_last(buf))
    {
        buf = dos_buffer_next(buf);
    }
    if (dos_buffer_is_free(buf))
    {
        return buf;
    }
    return 0;
}

char dos_last_slot() {
    return PEEK(ACTIVE_SLOT);
}

char dos_last_drive() {
    return PEEK(ACTIVE_DRIVE);
}

char dos_default_slot() {
    return default_slot(0);
}

char dos_default_drive() {
    return default_drive(0);
}
