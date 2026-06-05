#include "player.h"
#include <QColor>

Player::Player(int startX, int startY, int startWidth, int startHeight, int startHealth)
    : x(startX),
    y(startY),
    width(startWidth),
    height(startHeight),
    health(startHealth),
    velocityY(0.0f),
    velocityX(0.0f),
    isGrounded(false),
    isCrouched(false),
    normalHeight(startHeight),
    crouchHeight(startHeight - 20),
    currentState(KirbyState::Normal),
    isHovering(false),
    invincibleTimer(0),
    canEnterHover(false),

    facingRight(true),
    isMoving(false),
    isAttacking(false),
    attackAnimationTimer(0),
    swallowedEnemyType(EnemyType::WaddleDee),
    isChargingSpark(false),
    attackFrameTimer(0),
    prepareAttackTimer(0),
    fireAttackFrameTimer(0)

{}
KirbyState Player::getState() const { return currentState; }

bool Player::getIsGrounded() const { return isGrounded; }
bool Player::getIsCrouched() const { return isCrouched; }
int Player::getX() const { return x; }
int Player::getY() const { return y; }
QRect Player::getRect() const { return QRect(x, y, width, height); }
float Player::getVelocityY() const { return velocityY; }
int Player::getHealth() const { return health; }
int Player::getInvincibleTimer() const { return invincibleTimer; }
bool Player::getFacingRight() const { return facingRight; }
EnemyType Player::getSwallowedEnemyType() const { return swallowedEnemyType; }
bool Player::getIsAttacking() const { return isAttacking; }
bool Player::getIsChargingSpark() const { return isChargingSpark; }

void Player::setPosition(int newX, int newY) { x = newX; y = newY; }
void Player::setInvincibleTimer(int newInvincibleTimer) { invincibleTimer = newInvincibleTimer; }
void Player::setY(int newY) { y = newY; }
void Player::setX(int newX) { x = newX; }
void Player::setVelocityY(float newVelocityY) { velocityY = newVelocityY; }
void Player::setVelocityX(float newVelocityX) { velocityX = newVelocityX; }
void Player::setState(KirbyState newState) { currentState = newState; }
bool Player::getIsHovering() const { return isHovering; }
void Player::setHovering(bool hovering) { isHovering = hovering; }
void Player::setIsMoving(bool newIsMoving) { isMoving = newIsMoving; }
void Player::setFacingRight(bool newFacing) { facingRight = newFacing; }
void Player::setHealth(int newHealth) { health = newHealth; }
void Player::setSwallowedEnemyType(EnemyType newType) { swallowedEnemyType = newType; }
// Getters/setters
// Really helps avoid the messiness that was here before

void Player::takeDamage(int damage) { health -= damage; }
void Player::heal(int amount) { health += amount; }
void Player::updateInvincibility() { if (invincibleTimer > 0) { invincibleTimer--; } }
QString Player::directionalSprite(const QString& right, const QString& left) const { return facingRight ? right : left; }
void Player::releaseInAir() { canEnterHover = true; }

// These seem more complex but obvious from name what they do

void Player::move(int dx, int dy)
{
    x += dx;
    y += dy;
}

