#include "Game.h"

#include "Tile.h"
#include "Character.h"
#include "Loading.h"

MapTile* t;

void loadGame()
{
    t = new MapTile();
}

void drawGame(float dt)
{
    Texture* bg = createImage("assets/map.jpg");

    setRGBA(0.5f, 0.5f, 0.5f, 1.0f);
    clearRect();
    drawImage(bg, 0, 0, TOP | LEFT);

    freeImage(bg);

    t->makeMap("boss");
    t->drawTile();

    static bool test = true;
    iPoint off = iPointZero;

    if (test)
    {
        test = false;
        if (off == iPointZero)
            off = iPointMake(64, 64); //초기씬에서
        else
        {
            //씬 전환시 워프좌표 이동해야함.
        }
        loadCharacter(off);
    }

    drawCharacter(dt, t);
}

void freeGame()
{
    delete t;
}

void keyGame(iKeyStat stat, iPoint p)
{
	//keyCharacter(stat, p);
	//return;
}