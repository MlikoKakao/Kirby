#ifndef PROJECTILE_H
#define PROJECTILE_H

#include <QPainter>
#include <QRect>

enum class ProjectileType
{
    Star,
    EnemyFire,
    Air
};

class Projectile
{
public:
    Projectile(int startX, int startY, int size, int speed, ProjectileType type, int damage);

    void move();
    int getX() const;
    void draw(QPainter &painter, int cameraX) const;
    QRect getRect() const;

    ProjectileType getType() const;
    int getDamage() const;
    bool shouldBeRemoved() const;

private:
    int x;
    int y;
    int startX; // Air should only travel short distance
    int size;
    int speed;
    ProjectileType type;
    int damage;
};

#endif // PROJECTILE_H
