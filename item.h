#ifndef ITEM_H
#define ITEM_H
#include <QPainter>
#include <QPixmap>
#include "assets.h"

enum class ItemType
{
    MaximTomato,
    OneUp
};

class Item
{
public:
    Item(int startX, int startY, int startWidth, int startHeight, ItemType startType);
    void draw(QPainter &painter, int cameraX) const;
    QRect getRect() const;

    ItemType getType() const;

private:
    int x;
    int y;
    int width;
    int height;

    ItemType type;
};

#endif // ITEM_H
