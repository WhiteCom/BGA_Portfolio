#include "Common.h"

#if 1
AppData* appData;

void callAppData()
{
	int length;
	char* buf = loadFile(APP_DATA_PATH, length);
	if (buf == NULL)
	{
		AppData* ad = new AppData;
		buf = (char*)ad;

		memset(ad, 0x00, sizeof(AppData));
		ad->eff = 1.0f;
		ad->bgm = 1.0f;

		// 408 + dummy = 512
		for (int i = 0; i < 10; i++)
		{
			 char* a = &ad->mapData[512*i];

			 char s[MAP_FILE_SIZE]; sprintf(s, "map/map%d.tile", i);
			 int len;
			 char* b = loadFile(s, len);
			 memcpy(a, b, len);
			 delete b;
		}
	}
	appData = (AppData*)buf;
}

void saveAppData()
{
	saveFile(APP_DATA_PATH, (char*)appData, sizeof(AppData));
}
#endif