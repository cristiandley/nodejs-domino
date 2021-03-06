/*******************************************************************************
* Domino addon for Node.js
*
* Copyright (c) 2016 Nils T. Hjelme
*
* The MIT License (MIT)
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:

* The above copyright notice and this permission notice shall be included in all
* copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
*******************************************************************************/

#include "ItemValue.h"
#include <iostream>

void ItemValue::TextItem(LNItem *item) {
	LNText itemText = (LNText)*item;
	if (itemText.GetCount()>1) {
		vectorStrValue.clear();
		for (LNINT j = 0; j < itemText.GetCount(); j++) {
			LNString value = itemText[j];
			char buf[132000];
			LNStringTranslate(value, LNCHARSET_UTF8, 132000, buf);
			vectorStrValue.push_back(buf);
		}
		type = 2;
	} else {
		LNString value = itemText[0];
		char buf[132000];
		LNStringTranslate(value, LNCHARSET_UTF8, 132000, buf);
		type=1;
		stringValue = buf;
	}
}

ItemValue::ItemValue(LNItem *item) {
	LNITEMTYPE itemType = item->GetType();
	if (itemType == LNITEMTYPE_TEXT) {
		TextItem(item);
	}
	else if (itemType == LNITEMTYPE_NUMBERS) {

		LNNumbers tall = (LNNumbers)*item;
		LNNumber value = tall[0];
		type = 0;
		numberValue = value;
		//iv = new ItemValue(value);

	}
	else if (itemType == LNITEMTYPE_DATETIMES) {

		LNDatetimes dates = (LNDatetimes)*item;

		if (!dates.IsNull()) {
			//std::cout << "date item type: " << itemType << ", count= " << dates.GetCount()<< std::endl;

			LNDatetime date = dates[0];
			DateValue(&date);
		}



	}



}

void ItemValue::DateValue(LNDatetime * date) {
	try {
		LNINT day, year, month, hour, minute, second, hundredth;
		struct tm * timeinfo;

		date->GetDate(&month, &day, &year);
		if (date->IsTimeDefined()) {
			date->GetTime(&hour, &minute, &second, &hundredth);
		} else {
			hour = 0;
			minute = 0;
			second = 0;
			hundredth = 0;
		}

		time_t rawtime = time(0);
		timeinfo = localtime(&rawtime);
		timeinfo->tm_year = year - 1900;
		timeinfo->tm_mon = month - 1;
		timeinfo->tm_mday = day;

		timeinfo->tm_hour = hour - 1;
		timeinfo->tm_min = minute - 1;
		timeinfo->tm_sec = second - 1;
		type = 3;
		double dtime = mktime(timeinfo);

		dateTimeValue = dtime*1000; // convert to double time

	}
	catch (...) {
		std::cout << "error getting date from datetime" << std::endl;
	}
}
