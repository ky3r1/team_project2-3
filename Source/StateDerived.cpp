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
////////////////////////死亡ステート//////////////////////////
// 入った時
void DeathState::Enter()
{
#ifdef ENEMYANIMATION
	owner->GetModel()->PlayAnimation(static_cast<int>(EnemySlimeAnimation::Die), false);
#endif // ENEMYANIMATION
}
// 実行中
void DeathState::Execute(float elapsedTime)
{
#ifdef ENEMYANIMATION
	if (!owner->GetModel()->IsPlayAnimation())
	{
		owner->Destroy();
	}
#endif // ENEMYANIMATION
}
//出ていくとき
void DeathState::Exit()
{
}


////////////////////////待機ステート//////////////////////////
//入った時
void IdleState::Enter()
{
#ifdef ENEMYANIMATION
	owner->GetModel()->PlayAnimation(static_cast<int>(EnemySlimeAnimation::IdleNormal), true);
#endif // ENEMYANIMATION
}
//実行中
void IdleState::Execute(float elapsedTime)
{
	idle_timer++;
	//1秒経過したら徘徊ステート
	if (idle_timer > 60)
	{
		idle_timer = 0;
		owner->GetStateMachine()->ChangeSubState(static_cast<int>(EnemySlime::Search::Death));
	}
	// プレイヤー索敵
	//if (owner->SearchPlayer())
	//{
		owner->GetStateMachine()->ChangeState(static_cast<int>(EnemySlime::State::Battle));
	//}
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
	SetSubState(static_cast<int>(EnemySlime::Battle::Pursuit));
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
	owner->GetModel()->PlayAnimation(static_cast<int>(EnemySlimeAnimation::RunFWD), true);
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
		owner->GetStateMachine()->ChangeSubState(static_cast<int>(EnemySlime::Battle::Attack));
	}
	if (Player::Instance().GetHealth() <= 0)
	{
		owner->GetStateMachine()->ChangeState(static_cast<int>(EnemySlime::State::Search));
	}
	owner->MoveSystem();
	owner->OutMove();
	// 目的地点へ移動
	owner->MoveToTarget(elapsedTime, 0.5f);
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
	owner->GetModel()->PlayAnimation(static_cast<int>(EnemySlimeAnimation::Attack01), true);
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
		owner->GetStateMachine()->ChangeSubState(static_cast<int>(EnemySlime::Battle::Pursuit));
	}
	if (Player::Instance().GetHealth() <= 0)
	{
		owner->GetStateMachine()->ChangeState(static_cast<int>(EnemySlime::State::Search));
	}
}
//出ていくとき
void AttackState::Exit()
{

}
#endif // BATTLESTATE