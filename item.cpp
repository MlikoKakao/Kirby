#include "item.h"

Item::Item(int startX, int startY, int startWidth, int startHeight, ItemType startType):
    x(startX),
    y(startY),
    width(startWidth),
    height(startHeight),
    type(startType)
{}

QRect Item::getRect() const { return QRect(x, y, width, height); }
ItemType Item::getType() const { return type; }

void Item::draw(QPainter &painter, int cameraX) const
{
    QString spriteName;

    if (type == ItemType::MaximTomato)
    {
        spriteName = "maxim_tomato";
    }
    else if (type == ItemType::OneUp)
    {
        spriteName = "1up";
    }

    painter.drawPixmap(
        x - cameraX,
        y,
        width,
        height,
        Assets::instance().getPixmap(spriteName));
}

