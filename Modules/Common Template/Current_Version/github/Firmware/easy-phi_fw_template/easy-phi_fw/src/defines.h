/*
 * defines.h
 *
 * Created: 18/10/2013 10:28:22
 *  Author: stephan
 */ 


#ifndef DEFINES_H_
#define DEFINES_H_

#include "asf.h"

#undef FALSE
#undef TRUE
#define FALSE	0
#define TRUE	!FALSE

/** Typedefs **/
typedef uint16_t RET_TYPE;
enum return_type {RETURN_NOK = 0, RETURN_OK, RETURN_NOT_READY};

typedef struct Time {
	uint16_t year;
	uint16_t month;
	uint16_t day;
	uint16_t hour;
	uint16_t minute;
	uint16_t second;
	uint16_t weekday;
}Time;

/** Macros **/
#define YEAR0                   1900
#define EPOCH_YR                1970
#define SECS_DAY                (24L * 60L * 60L)
#define LEAPYEAR(year)          (!((year) % 4) && (((year) % 100) || !((year) % 400)))
#define YEARSIZE(year)          (LEAPYEAR(year) ? 366 : 365)
#define FIRSTSUNDAY(timp)       (((timp)->tm_yday - (timp)->tm_wday + 420) % 7)
#define FIRSTDAYOF(timp)        (((timp)->tm_wday - (timp)->tm_yday + 420) % 7)


#endif /* DEFINES_H_ */