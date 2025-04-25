#include "Player.h"
// #include "Game.h"
#include "D2DImage.h"
#include "D2DImageManager.h"
#include "TurnManager.h"
#include "KeyManager.h"
#include "Level.h"
#include "TimerManager.h"
#include "PathFinder.h"
#include "CombatSyetem.h"
#include "Item.h"
#include "Inventory.h"
#include "Animator.h"
#include "FModSoundPlayer.h"

Player::Player(FPOINT pos, float speed, int hp, int attDmg, int defense)
{
    position = pos;
    this->speed = speed;
    this->hp = 20;
    this->maxHp = 20;
    attackDmg = {1, 5};
    this->defense = 2;
    isMoving = false;

    isActive = true;
    level = 1;
    exp = 0;
    maxExp = 10;

    type = EntityType::PLAYER;
    curState = EntityState::IDLE;

    // 에너지 test
    actionCost = 10.f;
    energyPerTurn = 10.0f;

    //길찾기
    finder = new PathFinder();
    destPos = position;

    inven = new Inventory(this);

    image = D2DImageManager::GetInstance()->AddImage("warrior", L"Image/warrior.png", 21, 7);

    animator->AddClip("Idle", { 0,  1, 0.5f, true,  nullptr });
    animator->AddClip("Move", { 2,  8, 0.2f, true,  nullptr });
    animator->AddClip("Attack", { 13, 15, 0.1f, false, [this]() {
        // 공격 애니 끝나고 실행할 콜백
        if (target)
        {
            CombatSyetem::GetInstance()->ProcessAttack(this, target);
            Stop();
            SetState(EntityState::WAIT);
        }
    } });
    animator->AddClip("Dead", { 8, 12, 0.3f, false, nullptr });

    animator->Play("Idle");
}

Player::~Player()
{
}

void Player::SetStairs(const FPOINT& ascPos, const FPOINT& descPos)
{
    nowAscPos = ascPos;
    nowDescPos = descPos;
}

void Player::Update()
{
    Super::Update();
    switch (curState)
    {
    case EntityState::IDLE:
        return;
    case EntityState::MOVE:
        return;
    case EntityState::ATTACK:
        //if (curAnimFrame == endFrame)
        //    curState = EntityState::DUMMY;
        return;
    case EntityState::DEAD:
        // player는 죽은채로 계속 애니메이션 돼야함
        return;
    }
}

void Player::Render(HDC hdc)
{
    if (image)
        image->Middle_RenderFrameScale(position.x, position.y, 2.f, 2.f, curAnimFrame, 1);
}

void Player::Act(Level* level)
{
    switch (curState)
    {
    case EntityState::IDLE:
        ActIdle(level);
        return;
    case EntityState::MOVE:
        if (justMoved == true) {
            justMoved = false;
        }
        Move(level);
        return;
    case EntityState::ATTACK:
        Attack(level);
        return;
    case EntityState::DEAD:
        // player는 죽은채로 계속 애니메이션 돼야함
        return;
    case EntityState::WAIT:
        SetState(EntityState::IDLE);
        return;
        // 아이템때문에 고민중
    //case EntityState::USING_ITEM:
    //    UseItem()
    //    return;
    }
}

void Player::Attack(Level* level)
{
    //if (target)
    //{
    //    CombatSyetem::GetInstance()->ProcessAttack(this, target);
    //    Stop();

    //    //SetState(EntityState::IDLE);
    //}
}

void Player::ActIdle(Level* level)
{
    // 테스트용
    if (KeyManager::GetInstance()->IsOnceKeyDown('P'))
    {
        if (inven)
        {
            inven->UseItem("체력포션");
        }
    }

    if (position == destPos) { 
        if (destPos == nowAscPos && justMoved == false) {
            //목적지가 올라가는 계단이면 올라가는 함수 실행
            Ascending();
        }
        else if (destPos == nowDescPos && justMoved == false) {
            //목적지가 내려가는 계단이면 내려가는 함수 실행
            Descending();
        }
        return; 
    }
    if (finder->FindPath(position, destPos, level, OUT path))
        targetPos = path[1];

    target = level->GetActorAt(targetPos);
    if (target)
    {
        SetState(EntityState::ATTACK);
        return;
    }    
    if (!level->GetMap(targetPos.x, targetPos.y)->CanGo()) return;

    SetState(EntityState::MOVE);
}

void Player::GetItem(Item* item)
{
    if (inven)
    {
        inven->AddItem(item);
        entityObserver.NotifyChangePlayerInven(this);
    }
}

void Player::SetFunctions(function<void()> asc, function<void()> desc)
{
    Ascending = asc;
    Descending = desc;
}

void Player::Heal(int healAmount)
{
    hp += healAmount;
    if (hp >= maxHp) hp = maxHp;
    entityObserver.NotifyDamageTaken(this, -healAmount, D2D1::ColorF(D2D1::ColorF::White));
}

void Player::UseItem(Level* level)
{
    /*inven->Use(this, level);*/
}

void Player::SetState(EntityState state)
{
    switch (state)
    {
    case EntityState::IDLE:
        // SetAimData(0, 1, 2.0);
        curState = EntityState::IDLE;
        animator->Play("Idle");
        break;
    case EntityState::MOVE:
        // SetAimData(2, 8, 0.1);
        curState = EntityState::MOVE;
        animator->Play("Move");
        break;
    case EntityState::ATTACK:
        // SetAimData(13, 15, 0.1);
        curState = EntityState::ATTACK;
        animator->Play("Attack");
        break;
    case EntityState::DEAD:
        // SetAimData(8, 12, 0.3);
        curState = EntityState::DEAD;
        animator->Play("Dead");
        break;
    case EntityState::WAIT:
        curState = EntityState::WAIT;
        break;
    }

}

void Player::Move(Level* level)
{   
    //if (!level->GetMap(targetPos.x, targetPos.y)->CanGo())
    //{
    //    curState = EntityState::IDLE;
    //    return;
    //}
    FModSoundPlayer::GetInstance()->Play("step", 0.3f);
    FPOINT delta = targetPos - position;

    float deltaTime = TimerManager::GetInstance()->GetDeltaTime();
    delta.Normalize();

    position.x += speed * deltaTime * delta.x;
    position.y += speed * deltaTime * delta.y;

    delta = targetPos - position;

    // 매직넘버로,,, -> 변수로 dir 저장해두고 쓰면 Dot Product
    if (delta.Length() <= 10.f)
    {
        position = targetPos;
        SetState(EntityState::IDLE);
    }
}
