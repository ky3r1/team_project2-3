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
	// 
	if (owner->SearchPlayer())
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
////////////////////////徘徊ステート//////////////////////////
// 入った時
void WanderState::Enter()
{
#ifdef ENEMYANIMATION
	owner->GetModel()->PlayAnimation(static_cast<int>(EnemySlimeAnimation::WalkFWD), true);
#endif // ENEMYANIMATION
}
// 実行中
void WanderState::Execute(float elapsedTime)
{
	// 目的地点までのXZ平面での距離判定
	DirectX::XMFLOAT3 position = owner->GetPosition();
	DirectX::XMFLOAT3 targetPosition = owner->GetTargetPosition();
	float vx = targetPosition.x - position.x;
	float vz = targetPosition.z - position.z;
	float distSq = vx * vx + vz * vz;

	// 目的地へ着いた
	float radius = owner->GetRadius();
	if (distSq < radius * radius)
	{
		// 待機ステートへ遷移
		// ChangeStateクラスでStateを切り替える
		owner->GetStateMachine()->ChangeSubState(static_cast<int>(EnemySlime::Search::Idle));
	}
	// 目的地点へ移動
	owner->MoveToTarget(elapsedTime, 0.5f);
	// プレイヤー索敵
	if (owner->SearchPlayer())
	{
		// 見つかったら追跡ステートへ遷移
		// ChangeStateクラスでStateを切り替える
		owner->GetStateMachine()->ChangeState(static_cast<int>(Enemy::State::Battle));
	}
}
//出ていくとき
void WanderState::Exit()
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
	if (owner->SearchPlayer())
	{
		// 見つかったら追跡ステートへ遷移
		// ChangeStateクラスでStateを切り替える
		owner->GetStateMachine()->ChangeState(static_cast<int>(EnemySlime::State::Battle));
	}
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
	if (!owner->SearchPlayer())
	{
		// SearchStateステートへ遷移
		owner->GetStateMachine()->ChangeState(static_cast<int>(EnemySlime::State::Search));
	}
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
	// 目的地点へ移動
	owner->MoveToTarget(elapsedTime, 0.5f);

	// プレイヤーを見失った
	if (!owner->SearchPlayer())
	{
		owner->GetStateMachine()->ChangeState(static_cast<int>(EnemySlime::State::Search));
	}
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
	// 目的地点までのXZ平面での距離判定
	DirectX::XMFLOAT3 position = owner->GetPosition();
	DirectX::XMFLOAT3 player_position = Player::Instance().GetPosition();
	owner->SetTargetPosition(player_position);
	float vx = player_position.x - position.x;
	float vz = player_position.z - position.z;
	float distSq = vx * vx + vz * vz;
	// 攻撃範囲内までプレイヤーに近づいた
	float radius = owner->GetAttackRange();
	if (distSq > radius * radius)
	{
		//if (!owner->GetModel()->IsPlayAnimation())
		//{
			owner->GetStateMachine()->ChangeSubState(static_cast<int>(EnemySlime::Battle::Pursuit));
		//}
	}

	if (!owner->SearchPlayer())
	{
		owner->GetStateMachine()->ChangeState(static_cast<int>(EnemySlime::State::Search));
	}
}
//出ていくとき
void AttackState::Exit()
{

}
#endif // BATTLESTATE