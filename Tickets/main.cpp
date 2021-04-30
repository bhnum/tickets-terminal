#include <iostream>
#include <iomanip>

#include "pages.h"

using namespace std;

int main()
{
	//std::ios_base::sync_with_stdio(false);
	//std::setvbuf(stdout, nullptr, _IOFBF, BUFSIZ);

	/*loadtrips();

	tm t1 = tm(); t1.tm_year = 2019 - 1900; t1.tm_mon = 3; t1.tm_mday = 23;
	tm t2 = tm(); t2.tm_year = 2019 - 1900; t2.tm_mon = 3; t2.tm_mday = 21;
	tm t3 = tm(); t3.tm_year = 2019 - 1900; t3.tm_mon = 3; t3.tm_mday = 14;

	time_t t_ = time(0);
	tm tm_;
	localtime_s(&tm_, &t_);

	definetrip({ 0,Bus,1,2,1,t1 });
	definetrip({ 0,Bus,1,2,1,t2 });
	definetrip({ 0,Train,1,2,1,t1 });
	definetrip({ 0,Train,1,2,1,t2 });
	definetrip({ 0,Bus,1,5,3,t2 });
	definetrip({ 0,Bus,3,2,1,t3 });

	vector<Trip> t = gettrips();
	t = filtertripsby(t, Bus);
	t = filtertripsfrom(t, 1);
	t = filtertripsto(t, 2);
	t = filtertripson(t, t1);

	return 0;*/

	newlayout("Loading...");
	showlayout();

	loadaccounts();
	loadusers();
	loadtrips();
	loadtickets();
	loadtariffs();
	loadvehicleinformation();
	loadwaitinglists();

	closelayout();

	showmainpage();

	//showadminpage(1);

	return 0;
}