#ifndef PLAYER_H
#define PLAYER_H
#include <QPainter>
#include <QRect>
#include <QPixmap>
#include "assets.h"
#include "enemy.h"

enum class KirbyState {
    Normal,
    Inhaling,
    Mouthful_Enemy,
    Mouthful_Air,
    FireAbility,
    SparkAbility,
    Damaged,
    Dead
};

class Player
{
public:
    Player(int startX, int startY, int startWidth, int startHeight, int health);
    void draw(QPainter &painter, int cameraX) const;

    //-----------------------------------------------------------------
    //------------------------MOVEMENT SECTION------------------------//
    //------------------------MOVEMENT SECTION------------------------//
    //-----------------------------------------------------------------

    void move(int dx, int dy);
    void applyGravity();
    void jump();
    void fly();
    void releaseInAir();
    void setCrouched(bool crouched);
    QString getCurrentSpriteName() const;
    bool getIsGrounded() const;
    bool getIsCrouched() const;
    int getX() const;
    int getY() const;
    void setPosition(int newX, int newY);
    void applyGravity(int gravity);
    QRect getRect() const;
    void setY(int newY);
    void setX(int newX);
    void setVelocityY(float newVelocityY);
    float getVelocityY() const;
    void setVelocityX(float newVelocityX);
    void setGrounded(bool grounded);
    int getHeight() const;
    KirbyState getState() const;
    void setState(KirbyState newState);
    bool getIsHovering() const;
    void setHovering(bool hovering);
    void takeDamage(int damage);
    void heal(int amount);
    int getHealth() const;
    void setHealth(int newHealth);
    QRect getInhaleRect() const;
    int getInvincibleTimer() const;
    void updateInvincibility();
    void setInvincibleTimer(int newInvincibleTimer);
    void setFacingRight(bool newFacing);
    bool getFacingRight() const;
    void knockback(int forceX, int forceY);
    void applyHorizontalVelocity();
    void knockback(float forceX, float forceY);
    void setIsMoving(bool newIsMoving);
    void setSwallowedEnemyType(EnemyType newType);
    EnemyType getSwallowedEnemyType() const;
    void startAttackAnimation(int duration);
    void updateAttackAnimation();
    bool getIsAttacking() const;
    void stopAttackAnimation();

    void startSparkAttackHold();
    void updateSparkAttackHold();
    void stopSparkAttackHold();
    bool getIsChargingSpark() const;
    void updateAnimation(bool moving);
    void startPrepareAttack(int duration);
    std::vector<QRect> getFireAttackHitboxes() const;
    void drawFireAttack(QPainter& painter, int cameraX) const;

    void startFireAttackHold();
    void updateFireAttackHold();
    void stopFireAttackHold();




private:
    int x;
    int y;
    int width;
    int height;
    int health;

    float velocityY;
    float velocityX;
    bool isGrounded;
    bool isCrouched;

    int normalHeight;
    int crouchHeight;
    KirbyState currentState;
    bool isHovering;
    int invincibleTimer;
    bool canEnterHover;
    bool facingRight;
    bool isMoving;
    bool isAttacking;
    int attackAnimationTimer;
    EnemyType swallowedEnemyType;
    int animationCounter = 0;
    int runFrame = 0;
    bool isChargingSpark;
    int sparkFrameTimer;
    int attackFrameTimer;
    int prepareAttackTimer;
    int fireAttackFrameTimer;

    QString directionalSprite(const QString& right, const QString& left) const;
    QString getNormalSpriteName() const;
    QString getFireSpriteName() const;
    QString getSparkSpriteName() const;
};

#endif // PLAYER_H