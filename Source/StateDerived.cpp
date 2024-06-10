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
	owner->GetModel()->PlayAnimation(static_cast<int>(EnemySlimeAnimation::Death), false);
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
	// ChangeState�N���X��State��؂�ւ���
	if (Player::Instance().GetHealth() > 0)
	{
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
	if (owner->InAttackRange())
	{
		owner->GetStateMachine()->ChangeSubState(static_cast<int>(EnemySlime::Battle::Attack));
	}
	if (Player::Instance().GetHealth() <= 0)
	{
		owner->GetStateMachine()->ChangeState(static_cast<int>(EnemySlime::State::Search));
	}
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
	owner->GetModel()->PlayAnimation(static_cast<int>(EnemySlimeAnimation::Attack01), false);
#endif // ENEMYANIMATION
}
//���s��
void AttackState::Execute(float elapsedTime)
{
	if (!owner->InAttackRange())
	{
		owner->GetStateMachine()->ChangeSubState(static_cast<int>(EnemySlime::Battle::Pursuit));
	}
	if (Player::Instance().GetHealth() <= 0)
	{
		owner->GetStateMachine()->ChangeState(static_cast<int>(EnemySlime::State::Search));
	}
}
//�o�Ă����Ƃ�
void AttackState::Exit()
{

}
#endif // BATTLESTATE