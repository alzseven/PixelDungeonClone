#include "Entity.h"
#include "Level.h"
#include "D2DImage.h"
#include "D2DImageManager.h"
#include "TimerManager.h"
#include "UIManager.h"
#include "Animator.h"

Entity::Entity()
{
	image = D2DImageManager::GetInstance()->AddImage("warrior", L"Image/warrior.png", 21, 7);
	//image = D2DImageManager::GetInstance()->AddImage("entityTest", L"Image/rat.png", 16, 3);
	isActive = true;
	animator = new Animator;
}

Entity::~Entity()
{
	entityObserver.NotifySystemRelese(this);
}

void Entity::Update()
{
	//curTime += TimerManager::GetInstance()->GetDeltaTime();
	//if (curTime > maxAnimTime)
	//{
	//	curAnimFrame++;
	//	curTime = 0;
	//	if (!stayEndFrame)
	//	{
	//		if (curAnimFrame > endFrame)
	//		{
	//			curAnimFrame = startFrame;
	//		}
	//	}
	//	else if (curAnimFrame > endFrame)
	//	{
	//		curAnimFrame = endFrame;
	//		return;
	//	}
	//	
	//}
	float delta = TimerManager::GetInstance()->GetDeltaTime();
	animator->Update(delta);
	curAnimFrame = animator->GetCurFrame();
}

void Entity::Render(HDC hdc)
{
	image->Middle_RenderFrameScale(position.x, position.y, 2, 2, curAnimFrame, 1 );
}

void Entity::Act(Level* level)
{
}

void Entity::TakeDamage(int dmg)
{
	hp -= dmg;
	Stop();
	if (type == EntityType::PLAYER)
	{
		entityObserver.NotifyDamageTaken(this, dmg, D2D1::ColorF(D2D1::ColorF::Red));
	}
	else
	{
		entityObserver.NotifyDamageTaken(this, dmg, D2D1::ColorF(D2D1::ColorF::White));
	}
	if (hp <= 0)
	{
		hp = 0;
		//isActive = false;
		curState = EntityState::DEAD; // 몬스터 죽은거 확인용
		SetState(EntityState::DEAD);
		entityObserver.NotifyDeath(this);
	}
}

void Entity::TakeExp(int exp)
{
	this->exp += exp;
	if (this->exp / level == 20)
	{
		this->exp = this->exp % level;
		LevelUp();
	}
}

void Entity::LevelUp()
{
	level++;
	attackDmg.min += 1;
	attackDmg.min += 1;
	maxHp += 5;
	hp += 5;
}

void Entity::SetState(EntityState state)
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
	case EntityState::SLEEP:
		curState = EntityState::SLEEP;
		break;
	}

}