void Player::applyGravity(int gravity)
{
    const int maxFallSpeed = 20;

    velocityY += gravity;

    if (velocityY > maxFallSpeed)
    {
        velocityY = maxFallSpeed;
    }

    y += velocityY;
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
    bool shouldDraw = true;

    if (invincibleTimer > 0)
    {
        shouldDraw = (invincibleTimer / 5) % 2 == 0;
    }

    if (!shouldDraw)
    {
        return;
    }

    QString spriteName = getCurrentSpriteName();

    if (currentState == KirbyState::SparkAbility)
    {
        if (isAttacking)
        {
            int drawWidth = width + 190;
            int drawHeight = height + 190;

            int drawX = x + width / 2 - drawWidth / 2;
            int drawY = y + height / 2 - drawHeight / 2;

            painter.drawPixmap(
                drawX - cameraX,
                drawY,
                drawWidth,
                drawHeight,
                Assets::instance().getPixmap(spriteName)
                );
            return;
        }
        else if (isChargingSpark)
        {
            int drawX = x + width / 2 - width / 2;
            int drawY = y + height / 2 - height / 2;

            painter.drawPixmap(
                drawX - cameraX,
                drawY,
                width,
                height,
                Assets::instance().getPixmap(spriteName)
                );
            return;
        }
        int drawWidth = width + 20;
        int drawHeight = height + 35;

        int drawX = x - 15;
        int drawY = y - 35;

        painter.drawPixmap(
            drawX - cameraX,
            drawY,
            drawWidth,
            drawHeight,
            Assets::instance().getPixmap(spriteName)
            );

        return;
    }

    if (currentState == KirbyState::FireAbility)
    {
        int drawWidth = width + 30;
        int drawHeight = height + 30;

        int drawX = x - 15;
        int drawY = y - 25;

        painter.drawPixmap(
            drawX - cameraX,
            drawY,
            drawWidth,
            drawHeight,
            Assets::instance().getPixmap(spriteName)
            );

        return;
    }

    painter.drawPixmap(
        x - cameraX,
        y,
        width,
        height,
        Assets::instance().getPixmap(spriteName)
        );
}

QString Player::getCurrentSpriteName() const
{
    if (currentState == KirbyState::Inhaling)
    {
        return directionalSprite("kirby_attack_right", "kirby_attack_left");
    }

    if (currentState == KirbyState::FireAbility)
    {
        return getFireSpriteName();
    }

    if (currentState == KirbyState::SparkAbility)
    {
        return getSparkSpriteName();
    }

    return getNormalSpriteName();
}

void Player::jump()
{
    if (currentState == KirbyState::Mouthful_Enemy) return;

    if (isGrounded)
    {
        velocityY = -25;
        isGrounded = false;
        isHovering = false;
        canEnterHover = false;
    }
}

void Player::fly()
{
    if (currentState == KirbyState::Mouthful_Enemy) return;
    if (!canEnterHover) return;
    isHovering = true;

    if (velocityY > -10.0f)
    {
        velocityY -= 2.0f;
    }
}
void Player::setGrounded(bool grounded)
{
    isGrounded = grounded;
    if (isGrounded)
    {
        isHovering = false;
        canEnterHover = false;
    }
}

QRect Player::getInhaleRect() const
{
    int inhaleWidth = 160;
    int inhaleHeight = height + 50;

    if (facingRight) {
        return QRect(x + width, y - 50, inhaleWidth, inhaleHeight);
    } else {
        return QRect(x - inhaleWidth, y - 50, inhaleWidth, inhaleHeight);
    }
}

// This is needed for fluent knockback instead of having teleporting Kirby
void Player::applyHorizontalVelocity()
{
    x += velocityX;

    velocityX *= 0.8f;

    if (velocityX > -0.1f && velocityX < 0.1f) {
        velocityX = 0;
    }
}

void Player::knockback(float forceX, float forceY)
{
    velocityX = forceX;
    velocityY = forceY;
}

void Player::startAttackAnimation(int duration)
{
    isAttacking = true;
    attackAnimationTimer = duration;
    attackFrameTimer = 0;
}

void Player::updateAttackAnimation()
{
    if (currentState == KirbyState::SparkAbility &&
        (isChargingSpark || isAttacking))
    {
        return;
    }

    if (currentState == KirbyState::FireAbility && isAttacking)
    {
        return;
    }

    if (attackAnimationTimer > 0)
    {
        attackAnimationTimer--;
    }

    if (attackAnimationTimer <= 0)
    {
        isAttacking = false;
    }
}

void Player::updateAnimation(bool moving)
{
    isMoving = moving;

    if (!isMoving)
    {
        animationCounter = 0;
        runFrame = 0;
        return;
    }

    animationCounter++;

    if (animationCounter >= 8)
    {
        animationCounter = 0;

        if (currentState == KirbyState::SparkAbility)
        {
            runFrame = (runFrame + 1) % 2;
        }
        else
        {
            runFrame = (runFrame + 1) % 4;
        }
    }
}

