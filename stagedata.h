#ifndef STAGEDATA_H
#define STAGEDATA_H

#include <vector>
#include <QRect>
#include <QString>
#include "enemy.h"
#include "item.h"

struct Platform
{
    int x;
    int y;
    int width;
    int height;
};

struct Block
{
    int x;
    int y;
    int width;
    int height;
};

struct TerrainSprite
{
    int x;
    int y;
    int width;
    int height;
    QString spriteName;
};

struct StageData
{
    int width;
    int playerStartX;
    int playerStartY;

    QRect portalRect;
    QRect goalRect;

    std::vector<Platform> platforms;
    std::vector<Block> blocks;
    std::vector<Block> floors;
    std::vector<Item> items;
    std::vector<Enemy> enemies;
    std::vector<TerrainSprite> terrainSprites;
};

StageData createStage1();
StageData createStage2();

#endif