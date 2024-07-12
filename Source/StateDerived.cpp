#include "StateDerived.h"
#include "Player.h"
#include "Mathf.h"

////////////////////////�{���X�e�[�g//////////////////////////
//�f�X�g���N�^
SearchState::~SearchState()
{
	for (auto state : subStatePool)
	{
		delete state;
	}
	subStatePool.clear();
}
//��������
void SearchState::Enter()
{
	SetSubState(static_cast<int>(Enemy::Search::Idle));
}
//���s��
void SearchState::Execute(float elapsedTime)
{
	subState->Execute(elapsedTime);
	if (Player::Instance().GetHealth() > 0)
	{
		// Battle�X�e�[�g�֑J��
		owner->GetStateMachine()->ChangeState(static_cast<int>(Enemy::State::Battle));
	}
}
//�o�Ă����Ƃ�
void SearchState::Exit()
{
}

#ifdef SEARCHSTATE
////////////////////////�ҋ@�X�e�[�g//////////////////////////
//��������
void IdleState::Enter()
{
#ifdef ENEMYANIMATION
	owner->GetModel()->PlayAnimation(static_cast<int>(EnemySlimeAnimation::Walk), true);
#endif // ENEMYANIMATION
}
//���s��
void IdleState::Execute(float elapsedTime)
{
	// �U���t���O��true�Ȃ�퓬�X�e�[�g�֑J��
	if (owner->GetProjectileAttackFlg())
	{
		owner->GetStateMachine()->ChangeState(static_cast<int>(Enemy::State::Battle));
	}
	health = owner->GetHealth();
}
//�o�Ă����Ƃ�
void IdleState::Exit()
{
}
#endif // SEARCHSTATE


////////////////////////�퓬�X�e�[�g//////////////////////////
//�f�X�g���N�^
BattleState::~BattleState()
{
	for (auto state : subStatePool)
	{
		delete state;
	}
	subStatePool.clear();
}
//��������
void BattleState::Enter()
{
	SetSubState(static_cast<int>(Enemy01::Battle::Pursuit));
}
//���s��
void BattleState::Execute(float elapsedTime)
{
	subState->Execute(elapsedTime);
}
//�o�Ă����Ƃ�
void BattleState::Exit()
{
}

#ifdef BATTLESTATE
////////////////////////�ǐՃX�e�[�g//////////////////////////
//��������
void PursuitState::Enter()
{
#ifdef ENEMYANIMATION
	//TODO:�A�j���[�V�����ꎞ�}�����Ă邽�ߗv�C��
	owner->GetModel()->PlayAnimation(static_cast<int>(EnemySlimeAnimation::Walk), true);
#endif // ENEMYANIMATION
	owner->SetStateTimer(Mathf::RandomRange(3.0f, 5.0f));
}
//��������
void PursuitState::Execute(float elapsedTime)
{
	// �ړI�n�_�܂ł�XZ���ʂł̋�������
	DirectX::XMFLOAT3 position = owner->GetPosition();
	DirectX::XMFLOAT3 player_position = Player::Instance().GetPosition();
	owner->SetTargetPosition(player_position);
	float vx = player_position.x - position.x;
	float vz = player_position.z - position.z;
	float distSq = vx * vx + vz * vz;

	// �U���͈͓��܂Ńv���C���[�ɋ߂Â���
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
	// �ړI�n�_�ֈړ�
	owner->MoveToTarget(elapsedTime, 0.5f);
	if (health != owner->GetHealth())
	{
		owner->GetStateMachine()->ChangeSubState(static_cast<int>(Enemy::Battle::Hit));
	}
	health = owner->GetHealth();
}
//�o�Ă����Ƃ�
void PursuitState::Exit()
{
}


////////////////////////�U���X�e�[�g//////////////////////////
//��������
void AttackState::Enter()
{
#ifdef ENEMYANIMATION
	owner->GetModel()->PlayAnimation(static_cast<int>(EnemySlimeAnimation::Attack01), false);
#endif // ENEMYANIMATION
}
//���s��
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
//�o�Ă����Ƃ�
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