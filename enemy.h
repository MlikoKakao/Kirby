#ifndef ENEMY_H
#define ENEMY_H
#include <QPainter>

class Enemy
{
public:
    Enemy(int startX, int startY, int startWidth, int startHeight, int startSpeed);

    void move(int speed);
    void draw(QPainter &painter, int cameraX) const;

private:
    int x;
    int y;
    int width;
    int height;

    int moveTimer;
    int speed;
    int direction;
};

#endif // ENEMY_H
