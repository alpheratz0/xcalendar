#include <string.h>
#include <time.h>
#include "dateinfo.h"

static const char *mnames[] = {
	"January", "February", "March", "April", "May", "June", "July", "August",
	"September", "October", "November", "December"
};

extern dateinfo_t
dateinfo_from_today(void) {
	time_t ts = time(NULL);
    struct tm *now = localtime(&ts);
	dateinfo_t di_today = { 0 };

	di_today.day = now->tm_mday;
	di_today.month = now->tm_mon;
	di_today.year = now->tm_year + 1900;
	di_today.firstday_weekday = ((now->tm_wday - now->tm_mday) % 7 + 8) % 7;

	strcpy(di_today.month_name, mnames[di_today.month]);

	switch (di_today.month) {
		case 0: case 2: case 4: case 6: case 7: case 9: case 11:
			di_today.num_days_in_month = 31;
			break;
		case 3: case 5: case 8: case 10:
			di_today.num_days_in_month = 30;
			break;
		case 1:
			di_today.num_days_in_month = 28;
			di_today.num_days_in_month += (di_today.year % 400 == 0 ||
					(di_today.year % 4 == 0 && di_today.year % 100 != 0));
			break;
	}

	return di_today;
}
