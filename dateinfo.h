#ifndef __XCALENDAR_DATEINFO_H__
#define __XCALENDAR_DATEINFO_H__

#include "numdef.h"

struct dateinfo {
	u32     day;
	u32     month;
	u32     year;
	u32     firstday_weekday;
	u32     num_days_in_month;
	char    month_name[20];
};

typedef struct dateinfo dateinfo_t;

extern dateinfo_t
dateinfo_from_today(void);

#endif
