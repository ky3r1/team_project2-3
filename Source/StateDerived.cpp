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
	// 
	if (owner->SearchPlayer())
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
////////////////////////�p�j�X�e�[�g//////////////////////////
// ��������
void WanderState::Enter()
{
#ifdef ENEMYANIMATION
	owner->GetModel()->PlayAnimation(static_cast<int>(EnemySlimeAnimation::WalkFWD), true);
#endif // ENEMYANIMATION
}
// ���s��
void WanderState::Execute(float elapsedTime)
{
	// �ړI�n�_�܂ł�XZ���ʂł̋�������
	DirectX::XMFLOAT3 position = owner->GetPosition();
	DirectX::XMFLOAT3 targetPosition = owner->GetTargetPosition();
	float vx = targetPosition.x - position.x;
	float vz = targetPosition.z - position.z;
	float distSq = vx * vx + vz * vz;

	// �ړI�n�֒�����
	float radius = owner->GetRadius();
	if (distSq < radius * radius)
	{
		// �ҋ@�X�e�[�g�֑J��
		// ChangeState�N���X��State��؂�ւ���
		owner->GetStateMachine()->ChangeSubState(static_cast<int>(EnemySlime::Search::Idle));
	}
	// �ړI�n�_�ֈړ�
	owner->MoveToTarget(elapsedTime, 0.5f);
	// �v���C���[���G
	if (owner->SearchPlayer())
	{
		// ����������ǐՃX�e�[�g�֑J��
		// ChangeState�N���X��State��؂�ւ���
		owner->GetStateMachine()->ChangeState(static_cast<int>(Enemy::State::Battle));
	}
}
//�o�Ă����Ƃ�
void WanderState::Exit()
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
		owner->GetStateMachine()->ChangeSubState(static_cast<int>(EnemySlime::Search::Death));
	}
	// �v���C���[���G
	if (owner->SearchPlayer())
	{
		// ����������ǐՃX�e�[�g�֑J��
		// ChangeState�N���X��State��؂�ւ���
		owner->GetStateMachine()->ChangeState(static_cast<int>(EnemySlime::State::Battle));
	}
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
	SetSubState(static_cast<int>(EnemySlime::Battle::Pursuit));
	if (!owner->SearchPlayer())
	{
		// SearchState�X�e�[�g�֑J��
		owner->GetStateMachine()->ChangeState(static_cast<int>(EnemySlime::State::Search));
	}
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
		owner->GetStateMachine()->ChangeSubState(static_cast<int>(EnemySlime::Battle::Attack));
	}
	// �ړI�n�_�ֈړ�
	owner->MoveToTarget(elapsedTime, 0.5f);

	// �v���C���[����������
	if (!owner->SearchPlayer())
	{
		owner->GetStateMachine()->ChangeState(static_cast<int>(EnemySlime::State::Search));
	}
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
	// �ړI�n�_�܂ł�XZ���ʂł̋�������
	DirectX::XMFLOAT3 position = owner->GetPosition();
	DirectX::XMFLOAT3 player_position = Player::Instance().GetPosition();
	owner->SetTargetPosition(player_position);
	float vx = player_position.x - position.x;
	float vz = player_position.z - position.z;
	float distSq = vx * vx + vz * vz;
	// �U���͈͓��܂Ńv���C���[�ɋ߂Â���
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
//�o�Ă����Ƃ�
void AttackState::Exit()
{

}
#endif // BATTLESTATE