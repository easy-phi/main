/*
 * utils.c
 *
 * Created: 03/08/2013 21:42:52
 *  Author: limpkin
 */
#include "asf.h" 
#include "utils.h"

/**
 * \brief Calculate week from year, month, day.
 */
uint32_t calculate_week(uint32_t ul_year, uint32_t ul_month, uint32_t ul_day)
{
	uint32_t ul_week;

	if (ul_month == 1 || ul_month == 2) 
	{
		ul_month += 12;
		--ul_year;
	}

	ul_week = (ul_day + 2 * ul_month + 3 * (ul_month + 1) / 5 + ul_year + ul_year / 4 - ul_year / 100 + ul_year / 400) % 7;

	++ul_week;

	return ul_week;
}

void epoch_to_time(uint32_t time, Time* timeptr)
{
	const uint16_t _ytab[2][12] = {
		{31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31},
		{31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31}
	};
	
	uint16_t year = EPOCH_YR;

	uint32_t dayclock = (uint32_t)time % 86400;
	uint32_t dayno = (uint32_t)time / 86400;

	uint16_t tm_sec = (uint16_t)(dayclock % 60);
	uint16_t tm_min = (uint16_t)((dayclock % 3600) / 60);
	uint16_t tm_hour = (uint16_t)(dayclock / 3600);
	uint16_t tm_wday = (uint16_t)((dayno + 4) % 7);       /* day 0 was a thursday */
	
	while (dayno >= YEARSIZE(year))
	{
		dayno -= YEARSIZE(year);
		year++;
	}
	
	uint16_t tm_year = year;
	//uint16_t tm_yday = (uint16_t)dayno;
	uint16_t tm_mon = 0;
	uint16_t leap_year = (int)(LEAPYEAR(year));
	
	while (dayno >= _ytab[leap_year][tm_mon])
	{
		dayno -= _ytab[leap_year][tm_mon];
		tm_mon++;
	}
	tm_mon += 1; //it is 0 based in original tm struct
	int tm_mday = (int)dayno + 1;
	
	timeptr->hour = tm_hour;
	timeptr->minute =tm_min;
	timeptr->second = tm_sec;
	timeptr->day = tm_mday;
	timeptr->month = tm_mon;
	timeptr->year = tm_year;
	timeptr->weekday = tm_wday;
}

uint16_t hm_convert_string_to_uint16(char* string)
{
	uint16_t temp_uint = 0;
	
	while((*string != 0) && (*string >= '0') && (*string <= '9'))
	{
		temp_uint = temp_uint * 10;
		temp_uint += (*string++) - '0';
	}
	
	return temp_uint;
}

/**
 * HardFault_HandlerAsm:
 * Alternative Hard Fault handler to help debug the reason for a fault.
 * To use, edit the vector table to reference this function in the HardFault vector
 * This code is suitable for Cortex-M3 and Cortex-M0 cores
 */

// Use the 'naked' attribute so that C stacking is not used.
__attribute__((naked))
void HardFault_Handler(void){
        /*
         * Get the appropriate stack pointer, depending on our mode,
         * and use it as the parameter to the C handler. This function
         * will never return
         */

        __asm(  ".syntax unified\n"
                        "MOVS   R0, #4  \n"
                        "MOV    R1, LR  \n"
                        "TST    R0, R1  \n"
                        "BEQ    _MSP    \n"
                        "MRS    R0, PSP \n"
                        "B      HardFault_HandlerC      \n"
                "_MSP:  \n"
                        "MRS    R0, MSP \n"
                        "B      HardFault_HandlerC      \n"
                ".syntax divided\n") ;
}

/**
 * HardFaultHandler_C:
 * This is called from the HardFault_HandlerAsm with a pointer the Fault stack
 * as the parameter. We can then read the values from the stack and place them
 * into local variables for ease of reading.
 * We then read the various Fault Status and Address Registers to help decode
 * cause of the fault.
 * The function ends with a BKPT instruction to force control back into the debugger
 */
void HardFault_HandlerC(unsigned long *hardfault_args){
        volatile unsigned long stacked_r0 ;
        volatile unsigned long stacked_r1 ;
        volatile unsigned long stacked_r2 ;
        volatile unsigned long stacked_r3 ;
        volatile unsigned long stacked_r12 ;
        volatile unsigned long stacked_lr ;
        volatile unsigned long stacked_pc ;
        volatile unsigned long stacked_psr ;
        volatile unsigned long _CFSR ;
        volatile unsigned long _HFSR ;
        volatile unsigned long _DFSR ;
        volatile unsigned long _AFSR ;
        volatile unsigned long _BFAR ;
        volatile unsigned long _MMAR ;

        stacked_r0 = ((unsigned long)hardfault_args[0]) ;
        stacked_r1 = ((unsigned long)hardfault_args[1]) ;
        stacked_r2 = ((unsigned long)hardfault_args[2]) ;
        stacked_r3 = ((unsigned long)hardfault_args[3]) ;
        stacked_r12 = ((unsigned long)hardfault_args[4]) ;
        stacked_lr = ((unsigned long)hardfault_args[5]) ;
        stacked_pc = ((unsigned long)hardfault_args[6]) ;
        stacked_psr = ((unsigned long)hardfault_args[7]) ;

        // Configurable Fault Status Register
        // Consists of MMSR, BFSR and UFSR
        _CFSR = (*((volatile unsigned long *)(0xE000ED28))) ;   
                                                                                        
        // Hard Fault Status Register
        _HFSR = (*((volatile unsigned long *)(0xE000ED2C))) ;

        // Debug Fault Status Register
        _DFSR = (*((volatile unsigned long *)(0xE000ED30))) ;

        // Auxiliary Fault Status Register
        _AFSR = (*((volatile unsigned long *)(0xE000ED3C))) ;

        // Read the Fault Address Registers. These may not contain valid values.
        // Check BFARVALID/MMARVALID to see if they are valid values
        // MemManage Fault Address Register
        _MMAR = (*((volatile unsigned long *)(0xE000ED34))) ;
        // Bus Fault Address Register
        _BFAR = (*((volatile unsigned long *)(0xE000ED38))) ;

        __asm("BKPT #0\n") ; // Break into the debugger
        while(1);
}