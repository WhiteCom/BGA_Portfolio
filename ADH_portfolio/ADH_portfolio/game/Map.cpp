#include "Map.h"

#include "iStd.h"
#include "iWindow.h"
#include "TileType.h"

#include "Common.h"

Map::Map()
{
    tileX = 0;
    tileY = 0;
    tileWidth = 0;
    tileHeight = 0;
    tileIndex = NULL;
    tileWeight = NULL;
    tileEnemy = NULL;

    selectedTile = 0;
    selectedWeight = 0;
}

Map::Map(const char* szFormat, ...)
{
    tileX = 0;
    tileY = 0;
    tileWidth = 0;
    tileHeight = 0;
    tileIndex = NULL;
    tileWeight = NULL;
    tileEnemy = NULL;

    selectedTile = 0;
    selectedWeight = 0;

    char szText[1024];
    va_start_end(szText, szFormat);

    load(szText);
}

Map::~Map()
{
    clean();
}

void Map::clean()
{
    if (tileIndex == NULL)
        return;
    
    delete tileIndex;
    delete tileWeight;

    if (tileEnemy == NULL)
        return;

    for (int i = 0; i < tileEnemy->eTypeNum; i++)
        delete tileEnemy[i].eType;
    delete tileEnemy;
}

void Map::draw(iPoint off, Texture** texs)
{
    int i, j, xy = tileX * tileY;
    int ti, tw;

    for (i = 0; i < xy; i++)
    {
        if (tileIndex[i] > -1)
        {
            iPoint _off = iPointMake(off.x + i % tileX * TILE_WSIZE, off.y + i / tileX * TILE_HSIZE);
            drawImage(texs[tileIndex[i]], _off.x, _off.y, TOP | LEFT);
        }
    }
}

void Map::init(int x, int y, int w, int h)
{
    tileX = x;
    tileY = y;
    tileWidth = w;
    tileHeight = h;

    int tileXY = tileX * tileY;
    int i;

    tileIndex = new int[tileXY];
    tileWeight = new int[tileXY]; 
    tileEnemy = new enemyInfo[tileXY];
    

    for (i = 0; i < tileXY; i++)
    {
        tileIndex[i] = -1;
        tileWeight[i] = -1;

        tileEnemy[i].eTypeNum = -1;
        if (tileEnemy[i].eTypeNum > 0)
        {
            tileEnemy[i].eType = new enemyType[tileEnemy[i].eTypeNum];
            for (int j = 0; j < tileEnemy[i].eTypeNum; j++)
            {
                tileEnemy[i].eType[j] = {
                    -1, -1 // type, num
                };
            }
        }
    }

    selectedTile = -1;
    selectedWeight = -1;
}

void Map::load(const char* szFormat, ...)
{
    clean();

    char szText[1024];
    va_start_end(szText, szFormat);

    FILE* pf = fopen(szText, "rb");
    if (pf == NULL)
    {
        printf("No File!");
        return;
    }

    // tile info
    fread(&tileX, sizeof(int), 1, pf);
    fread(&tileY, sizeof(int), 1, pf);
    fread(&tileWidth, sizeof(int), 1, pf);
    fread(&tileHeight, sizeof(int), 1, pf);

    int xy = tileX * tileY;
    int i;

    tileIndex = new int[xy];
    fread(tileIndex, sizeof(int), xy, pf);
    tileWeight = new int[xy];
    fread(tileWeight, sizeof(int), xy, pf);
    tileEnemy = new enemyInfo[xy];
    fread(tileEnemy, sizeof(enemyInfo), xy, pf);


    fclose(pf);
}

void Map::save(const char* str, ...)
{
    FILE* pf = fopen(str, "wb");

    // tile info
    fwrite(&tileX, sizeof(int), 1, pf);
    fwrite(&tileY, sizeof(int), 1, pf);
    fwrite(&tileWidth, sizeof(int), 1, pf);
    fwrite(&tileHeight, sizeof(int), 1, pf);

    int xy = tileX * tileY;

    fwrite(tileIndex, sizeof(int), xy, pf);
    fwrite(tileWeight, sizeof(int), xy, pf);
    fwrite(tileEnemy, sizeof(enemyInfo), xy, pf);

    fclose(pf);
}

#if 1
void Map::loadA(char* buf)
{
    memcpy(&tileX, &buf[0], sizeof(int));
    memcpy(&tileY, &buf[4], sizeof(int));
    memcpy(&tileWidth, &buf[8], sizeof(int));
    memcpy(&tileHeight, &buf[12], sizeof(int));

    int xy = tileX * tileY;
    int off = 16;

    memcpy(tileIndex, &buf[off], sizeof(int) * xy);
    off += sizeof(int) * xy;
    memcpy(tileWeight, &buf[off], sizeof(int) * xy);
    off += sizeof(int) * xy;
    memcpy(tileEnemy, &buf[off], sizeof(enemyInfo) * xy);
}

void Map::saveA(char* buf)
{
    memcpy(&buf[0], &tileX, sizeof(int));
    memcpy(&buf[4], &tileY, sizeof(int));
    memcpy(&buf[8], &tileWidth, sizeof(int));
    memcpy(&buf[12], &tileHeight, sizeof(int));

    int xy = tileX * tileY;
    int off = 16;

    memcpy(&buf[off], tileIndex, sizeof(int) * xy);
    off += sizeof(int) * xy;
    memcpy(&buf[off], tileWeight, sizeof(int) * xy);
    off += sizeof(int) * xy;
    memcpy(&buf[off], tileEnemy, sizeof(enemyInfo) * xy);

    saveAppData();
}
#endif

void Map::insert(iPoint point, int type)
{
    int x = point.x; x /= tileWidth;
    int y = point.y; y /= tileHeight;
    int xy = tileX * y + x;

    if (selectedTile > -1)
        tileIndex[xy] = selectedTile;
    if (selectedWeight > -1)
    {
        tileWeight[xy] = selectedWeight;
    }
    
    //EnemyType
    
    //#need update!
    if (tileEnemy)
    {
        for (int i = 0; i < tileX * tileY; i++)
        {
            if (tileWeight[i] < 9)
            {
                //create Enemy;
                tileEnemy[i].eTypeNum = random() % 4;
                tileEnemy[i].eType = new enemyType[tileEnemy->eTypeNum];

                tileEnemy[i].eType->type = type + random() % 2;


            }
        }
    }
    
}

