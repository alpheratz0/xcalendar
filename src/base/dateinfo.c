#include <string.h>
#include <time.h>

#include "dateinfo.h"

static const char *mnames[] = {
	"Jan", "Feb", "Mar",
	"Apr", "May", "Jun",
	"Jul", "Aug", "Sep",
	"Oct", "Nov", "Dec"
};

extern dateinfo_t
dateinfo_from(u32 month, u32 year) {
	time_t ts = time(NULL);
	struct tm *now = localtime(&ts);
	dateinfo_t di = { 0 };

	if (year == 0 || ((now->tm_year + 1900) == (i32)(year) && now->tm_mon == (i32)(month))) {
		di.day = now->tm_mday;
		di.month = now->tm_mon;
		di.year = now->tm_year + 1900;
	}
	else {
		di.day = 0;
		di.month = month;
		di.year = year;
	}

	if (year != 0) {
		now->tm_year = (i32)(year) - 1900;
		now->tm_mon = month;
		now->tm_mday = 1;
		mktime(now);
	}

	di.firstday_weekday = ((now->tm_wday - now->tm_mday) % 7 + 8) % 7;

	strcpy(di.month_name, mnames[di.month]);

	switch (di.month) {
		case 0: case 2:
		case 4: case 6:
		case 7: case 9:
		case 11:
			di.num_days_in_month = 31;
			break;
		case 3: case 5:
		case 8: case 10:
			di.num_days_in_month = 30;
			break;
		case 1:
			di.num_days_in_month = 28;
			di.num_days_in_month += (di.year % 400 == 0 ||
					(di.year % 4 == 0 && di.year % 100 != 0));
			break;
	}

	return di;
}
