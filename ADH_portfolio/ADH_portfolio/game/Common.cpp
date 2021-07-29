#include "Common.h"
#include "Map.h"

//============================================
// AppData
//============================================

AppData::AppData()
{
	eff = 0.0f;
	bgm = 0.0f;

	mapData = new char[MAP_FILE_SIZE * MAP_NUM + 1];
}

AppData::~AppData()
{
	delete mapData;
}

#if 1 //#need update! chanage file format no use save.dat
AppData* appData;

void loadMapData()
{
	int bufOff = 0;
	char* buf = NULL;

	for (int i = 0; i < MAP_NUM; i++)
	{
		char strMap[128];
		sprintf(strMap, "map/map%d.tile", i);
		
		int tmpLen;
		char* tmpBuf = loadFile(strMap, tmpLen);

		if (buf == NULL)
			buf = new char[tmpLen * 9 + 1];
		memcpy(&buf[bufOff], tmpBuf, tmpLen);
		bufOff += tmpLen;
		buf[bufOff] = 0;
		delete tmpBuf;
	}

	FILE* wp = fopen(APP_DATA_PATH, "wb");
	fwrite(buf, sizeof(char), bufOff, wp);
	fclose(wp);
}

void callAppData()
{
	AppData* ad = new AppData();
	char* buf = (char*)ad;

	ad->eff = 1.0f;
	ad->bgm = 1.0f;
	memset(ad->mapData, 0x00, sizeof(MAP_FILE_SIZE * MAP_NUM));

	int bufOff = 0;
	// 3088 + dummy = MAP_FILE_SIZE
	for (int i = 0; i < MAP_NUM; i++)
	{
		char* a;
		a = &ad->mapData[MAP_FILE_SIZE * i];

		 char s[MAP_FILE_SIZE]; sprintf(s, "map/map%d.tile", i);
		 int len;
		 char* b = loadFile(s, len);
		 memcpy(a, b, len);
		 //strcat(a, b);
		 a[len] = 0;

		 bufOff += MAP_FILE_SIZE;
		 //a[bufOff-1] = 0;
		 printf("a : %s\n", a);
		 delete b;
	}
	appData = ad;

	printf("Appdata->mapdata : %s\n", appData->mapData);
}

void saveAppData()
{
	saveFile(APP_DATA_PATH, (char*)appData, sizeof(AppData));
}
#endif

void freeAppData()
{
	delete appData;
}