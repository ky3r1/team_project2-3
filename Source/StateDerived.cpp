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
////////////////////////���S�X�e�[�g//////////////////////////
// ��������
void DeathState::Enter()
{
#ifdef ENEMYANIMATION
	owner->GetModel()->PlayAnimation(static_cast<int>(EnemySlimeAnimation::Die), false);
#endif // ENEMYANIMATION
}
// ���s��
void DeathState::Execute(float elapsedTime)
{
#ifdef ENEMYANIMATION
	if (!owner->GetModel()->IsPlayAnimation())
	{
		owner->Destroy();
	}
#endif // ENEMYANIMATION
}
//�o�Ă����Ƃ�
void DeathState::Exit()
{
}


////////////////////////�ҋ@�X�e�[�g//////////////////////////
//��������
void IdleState::Enter()
{
#ifdef ENEMYANIMATION
	owner->GetModel()->PlayAnimation(static_cast<int>(EnemySlimeAnimation::IdleNormal), true);
#endif // ENEMYANIMATION
}
//���s��
void IdleState::Execute(float elapsedTime)
{
	idle_timer++;
	//1�b�o�߂�����p�j�X�e�[�g
	if (idle_timer > 60)
	{
		idle_timer = 0;
		owner->GetStateMachine()->ChangeSubState(static_cast<int>(Enemy01::Search::Death));
	}
	// �v���C���[���G
	//if (owner->SearchPlayer())
	//{
		owner->GetStateMachine()->ChangeState(static_cast<int>(Enemy01::State::Battle));
	//}
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
	owner->GetModel()->PlayAnimation(static_cast<int>(EnemySlimeAnimation::RunFWD), true);
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
		owner->GetStateMachine()->ChangeSubState(static_cast<int>(Enemy01::Battle::Attack));
	}
	if (Player::Instance().GetHealth() <= 0)
	{
		owner->GetStateMachine()->ChangeState(static_cast<int>(Enemy01::State::Search));
	}
	owner->MoveSystem();
	owner->OutMove();
	// �ړI�n�_�ֈړ�
	owner->MoveToTarget(elapsedTime, 0.5f);
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
	owner->GetModel()->PlayAnimation(static_cast<int>(EnemySlimeAnimation::Attack01), true);
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
		owner->GetStateMachine()->ChangeSubState(static_cast<int>(Enemy01::Battle::Pursuit));
	}
	if (Player::Instance().GetHealth() <= 0)
	{
		owner->GetStateMachine()->ChangeState(static_cast<int>(Enemy01::State::Search));
	}
	owner->Turn(elapsedTime, vx, vz, owner->GetTurnSpeed());
#ifdef ENEMYATTACK
	owner->InputProjectile();
#endif // ENEMYATTACK
}
//�o�Ă����Ƃ�
void AttackState::Exit()
{

}
#endif // BATTLESTATE