QString Player::getNormalSpriteName() const
{
    if (isHovering)
    {
        if (velocityY > 0)
        {
            return directionalSprite("kirby_fly1_r", "kirby_fly1_l");
        }

        return directionalSprite("kirby_fly2_r", "kirby_fly2_l");
    }

    if (!isGrounded)
    {
        if (velocityY < -8)
        {
            return directionalSprite("kirby_jump1_r", "kirby_jump1_l");
        }

        if (velocityY < 2)
        {
            return directionalSprite("kirby_jump2", "kirby_jump2_l");
        }

        return directionalSprite("kirby_jump3", "kirby_jump3_l");
    }

    if (isCrouched)
    {
        return directionalSprite("kirby_crouch_r", "kirby_crouch_l");
    }

    if (isMoving)
    {
        if (runFrame == 0)
        {
            return directionalSprite("kirby_run1_right", "kirby_run1_left");
        }

        if (runFrame == 1)
        {
            return directionalSprite("kirby_run2_right", "kirby_run2_left");
        }

        if (runFrame == 2)
        {
            return directionalSprite("kirby_run3_right", "kirby_run3_left");
        }

        return directionalSprite("kirby_run4_right", "kirby_run4_left");
    }

    return directionalSprite("kirby_idle_right", "kirby_idle_left");
}

QString Player::getFireSpriteName() const
{
    if (isAttacking)
    {
        return directionalSprite(
            "kirby_fire_attack_r",
            "kirby_fire_attack_l"
            );
    }

    if (isHovering)
    {
        if (velocityY > 0)
        {
            return directionalSprite(
                "kirby_fire_fly1_r",
                "kirby_fire_fly1_l"
                );
        }

        return directionalSprite(
            "kirby_fire_fly2_r",
            "kirby_fire_fly2_l"
            );
    }

    if (!isGrounded)
    {
        return directionalSprite(
            "kirby_fire_fly2_r",
            "kirby_fire_fly2_l"
            );
    }

    if (isCrouched)
    {
        return directionalSprite(
            "kirby_fire_down_r",
            "kirby_fire_down_l"
            );
    }

    if (isMoving)
    {
        if (runFrame == 0)
        {
            return directionalSprite(
                "kirby_fire_run1_r",
                "kirby_fire_run1_l"
                );
        }

        if (runFrame == 1)
        {
            return directionalSprite(
                "kirby_fire_run2_r",
                "kirby_fire_run2_l"
                );
        }

        return directionalSprite(
            "kirby_fire_run3_r",
            "kirby_fire_run3_l"
            );
    }

    return directionalSprite(
        "kirby_fire_idle_r",
        "kirby_fire_idle_l"
        );
}

QString Player::getSparkSpriteName() const
{
    if (isChargingSpark)
    {
        return "kirby_spark_attack1";
    }

    if (isAttacking)
    {
        int frame = (attackFrameTimer / 8) % 2;

        if (frame == 0)
        {
            return "kirby_spark_attack2";
        }

        return "kirby_spark_attack3";
    }

    if (isHovering)
    {
        if (velocityY > 0)
        {
            return directionalSprite(
                "kirby_spark_fly1_r",
                "kirby_spark_fly1_l"
                );
        }

        return directionalSprite(
            "kirby_spark_fly2_r",
            "kirby_spark_fly2_l"
            );
    }

    if (!isGrounded)
    {
        return directionalSprite(
            "kirby_spark_fly2_r",
            "kirby_spark_fly2_l"
            );
    }

    if (isCrouched)
    {
        return directionalSprite(
            "kirby_spark_down_r",
            "kirby_spark_down_l"
            );
    }

    if (isMoving)
    {
        if (runFrame == 0)
        {
            return directionalSprite(
                "kirby_spark_run1_r",
                "kirby_spark_run1_l"
                );
        }

        return directionalSprite(
            "kirby_spark_run2_r",
            "kirby_spark_run2_l"
            );
    }

    return directionalSprite(
        "kirby_spark_idle_r",
        "kirby_spark_idle_l"
        );
}

