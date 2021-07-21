#include "Common.h"
#include "Map.h"

#if 1
AppData* appData;

void loadMapData()
{
	int bufOff = 0;
	char* buf = NULL;

	for (int i = 0; i < MAP_NUM; i++)
	{
		const char* str = "map";
		char strMap[128];
		sprintf(strMap, "map/%s%d.tile", str, i);
		
		int tmpLen;
		char* tmpBuf = loadFile(strMap, tmpLen);

		if (buf == NULL)
			buf = new char[tmpLen * 9 + 1];
		memcpy(&buf[bufOff], tmpBuf, tmpLen);
		bufOff += tmpLen;
		buf[bufOff] = 0;
	}

	FILE* wp = fopen(APP_DATA_PATH, "wb");
	fwrite(buf, sizeof(char), bufOff, wp);
	fclose(wp);
}

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