#include "PowerPC_EABI_Support/MSL_C/MSL_Common/buffer_io.h"
#include <stdio.h>

// not present in the retail binary; kept commented out for reference
//void __convert_from_newlines(){
//}
//
//void __convert_to_newlines(){
//}

void __prep_buffer(FILE *file)
{
    u32 size = file->buffer_size;
    u32 pos = file->position;
    u32 align = file->buffer_alignment;

    file->buffer_ptr = file->buffer;
    file->buffer_len = size - (pos & align);
    file->buffer_pos = pos;
}


// not present in the retail binary; kept commented out for reference
//void __load_buffer(){
//}

int __flush_buffer(FILE* file, size_t* length)
{
    size_t bufferLen;
    int writeCode;

    bufferLen = file->buffer_ptr - file->buffer;
    if (bufferLen) {
        file->buffer_len = bufferLen;
        writeCode = file->write_proc(file->handle, file->buffer, &file->buffer_len, file->ref_con);
        if (length) {
            *length = file->buffer_len;
        }
        if (writeCode) {
            return writeCode;
        }
        file->position += file->buffer_len;
    }

    file->buffer_ptr = file->buffer;
    file->buffer_len = file->buffer_size;
    file->buffer_len = file->buffer_len - (file->position & file->buffer_alignment);
    file->buffer_pos = file->position;
    return 0;
}

// not present in the retail binary; kept commented out for reference
//void setvbuf(){
//}
//
//void setbuf(){
//}