void Player::startPrepareAttack(int duration)
{
    prepareAttackTimer = duration;
    isAttacking = false;
    attackAnimationTimer = 0;
    attackFrameTimer = 0;
}


void Player::stopAttackAnimation()
{
    isAttacking = false;
    isChargingSpark = false;
    attackAnimationTimer = 0;
    attackFrameTimer = 0;
    fireAttackFrameTimer = 0;
}

void Player::startSparkAttackHold()
{
    if (currentState != KirbyState::SparkAbility)
    {
        return;
    }

    isChargingSpark = true;
    isAttacking = false;
    attackFrameTimer = 0;
    attackAnimationTimer = 0;
}

void Player::updateSparkAttackHold()
{
    if (currentState != KirbyState::SparkAbility)
    {
        return;
    }

    if (!isChargingSpark && !isAttacking)
    {
        return;
    }

    attackFrameTimer++;

    const int chargeFrames = 12;

    if (isChargingSpark && attackFrameTimer >= chargeFrames)
    {
        isChargingSpark = false;
        isAttacking = true;
    }
}

void Player::stopSparkAttackHold()
{
    isChargingSpark = false;
    isAttacking = false;
    attackFrameTimer = 0;
    attackAnimationTimer = 0;
}

std::vector<QRect> Player::getFireAttackHitboxes() const
{
    std::vector<QRect> hitboxes;

    if (currentState != KirbyState::FireAbility || !isAttacking)
    {
        return hitboxes;
    }

    int kirbyFrontX = facingRight
                          ? x + width
                          : x;

    int kirbyCenterY = y + height / 2;

    QRect closeFire(
        facingRight ? kirbyFrontX : kirbyFrontX - 90,
        kirbyCenterY - 60,
        90,
        120
        );

    QRect middleFire(
        facingRight ? kirbyFrontX + 70 : kirbyFrontX - 170,
        kirbyCenterY - 45,
        100,
        90
        );

    QRect farFire(
        facingRight ? kirbyFrontX + 150 : kirbyFrontX - 240,
        kirbyCenterY - 30,
        90,
        60
        );

    hitboxes.push_back(closeFire);
    hitboxes.push_back(middleFire);
    hitboxes.push_back(farFire);

    return hitboxes;
}

void Player::drawFireAttack(QPainter& painter, int cameraX) const
{
    if (currentState != KirbyState::FireAbility || !isAttacking)
    {
        return;
    }

    int fireWidth = 240;
    int fireHeight = 130;

    int fireX;

    if (facingRight)
    {
        fireX = x + width - 20;
    }
    else
    {
        fireX = x - fireWidth + 20;
    }

    int fireY = y + height / 2 - fireHeight / 2;

    int frame = (fireAttackFrameTimer / 8) % 3;

    QString spriteName;

    if (frame == 0)
    {
        spriteName = directionalSprite(
            "kirbyfire_fire1_right",
            "kirbyfire_fire1_left"
            );
    }
    else if (frame == 1)
    {
        spriteName = directionalSprite(
            "kirbyfire_fire2_right",
            "kirbyfire_fire2_left"
            );
    }
    else
    {
        spriteName = directionalSprite(
            "kirbyfire_fire3_right",
            "kirbyfire_fire3_left"
            );
    }

    painter.drawPixmap(
        fireX - cameraX,
        fireY,
        fireWidth,
        fireHeight,
        Assets::instance().getPixmap(spriteName)
        );
}

void Player::startFireAttackHold()
{
    if (currentState != KirbyState::FireAbility)
    {
        return;
    }

    if (isAttacking)
    {
        return;
    }

    isAttacking = true;
    attackAnimationTimer = 0;
    fireAttackFrameTimer = 0;
}

void Player::updateFireAttackHold()
{
    if (currentState != KirbyState::FireAbility)
    {
        return;
    }

    if (!isAttacking)
    {
        return;
    }

    fireAttackFrameTimer++;
}

void Player::stopFireAttackHold()
{
    if (currentState == KirbyState::FireAbility)
    {
        isAttacking = false;
    }

    fireAttackFrameTimer = 0;
    attackAnimationTimer = 0;
}