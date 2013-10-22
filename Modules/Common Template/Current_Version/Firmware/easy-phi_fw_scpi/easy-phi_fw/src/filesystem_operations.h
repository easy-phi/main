/*
 * filesystem_operations.h
 *
 * Created: 09/06/2013 20:07:55
 *  Author: limpkin
 */ 


#ifndef FILESYSTEM_OPERATIONS_H_
#define FILESYSTEM_OPERATIONS_H_

#include <asf.h>
#include "defines.h"

RET_TYPE get_bytes_from_opened_file(char* buffer, UINT nb_bytes, UINT* nb_bytes_read);
RET_TYPE read_string_from_opened_file(char* buffer, int32_t buffer_size);
RET_TYPE list_folder_contents(char* ptr, bool to_cdc_serial);
RET_TYPE mount_chanfat_filesystem(void);
RET_TYPE start_file_read(char* filename);
RET_TYPE end_current_file_read(void);

#endif /* FILESYSTEM_OPERATIONS_H_ */