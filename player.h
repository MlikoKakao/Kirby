#ifndef PLAYER_H
#define PLAYER_H
#include <QPainter>
#include <QRect>

class Player
{
public:
    Player(int startX, int startY, int startWidth, int startHeight);

    void move(int dx, int dy);
    void applyGravity();
    void jump();
    void setCrouched(bool crouched);
    void draw(QPainter &painter, int cameraX) const;
    bool getIsGrounded() const;
    int getX() const;
    int getY() const;
    void setPosition(int newX, int newY);
    void applyGravity(int gravity);
    QRect getRect() const;
    void setY(int newY);
    void setX(int newX);
    void setVelocityY(int newVelocityY);
    int getVelocityY() const;
    void setGrounded(bool grounded);
    int getHeight() const;

private:
    int x;
    int y;
    int width;
    int height;

    int velocityY;
    bool isGrounded;
    bool isCrouched;

    int normalHeight;
    int crouchHeight;

};

#endif // PLAYER_H