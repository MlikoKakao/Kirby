#ifndef ENEMY_H
#define ENEMY_H
#include <QPainter>
#include <QRandomGenerator>
#include <vector>
#include "assets.h"


enum class EnemyType
{
    WaddleDee,
    Gordo,
    HotHead,
    Sparky
};

enum class EnemyAIState
{
    Walking,
    PreparingAttack,
    Attacking
};

class Enemy
{
public:
    Enemy(int startX, int startY, int startWidth, int startHeight, int startSpeed, int startHealth, EnemyType startType);

    int getY() const;
    int getX() const;
    QRect getRect() const;
    float getVelocityY() const;
    void setX(int newX);
    void setY(int newY);
    void setVelocityY(float newVelocityY);
    void setGrounded(bool grounded);
    void setMoveTimer(int mt);

    void move(int speed);
    void draw(QPainter &painter, int cameraX) const;
    void moveToward(int targetX, int targetY, int speed);
    void takeDamage(int damage);
    int getHealth() const;
    void applyGravity(float gravity);

    EnemyType getType() const;
    bool canBeInhaled() const;
    bool canBeDestroyed() const;

    void updateAnimation();

    void updateAI(const QRect& playerRect);
    bool isAttacking() const;
    std::vector<QRect> getFireHitboxes() const;
    void drawFire(QPainter& painter, int cameraX) const;
    QRect getSparkHitbox() const;
    void drawSparkyAttack(QPainter& painter, int cameraX) const;
    bool shouldShootProjectile(const QRect& playerRect);
    int getDirection() const;
    void turnAround();


private:
    int x;
    int y;
    int width;
    int height;

    int speed;
    int health;

    EnemyType type;

    int moveTimer;
    int turnTime;
    int direction;
    float velocityY;
    bool isGrounded;

    int animationTimer;
    int animationFrame;

    int projectileCooldown;

    EnemyAIState aiState;
    int attackTimer;
    int attackCooldown;
    int prepareAttackTimer;

    int jumpCooldown;
    int attackAnimationTimer;

    QString getCurrentSpriteName() const;
    QString directionalSprite(const QString& right, const QString& left) const;
    QString getCurrentFireSpriteName() const;
};

#endif // ENEMY_H
