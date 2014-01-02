/*
 * filesystem_operations.c
 *
 * Created: 09/06/2013 19:38:16
 *  Author: limpkin
 */ 
#include <asf.h>
#include <string.h>
#include "defines.h"
#include "filesystem_operations.h"

Ctrl_status chanfat_status;
FIL chanfat_file_object;
FILINFO chanfat_Finfo;
FRESULT chanfat_res;
FATFS chanfat_fs;
DIR chanfat_dir;
#if _USE_LFN
char Lfname[512];
#endif

char temp_fs_string[1024];


RET_TYPE mount_chanfat_filesystem(void)
{
	chanfat_status = sd_mmc_test_unit_ready(0);
	
	if (CTRL_FAIL == chanfat_status)
		return RETURN_NOK;
	
	memset(&chanfat_fs, 0, sizeof(FATFS));
	
	chanfat_res = f_mount(LUN_ID_SD_MMC_MCI_0_MEM, &chanfat_fs);
	if (FR_INVALID_DRIVE == chanfat_res) 
		return RETURN_NOK;
		
	return RETURN_OK;
}

RET_TYPE start_file_read(char* filename)
{
	chanfat_res = f_open(&chanfat_file_object, filename, FA_OPEN_EXISTING | FA_READ);
	
	if(chanfat_res == FR_NOT_READY)
		return RETURN_NOT_READY;
	else if(chanfat_res)
		return RETURN_NOK;
	else
		return RETURN_OK;
}

RET_TYPE get_bytes_from_opened_file(char* buffer, UINT nb_bytes, UINT* nb_bytes_read)
{
	chanfat_res = f_read(&chanfat_file_object, buffer, (UINT)nb_bytes, nb_bytes_read);
	
	if(chanfat_res)
		return RETURN_NOK;
	else
		return RETURN_OK;
}

RET_TYPE read_string_from_opened_file(char* buffer, int32_t buffer_size)
{
	char* buff_ptr;
	
	buff_ptr = f_gets(buffer, buffer_size, &chanfat_file_object);
	
	if(buff_ptr == buffer)
		return RETURN_OK;
	else
		return RETURN_NOK;
}

RET_TYPE end_current_file_read(void)
{
	f_close(&chanfat_file_object);
	return RETURN_OK;
}

RET_TYPE list_folder_contents(char* ptr, bool to_cdc_serial)
{
	uint32_t p1, s1, s2, str_ptr;
	
	while(*ptr == ' ') ptr++;
	chanfat_res = f_opendir(&chanfat_dir, ptr);
	
	if(chanfat_res) 
	{ 
		return RETURN_NOK;
	}
	
	p1 = s1 = s2 = 0;	
	for(;;) 
	{
		#if _USE_LFN
		chanfat_Finfo.lfname = Lfname;
		chanfat_Finfo.lfsize = sizeof(Lfname);
		#endif
		
		chanfat_res = f_readdir(&chanfat_dir, &chanfat_Finfo);
		if ((chanfat_res != FR_OK) || !chanfat_Finfo.fname[0]) break;
		if (chanfat_Finfo.fattrib & AM_DIR) 
		{
			s2++;
		} 
		else 
		{
			s1++; 
			p1 += chanfat_Finfo.fsize;
		}
		
		str_ptr= sprintf(temp_fs_string, "%c%c%c%c%c %u/%02u/%02u %02u:%02u %9lu  %s",
		(chanfat_Finfo.fattrib & AM_DIR) ? 'D' : '-',
		(chanfat_Finfo.fattrib & AM_RDO) ? 'R' : '-',
		(chanfat_Finfo.fattrib & AM_HID) ? 'H' : '-',
		(chanfat_Finfo.fattrib & AM_SYS) ? 'S' : '-',
		(chanfat_Finfo.fattrib & AM_ARC) ? 'A' : '-',
		(chanfat_Finfo.fdate >> 9) + 1980, (chanfat_Finfo.fdate >> 5) & 15, chanfat_Finfo.fdate & 31,
		(chanfat_Finfo.ftime >> 11), (chanfat_Finfo.ftime >> 5) & 63,
		chanfat_Finfo.fsize, &(chanfat_Finfo.fname[0]));
		#if _USE_LFN
		sprintf(temp_fs_string+str_ptr, "  %s\r\n", Lfname);
		#else
		sprintf(temp_fs_string+str_ptr, "\r\n");
		#endif
		
		if(to_cdc_serial)
		{
			udi_cdc_write_buf(temp_fs_string, strlen(temp_fs_string));
		}
	}
	sprintf(temp_fs_string, "%4u File(s),%10lu bytes total\n%4u Dir(s)\r\n", s1, p1, s2);
	udi_cdc_write_buf(temp_fs_string, strlen(temp_fs_string));
	
	return RETURN_OK;
}

/*FRESULT scan_files(int8_t* path)
{	
	uint16_t i;
	char *fn;

	if((chanfat_res = f_opendir(&chanfat_dir, path)) == FR_OK) 
	{
		i = strlen(path);
		while (((res = f_readdir(&chanfat_dir, &chanfet_Finfo)) == FR_OK) && chanfet_Finfo.fname[0]) 
		{
			#if _USE_LFN
			fn = *chanfet_Finfo.lfname ? chanfet_Finfo.lfname : chanfet_Finfo.fname;
			#else
			fn = chanfet_Finfo.fname;
			#endif
			if (chanfet_Finfo.fattrib & AM_DIR) 
			{
				acc_dirs++;
				*(path+i) = '/'; 
				strcpy(path+i+1, fn);
				chanfat_res = scan_files(path);
				*(path+i) = '\0';
				if (chanfat_res != FR_OK) break;
			} 
			else 
			{
				//				xprintf("%s/%s\n", path, fn);
				acc_files++;
				acc_size += Finfo.fsize;
			}
		}
	}
	
	return res;
}*/