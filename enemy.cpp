#include "enemy.h"
#include <QDebug>

Enemy::Enemy(int startX, int startY, int startWidth, int startHeight, int startSpeed):
    x(startX),
    y(startY),
    width(startWidth),
    height(startHeight),
    speed(startSpeed),
    direction(1)
{}

void Enemy::move(int speed)
{
    moveTimer++;
    if (moveTimer % 2 == 0)
    {
        x += speed * direction;
    }

    if (moveTimer > 120)
    {
        direction *= -1;
        moveTimer = 0;
    }

}

void Enemy::draw(QPainter &painter, int cameraX) const
{
    painter.setBrush(QColor(255, 0, 0));
    painter.drawRect(x - cameraX, y, width, height);
}