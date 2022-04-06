#ifndef __XCALENDAR_BASE_DATEINFO_H__
#define __XCALENDAR_BASE_DATEINFO_H__

#include "../util/numdef.h"

typedef struct dateinfo dateinfo_t;

struct dateinfo {
	u32 day;
	u32 month;
	u32 year;
	u32 firstday_weekday;
	u32 num_days_in_month;
	char month_name[20];
};

extern dateinfo_t
dateinfo_from_today(void);

#endif
