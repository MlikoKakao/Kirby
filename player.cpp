#include "player.h"
#include <QColor>

Player::Player(int startX, int startY, int startWidth, int startHeight):
    x(startX),
    y(startY),
    width(startWidth),
    height(startHeight),
    velocityY(0),
    isGrounded(false),
    isCrouched(false),
    normalHeight(startHeight),
    crouchHeight(startHeight - 20)

{}

void Player::move(int dx, int dy)
{
    x += dx;
    y += dy;
}

void Player::setPosition(int newX, int newY)
{
    x = newX;
    y = newY;
}

void Player::applyGravity(int gravity)
{
    velocityY += gravity;
    y += velocityY;
}

bool Player::getIsGrounded() const
{
    return isGrounded;
}

int Player::getX() const
{
    return x;
}

int Player::getY() const
{
    return y;
}

void Player::setCrouched(bool crouch)
{
    if (crouch && isGrounded && !isCrouched) {
        height = crouchHeight;
        y += normalHeight - crouchHeight;
        isCrouched = true;
    }

    if (!crouch && isCrouched) {
        height = normalHeight;
        y -= normalHeight - crouchHeight;
        isCrouched = false;
    }
}

void Player::draw(QPainter &painter, int cameraX) const
{
    painter.setBrush(QColor(255, 105, 180));
    painter.drawRect(x - cameraX, y, width, height);
}

void Player::jump()
{
    if (isGrounded)
    {
        velocityY = -20;
        isGrounded = false;
    }
}

QRect Player::getRect() const
{
    return QRect(x, y, width, height);
}

void Player::setY(int newY)
{
    y = newY;
}

void Player::setX(int newX)
{
    x = newX;
}

void Player::setVelocityY(int newVelocityY)
{
    velocityY = newVelocityY;
}

int Player::getVelocityY() const
{
    return velocityY;
}

void Player::setGrounded(bool grounded)
{
    isGrounded = grounded;
}
