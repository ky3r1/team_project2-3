#include "StateDerived.h"
#include "Player.h"
#include "Mathf.h"

////////////////////////捜索ステート//////////////////////////
//デストラクタ
SearchState::~SearchState()
{
	for (auto state : subStatePool)
	{
		delete state;
	}
	subStatePool.clear();
}
//入った時
void SearchState::Enter()
{
	SetSubState(static_cast<int>(Enemy::Search::Idle));
}
//実行中
void SearchState::Execute(float elapsedTime)
{
	subState->Execute(elapsedTime);
	if (Player::Instance().GetHealth() > 0)
	{
		// Battleステートへ遷移
		owner->GetStateMachine()->ChangeState(static_cast<int>(Enemy::State::Battle));
	}
}
//出ていくとき
void SearchState::Exit()
{
}

#ifdef SEARCHSTATE
////////////////////////待機ステート//////////////////////////
//入った時
void IdleState::Enter()
{
#ifdef ENEMYANIMATION
	owner->GetModel()->PlayAnimation(static_cast<int>(EnemySlimeAnimation::Walk), true);
#endif // ENEMYANIMATION
}
//実行中
void IdleState::Execute(float elapsedTime)
{
	// 攻撃フラグがtrueなら戦闘ステートへ遷移
	if (owner->GetProjectileAttackFlg())
	{
		owner->GetStateMachine()->ChangeState(static_cast<int>(Enemy::State::Battle));
	}
	health = owner->GetHealth();
}
//出ていくとき
void IdleState::Exit()
{
}
#endif // SEARCHSTATE


////////////////////////戦闘ステート//////////////////////////
//デストラクタ
BattleState::~BattleState()
{
	for (auto state : subStatePool)
	{
		delete state;
	}
	subStatePool.clear();
}
//入った時
void BattleState::Enter()
{
	SetSubState(static_cast<int>(Enemy01::Battle::Pursuit));
}
//実行中
void BattleState::Execute(float elapsedTime)
{
	subState->Execute(elapsedTime);
}
//出ていくとき
void BattleState::Exit()
{
}

#ifdef BATTLESTATE
////////////////////////追跡ステート//////////////////////////
//入った時
void PursuitState::Enter()
{
#ifdef ENEMYANIMATION
	//TODO:アニメーション一時挿入してるため要修正
	owner->GetModel()->PlayAnimation(static_cast<int>(EnemySlimeAnimation::Walk), true);
#endif // ENEMYANIMATION
	owner->SetStateTimer(Mathf::RandomRange(3.0f, 5.0f));
}
//入った時
void PursuitState::Execute(float elapsedTime)
{
	// 目的地点までのXZ平面での距離判定
	DirectX::XMFLOAT3 position = owner->GetPosition();
	DirectX::XMFLOAT3 player_position = Player::Instance().GetPosition();
	owner->SetTargetPosition(player_position);
	float vx = player_position.x - position.x;
	float vz = player_position.z - position.z;
	float distSq = vx * vx + vz * vz;

	// 攻撃範囲内までプレイヤーに近づいた
	float radius = owner->GetAttackRange();
	if (distSq < radius * radius)
	{
		owner->GetStateMachine()->ChangeSubState(static_cast<int>(Enemy::Battle::Attack));
	}
	if (Player::Instance().GetHealth() <= 0)
	{
		owner->GetStateMachine()->ChangeState(static_cast<int>(Enemy::State::Search));
	}
	owner->MoveSystem();
	owner->OutMove();
	// 目的地点へ移動
	owner->MoveToTarget(elapsedTime, 0.5f);
	if (health != owner->GetHealth())
	{
		owner->GetStateMachine()->ChangeSubState(static_cast<int>(Enemy::Battle::Hit));
	}
	health = owner->GetHealth();
}
//出ていくとき
void PursuitState::Exit()
{
}


////////////////////////攻撃ステート//////////////////////////
//入った時
void AttackState::Enter()
{
#ifdef ENEMYANIMATION
	owner->GetModel()->PlayAnimation(static_cast<int>(EnemySlimeAnimation::Attack01), false);
#endif // ENEMYANIMATION
}
//実行中
void AttackState::Execute(float elapsedTime)
{
	DirectX::XMFLOAT3 player_position = Player::Instance().GetPosition();
	DirectX::XMFLOAT3 enemy_position = owner->GetPosition();
	owner->SetTargetPosition(player_position);
	float vx = player_position.x - enemy_position.x;
	float vz = player_position.z - enemy_position.z;
	float distSq = vx * vx + vz * vz;
	if (distSq > owner->GetAttackRange() * owner->GetAttackRange())
	{
		if (!owner->GetModel()->IsPlayAnimation())
		{
			owner->GetStateMachine()->ChangeSubState(static_cast<int>(Enemy::Battle::Pursuit));
		}
	}
	else
	{
#ifdef ENEMYATTACK
		owner->InputProjectile();
#endif // ENEMYATTACK
	}
	if (Player::Instance().GetHealth() <= 0)
	{
		if (!owner->GetModel()->IsPlayAnimation())
		{
			owner->GetStateMachine()->ChangeState(static_cast<int>(Enemy::State::Search));
		}
	}
	if (!owner->GetProjectileAttackFlg())
	{
		if (!owner->GetModel()->IsPlayAnimation())
		{
			owner->GetStateMachine()->ChangeSubState(static_cast<int>(Enemy::Battle::Idle));
		}
	}
	owner->Turn(elapsedTime, vx, vz, owner->GetTurnSpeed());
	if (health != owner->GetHealth())
	{
		owner->GetStateMachine()->ChangeSubState(static_cast<int>(Enemy::Battle::Hit));
	}
	health = owner->GetHealth();
}
//出ていくとき
void AttackState::Exit()
{
}

void BattleIdleState::Enter()
{
#ifdef ENEMYANIMATION
	owner->GetModel()->PlayAnimation(static_cast<int>(EnemySlimeAnimation::Walk), true);
#endif // ENEMYANIMATION
}

void BattleIdleState::Execute(float elapsedTime)
{
	if (owner->GetProjectileAttackFlg())
	{
		owner->GetStateMachine()->ChangeSubState(static_cast<int>(Enemy::Battle::Attack));
	}

	DirectX::XMFLOAT3 player_position = Player::Instance().GetPosition();
	DirectX::XMFLOAT3 enemy_position = owner->GetPosition();
	owner->SetTargetPosition(player_position);
	float vx = player_position.x - enemy_position.x;
	float vz = player_position.z - enemy_position.z;
	float distSq = vx * vx + vz * vz;
	if (distSq > owner->GetAttackRange() * owner->GetAttackRange())
	{
		owner->GetStateMachine()->ChangeSubState(static_cast<int>(Enemy::Battle::Pursuit));
	}
	if (health != owner->GetHealth())
	{
		owner->GetStateMachine()->ChangeSubState(static_cast<int>(Enemy::Battle::Hit));
	}
	health = owner->GetHealth();
    owner->Turn(elapsedTime, vx, vz, owner->GetTurnSpeed());
}

void BattleIdleState::Exit()
{
}

void HitDamageState::Enter()
{
#ifdef ENEMYANIMATION
	owner->GetModel()->PlayAnimation(static_cast<int>(EnemySlimeAnimation::Hit), false);
#endif // ENEMYANIMATION
}

void HitDamageState::Execute(float elapsedTime)
{
	if (!owner->GetModel()->IsPlayAnimation())
	{
		owner->GetStateMachine()->ChangeState(static_cast<int>(Enemy::State::Search));
	}
}

void HitDamageState::Exit()
{
}
#endif // BATTLESTATE