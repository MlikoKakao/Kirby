#include "projectile.h"
#include "assets.h"
#include <QColor>
#include <cmath>

Projectile::Projectile(int startX, int startY, int size, int speed, ProjectileType type, int damage)
    : x(startX),
    y(startY),
    startX(startX),
    size(size),
    speed(speed),
    type(type),
    damage(damage)
{}

void Projectile::move() { x += speed; }
int Projectile::getX() const { return x; }
int Projectile::getDamage() const { return damage; }
ProjectileType Projectile::getType() const { return type; }

void Projectile::draw(QPainter &painter, int cameraX) const
{
    if (type == ProjectileType::Star)
    {
        QString spriteName;

        if (speed > 0)
        {
            spriteName = "kirby_star_r";
        }
        else if (speed < 0)
        {
            spriteName = "kirby_star_l";
        }
        else
        {
            spriteName = "kirby_star";
        }

        painter.drawPixmap(
            x - cameraX,
            y - 50,
            size + 40,
            size + 35,
            Assets::instance().getPixmap(spriteName)
            );
    }
    else if (type == ProjectileType::EnemyFire)
    {
        painter.drawPixmap(
            x - cameraX,
            y - 20,
            size + 20,
            size + 20,
            Assets::instance().getPixmap("hot_head_fire")
            );
    }
    else if (type == ProjectileType::Air) // Air attack
    {
        painter.setBrush(QColor(200, 240, 255, 200));
        painter.drawEllipse(x - cameraX, y, size, size);
    }
}

QRect Projectile::getRect() const
{
    if (type == ProjectileType::Star)
    {
        return QRect(x, y - 50, size + 40, size + 35);
    }

    if (type == ProjectileType::EnemyFire)
    {
        return QRect(x, y - 20, size + 20, size + 20);
    }

    return QRect(x, y, size, size);
}

bool Projectile::shouldBeRemoved() const
{
    if (type == ProjectileType::Air)
    {
        int distanceTravelled = std::abs(x - startX);
        return distanceTravelled > 250;
    }

    return false;
}