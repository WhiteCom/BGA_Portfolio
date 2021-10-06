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

#if 1 //#openAL
	ad->eff = 1.0f;
	ad->bgm = 1.0f;
#endif

	appData = ad;
}

void callMapData()
{
	AppData* ad = new AppData();

#if 1 //#openAL
	ad->eff = appData->eff;
	ad->bgm = appData->bgm;
#endif
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
		a[len] = 0;

		bufOff += MAP_FILE_SIZE;
		//xprint("a : %s\n", a);

		delete b;
	}
	appData = ad;
	//xprint("Appdata->mapdata : %s\n", appData->mapData);
}

void saveAppData()
{
	saveFile(APP_DATA_PATH, (char*)appData, sizeof(AppData));
}
#endif //#need update! chanage file format no use save.dat

void freeAppData()
{
	delete appData;
}

#if 0 //Test File I/O
struct AAA
{
	int len;
	char* buf;
};
void prevCode(const char* fileName)
{
	AAA a;

	FILE* pf = fopen(fileName, "rb");
	fread(&a.len, 1, sizeof(int), pf);
	a.buf = new char[a.len];
	fread(a.buf, 1, sizeof(char)* a.len, pf);
	fclose(pf);
}

void currCode(const char* fileName)
{
	int len;
	char* buf = getStream(fileName, len);
	int off = 0;

	AAA a;
	memcpy(&a.len, &buf[off], sizeof(int)); off += sizeof(int);
	a.buf = new char[a.len];
	memcpy(a.buf, &buf[off], sizeof(char)*a.len); off+=sizeof(char)*a.len;

}
#endif




