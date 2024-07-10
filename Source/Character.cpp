#include "Character.h"
#include "StageManager.h"
//#include "StageMapChip.h"
#include "Mathf.h"
#include "ProjectileManager.h"
#include "ProjectileStraight.h"
#include "ProjectileHoming.h"
#include "EnemyManager.h"


//�s��X�V����
void Character::UpdateTransform()
{
    //�X�P�[���s����쐬
    DirectX::XMMATRIX S = DirectX::XMMatrixScaling(scale.x, scale.y, scale.z);
    //DirectX::XMMATRIX S = DirectX::XMMatrixScaling(0.1, 0.1, 0.1);
    //��]�s����쐬
    DirectX::XMMATRIX R = DirectX::XMMatrixRotationRollPitchYaw(angle.x, angle.y, angle.z);
    //�ʒu�s����쐬
    DirectX::XMMATRIX T = DirectX::XMMatrixTranslation(position.x, position.y, position.z);
    //3�̍s���g�ݍ��킹�A���[���h�s����쐬
    DirectX::XMMATRIX W = S * R * T;
    //�v�Z�������[���h�s������o��
    DirectX::XMStoreFloat4x4(&transform, W);
}

//�W�����v����
void Character::Jump(float speed)
{
    velocity.y = speed;
}

void Character::Move(float vx, float vz, float speed)
{
    //�ړ������x�N�g��
    moveVecX = vx;
    moveVecZ = vz;

    //�ő呬�x�ݒ�
    maxMoveSpeed = speed;
}

void Character::Turn(float elapsedTime, float vx, float vz, float speed)
{
    speed *= elapsedTime;

    //�i�s�x�N�g�����[���x�N�g���̏ꍇ�͏�������K�v�Ȃ�
    float length = sqrtf(vx * vx + vz * vz);
    if (length < 0.0001f) return;

    //�i�s�x�N�g����P�ʃx�N�g����
    vx /= length;
    vz /= length;


    //���g�̉�]�l����O���������߂�
    float frontX = sinf(angle.y);
    float frontZ = cosf(angle.y);

    //��]�p�����߂邽�߁A2�̒P�ʃx�N�g���̓��ς��v�Z����
    float dot = (vx * frontX) + (vz * frontZ);

    //���ϒl��-1.0�`1.0�ŕ\������Ă���A2�̒P�ʃx�N�g���̊p�x��
    // �������ق�1.0�ɋ߂Â��Ƃ��������𗘗p���ĉ�]���x�𒲐�����
    float rot = 1.0 - dot;
    if (rot > speed) { rot = speed; }
    //speed *= rot;
    //���E������s�����߂�2�̒P�ʃx�N�g���̊O�ς��v�Z����
    float cross = (vz * frontX) - (vx * frontZ);

    //2D�̊O�ϒl�����̏ꍇ�����̏ꍇ�ɂ���č��E���肪�s����
    //���E������s�����Ƃɂ���č��E��]��I������

    if (cross < 0.0f)
    {
        angle.y += rot;
    }
    else
    {
        angle.y -= rot;
    }
}

//���͏����X�V
void Character::UpdateVelocity(float elapsedTime)
{
    //�o�߃t���[��
    float elapsedFrame = 60.0f * elapsedTime;

    //�������͍X�V����
    UpdateVerticalVelocity(elapsedFrame);

    //�������͍X�V����
    UpdateHorizontalVelocity(elapsedFrame);

    //�����ړ��X�V����
    UpdateVerticalMove(elapsedTime);

    //�����ړ��X�V����
    UpdateHorizontalMove(elapsedTime);
}

//���G����
void Character::UpdateInvincibleTime(float elapsedTime)
{
    if (invincibleTimer > 0.0f)
    {
        invincibleTimer -= elapsedTime;
    }
}

//�_���[�W��^����
bool Character::ApplyDamage(int damage, float invincibleTime)
{
    //�@�_���[�W���O�̏ꍇ�͌��N��Ԃ�ύX����K�v���Ȃ�
    if (damage == 0) return false;

    //���S���Ă���ꍇ�͌��N��Ԃ�ύX���Ȃ�
    if (health <= 0) return false;
    if (invincibleTimer <= 0)
    {
        //�_���[�W����
        health -= damage;
        invincibleTimer = invincibleTime;
        //���S�ʒm
        if (health <= 0)
        {
            OnDead();
        }
        else
        {
            OnDamaged();
        }
        //���N��Ԃ��ύX�����ꍇ��true��Ԃ�
        return true;
    }
}

//�Ռ���^����
void Character::AddImpulse(const DirectX::XMFLOAT3& impulse)
{
    //���͂ɗ͂�������
    velocity.x += impulse.x;
    velocity.y += impulse.y;
    velocity.z += impulse.z;
}

void Character::DrawDebugGUI()
{
}

//������
void Character::Clear()
{
    health = maxHealth;
}

//�������͏���
void Character::UpdateVerticalVelocity(float elapsedFrame)
{
    ////�d�͏���
    velocity.y += gravity * elapsedFrame;
}

//�����ړ��X�V����
void Character::UpdateVerticalMove(float elapsedTime)
{
    //���������̈ړ���
    float my = velocity.y * elapsedTime;

    slopeRate = 0.0f;

    //�L������Y�������ƂȂ�@���x�N�g��
    DirectX::XMFLOAT3 normal = { 0,1,0 };

    //������
    if (my < 0.0f)
    {
        //���C�̊J�n�ʒu�͑�����菭����
        DirectX::XMFLOAT3 start = { position.x,position.y + stepOffset,position.z };
        //���C�̏I�_�ʒu�͈ړ���̈ʒu
        DirectX::XMFLOAT3 end = { position.x,position.y + my ,position.z };

        //���C�L���X�g�ɂ��n�ʔ���
        HitResult hit;
        if (StageManager::Instance().RayCast(start, end, hit))
        {
            //�n�ʂɐڒn���Ă���

            //�@���x�N�g���擾
            normal = hit.normal;

            position = hit.position;
            //if (position.y > 0.5)angle = hit.rotation;

            position.y = hit.position.y;


            //�X�Η��̌v�Z
            float normalLengthXZ = sqrtf(hit.normal.x + hit.normal.z * hit.normal.z);
            slopeRate = 1.0 - (hit.normal.y / (normalLengthXZ + hit.normal.y));

            //�ڒn����
            if (!isGround)
            {
                OnLanding();
            }
            isGround = true;
            velocity.y = 0.0f;
        }
        else
        {
            //�󒆂ɕ����Ă���
            position.y += my;
            isGround = false;
        }
    }
    //�㏸��
    else if (my > 0.0f)
    {
        position.y += my;
        isGround = false;
    }

    //�n�ʂ̌����ɉ����悤��XZ����]
    {
        //Y�����@���x�N�g�������Ɍ����I�C���[�p��]���Z�o
        float X, Z;
        X = atan2f(normal.z, normal.y);
        Z = -atan2f(normal.x, normal.y);

        angle.x = Mathf::Lerp(angle.x, X, 0.1f);
        angle.z = Mathf::Lerp(angle.z, Z, 0.1f);
    }
}

//�������͍X�V����
void Character::UpdateHorizontalVelocity(float elapsedFrame)
{
    //�@XZ���ʂ̑��͂���������
    DirectX::XMFLOAT2 vel = { velocity.x, velocity.z };

    DirectX::XMVECTOR velVel = DirectX::XMLoadFloat2(&vel);
    float length = DirectX::XMVectorGetX(DirectX::XMVector2Length(velVel));
    if (length > 0.0f)
    {
        //���C��
        float friction = this->friction * elapsedFrame;

        //�󒆂ɂ���Ƃ��͖��C�͂����炷
        if (isGround == false) friction *= airControl;

        //���C�ɂ�鉡�����̌�������
        if (length > friction)
        {
            //���x��P�ʃx�N�g����
            auto v = DirectX::XMVector2Normalize(velVel);

            //�P�ʃx�N�g�����������x���@friction ���X�P�[�����O
            v = DirectX::XMVectorScale(v, friction);

            //�X�P�[�����O�����x�N�g���𑬓x�Ɍ��Z
            DirectX::XMStoreFloat2(&vel, DirectX::XMVectorSubtract(velVel, v));

            velocity.x = vel.x;
            velocity.z = vel.y;
        }
        //�������̑��͂����C�͈ȉ��ɂȂ����̂ő��͂𖳌���
        else
        {
            velocity.x = 0.0f;
            velocity.z = 0.0f;
        }

        //���͂̍ő�l
        if (velocity.x < -5.0f)
        {
            velocity.x = -5.0f;
        }
        if (velocity.x > 5.0f)
        {
            velocity.x = 5.0f;
        }
        if (velocity.z < -5.0f)
        {
            velocity.z = -5.0f;
        }
        if (velocity.z > 5.0f)
        {
            velocity.z = 5.0f;
        }
    }

    //XZ���ʂ̑��͂���������
    if (length <= maxMoveSpeed)
    {
        //�ړ��x�N�g�����[���x�N�g���łȂ��Ȃ��������
        float moveVecLength = sqrtf(moveVecX * moveVecX + moveVecZ * moveVecZ);
        if (moveVecLength > 0.0f)
        {
            //������
            float acceleration = this->acceleration * elapsedFrame;

            if (isGround == false) acceleration *= airControl;

            //�ړ��x�N�g���ɂ���������
            velocity.x += moveVecX * acceleration;
            velocity.z += moveVecZ * acceleration;



            //�ő呬�x����
            if (length > maxMoveSpeed)
            {
                float vx = velocity.x / length;
                float vz = velocity.z / length;

                velocity.x = vx * maxMoveSpeed;
                velocity.z = vz * maxMoveSpeed;
            }

            //�����ŃK�^�K�^���Ȃ��悤�ɂ���
            if (isGround && slopeRate > 0.0f)
            {
                velocity.y -= length * slopeRate * elapsedFrame;
            }
        }
    }
    //�ړ��x�N�g�������Z�b�g
    moveVecX = 0.0f;
    moveVecZ = 0.0f;
}

//�����ړ��X�V����
void Character::UpdateHorizontalMove(float elapsedTime)
{
    float velocityLengthXZ = sqrtf(velocity.z * velocity.z + velocity.x * velocity.x);
    if (velocityLengthXZ > 0.0f)
    {
        //�����ړ��l
        float mx = velocity.x * elapsedTime;
        float mz = velocity.z * elapsedTime;

        //���C�̊J�n�ʒu�ƏI�_�ʒu
        DirectX::XMFLOAT3 start = { position.x,position.y + stepOffset, position.z };
        DirectX::XMFLOAT3 end = { position.x + mx, position.y + stepOffset,position.z + mz };

        //���C�L���X�g�ɂ��ǔ���
        HitResult hit;
        if (StageManager::Instance().RayCast(start, end, hit))
        {
            //�ǂ܂ł̃x�N�g��
            DirectX::XMVECTOR Start = DirectX::XMLoadFloat3(&start);
            DirectX::XMVECTOR End = DirectX::XMLoadFloat3(&end);
            DirectX::XMVECTOR Vec = DirectX::XMVectorSubtract(Start, End);

            //�ǂ̖@��
            DirectX::XMVECTOR Normal = DirectX::XMLoadFloat3(&hit.normal);

            //���˃x�N�g����@���Ɏˉe
            DirectX::XMVECTOR Dot = DirectX::XMVector3Dot(Vec, Normal);

            // �␳�ʒu�̌v�Z
            DirectX::XMVECTOR PB = DirectX::XMVectorSubtract(End, DirectX::XMLoadFloat3(&hit.position));

            DirectX::XMVECTOR R = DirectX::XMVectorAdd(PB, DirectX::XMVectorScale(DirectX::XMVector3Normalize(Normal), DirectX::XMVectorGetX(Dot)));

            DirectX::XMVECTOR O = DirectX::XMVectorAdd(End, DirectX::XMVectorScale(DirectX::XMVector3Normalize(Normal), DirectX::XMVectorGetX(Dot)));
            DirectX::XMFLOAT3 o;
            DirectX::XMStoreFloat3(&o, O);

            DirectX::XMFLOAT3 collectPosition = position;
            collectPosition.x = o.x;
            collectPosition.z = o.z;
            HitResult hit2;
            if (!StageManager::Instance().RayCast(hit.position, collectPosition, hit2))
            {
                position.x = o.x;
                position.z = o.z;
            }
        }
        else
        {
            //�ړ�
            position.x += mx;
            position.z += mz;
        }
    }
}

void Character::ProjectileDirection(ID3D11DeviceContext* dc, const DirectX::XMFLOAT4X4& view, const DirectX::XMFLOAT4X4& projection)
{
    //�r���[�|�[�g
    D3D11_VIEWPORT viewport;
    UINT numViewports = 1;
    dc->RSGetViewports(&numViewports, &viewport);

    //�ϊ��s��
    DirectX::XMMATRIX View = DirectX::XMLoadFloat4x4(&view);
    DirectX::XMMATRIX Projection = DirectX::XMLoadFloat4x4(&projection);
    DirectX::XMMATRIX World = DirectX::XMMatrixIdentity();


    //�G�l�~�[�z�u����
    GamePad& gamePad = Input::Instance().GetGamePad();
    Mouse& mouse = Input::Instance().GetMouse();
    /*if (gamePad.GetButtonDown() & GamePad::BTN_A)
    {*/
    //�}�E�X�J�[�\�����W�擾
    DirectX::XMFLOAT3 screenPosition;
    screenPosition.x = static_cast<float>(mouse.GetPositionX());
    screenPosition.y = 0;
    screenPosition.z = static_cast<float>(mouse.GetPositionY());

    DirectX::XMVECTOR ScreenCursor = DirectX::XMLoadFloat3(&screenPosition);

    DirectX::XMVECTOR WorldPosition = DirectX::XMVector3Unproject
    (
        ScreenCursor,
        viewport.TopLeftX,
        viewport.TopLeftY,
        viewport.Width,
        viewport.Height,
        viewport.MinDepth,
        viewport.MaxDepth,
        Projection,
        View,
        World
    );
    DirectX::XMFLOAT3 world_position_start;
    DirectX::XMStoreFloat3(&world_position_start, WorldPosition);

    screenPosition.z = 1;
    ScreenCursor = DirectX::XMLoadFloat3(&screenPosition);
    WorldPosition = DirectX::XMVector3Unproject
    (
    	ScreenCursor,
    	viewport.TopLeftX,
    	viewport.TopLeftY,
    	viewport.Width,
    	viewport.Height,
    	viewport.MinDepth,
    	viewport.MaxDepth,
    	Projection,
    	View,
    	World
    );
    DirectX::XMFLOAT3 world_position_end;
    DirectX::XMStoreFloat3(&world_position_end, WorldPosition);

    HitResult hit;
    StageMain stage_main;
    if (stage_main.RayCast(world_position_start, world_position_end, hit))
    {
    /*EnemyManager& enemyManager = EnemyManager::Instance();
    EnemySlime* slime = new EnemySlime(GREEN, 0);
    slime->SetPosition(DirectX::XMFLOAT3(world_position_start.x, world_position_start.y, world_position_start.z));
    enemyManager.Register(slime);*/
    }
    //}
}

//DelayTime�̍X�V
void Character::UpdateDelayTime(bool& checker, int& time, int delaytime)
{
    //checker��false�ꍇ�̓^�C�������炷
    //checker��true�ꍇ��checker��true�ɂ��ă^�C�������Z�b�g
    if (!checker)
    {
        time--;
    }
    if (time < 0)
    {
        checker = true;
        time = delaytime;
    }
}


//�e�ƃL�����N�^�[�̏Փ˔���
void Character::CollisionProjectileVsCharacter(Character* character, Effect hiteffect)
{
    //���ׂĂ̒e�ۂ�target�𑍓�����ŏՓˏ���
    int projectileCount = ProjectileManager::Instance().GetProjectileCount();
    for (int i = 0; i < projectileCount; ++i)
    {
        //�e�ێ擾
        Projectile* projectile = ProjectileManager::Instance().GetProjectile(i);
         EnemyManager& enemyManager = EnemyManager::Instance();
         int enemyCount = enemyManager.GetEnemyCount();

        //�Փ˔���
        DirectX::XMFLOAT3 outPosition;
        if (Collision::IntersectSphereVsCylinder(
            projectile->GetPosition(),
            projectile->GetRadius(),
            character->GetPosition(),
            character->GetRadius(),
            character->GetHeight(),
            outPosition))
        {
            //�Փ˂����e�ƒe���o�����L�����N�^�[(������character�ł͂Ȃ�)�̃J�e�S���[�������ꍇ
            if (category == projectile->GetCategory())
            {
                //�e�۔j��
                if (projectile_category == PENETRATION)
                {

                    if (category == PLAYERCATEGORY && penetration_count == 0)
                    {
                        projectile->Destroy();
                    }
                }
                if(projectile_category == RICOCHET)
                {
                    //if(invincible==true){}
                    for (int i = 0; i < enemyCount; i++)
                    {
                        Enemy* enemy = EnemyManager::Instance().GetEnemy(i);
                        Enemy* ne = EnemyManager::Instance().NearEnemy(enemy->GetPosition());
                        if (enemy->isHit)break;
                        enemy->isHit = true;
                        
                        if (invincible == true)break;
                        invincible = true;
                        ProjectileRicochetShotting(character->GetPosition(), PLAYERCATEGORY, 0.0f, FRONT);
                    }
                    if(invincible==false)
                    {
                        if (penetration_count == 0)
                        {
                            projectile->Destroy();
                            for (int i = 0; i < enemyCount; i++)
                            {
                                Enemy* enemy = EnemyManager::Instance().GetEnemy(i);
                                if (enemy->isHit)
                                {
                                    enemy->isHit = false;
                                }
                            }
                        }
                    }
                }
                if (/*character->ApplyDamage(1, 0.5f),*/true)
                {
                    //������΂�
                    {
                        DirectX::XMFLOAT3 impulse;
                        //������΂���
                        const float power = 2.0f;

                        //�G�̈ʒu
                        DirectX::XMVECTOR eVec = DirectX::XMLoadFloat3(&character->GetPosition());
                        //�e�̈ʒu
                        DirectX::XMVECTOR pVec = DirectX::XMLoadFloat3(&projectile->GetPosition());
                        //�e����G�ւ̕����x�N�g�����v�Z�i�G - �e�j
                        auto v = DirectX::XMVectorSubtract(eVec, pVec);
                        //�����x�N�g���𐳋K��
                        v = DirectX::XMVector3Normalize(v);

                        DirectX::XMFLOAT3 vec;
                        DirectX::XMStoreFloat3(&vec, v);

                        impulse.x = power * vec.x;
                        impulse.y = power * 0.5f;
                        impulse.z = power * vec.z;
                        penetration_count--;
                        ricochet_count--;
                        
                        character->AddImpulse(impulse);
                    }
                    //�q�b�g�G�t�F�N�g�Đ�
                    {
                        DirectX::XMFLOAT3 e = character->GetPosition();
                        e.y += character->GetHeight() * 0.5f;
                        hiteffect.Play(e, 2.0f);
                    }
                }
                else
                {
                    //�e�۔j��
                    projectile->Destroy();
                }
            }
        }
    }
}

//�e�۔��ˏ���
void Character::ProjectileStraightShotting(int category, float angle, int vector)
{
    //����
    ProjectileStraight* projectile{};
    //�O����
    DirectX::XMFLOAT3 dir;
    //DirectX::XMFLOAT3 axis = { 0,1,0 };
    //DirectX::XMVECTOR Axis;
    //DirectX::XMFLOAT3 s={ scale.x / 0.1f,scale.y / 0.1f ,scale.z / 0.1f };
    DirectX::XMFLOAT3 s = { 0.1f / scale.x,0.1f / scale.y ,0.1f / scale.z };

    dir.x = vector * transform._31 * 100.0f * s.x;
    dir.y = 0.0f;
    dir.z = vector * transform._33 * 100.0f * s.z;
    DirectX::XMFLOAT3 right;
    right.x = transform._11 * 100.0f * s.x;
    right.y = 0.0f;
    right.z = transform._13 * 100.0f * s.z;
    //���ˈʒu�i�v���C���[�̍�������j
    DirectX::XMFLOAT3 pos;
    pos.x = position.x;
    pos.y = position.y + height * 0.5f;
    pos.z = position.z;

    DirectX::XMVECTOR Right = DirectX::XMLoadFloat3(&right);
    Right = DirectX::XMVectorScale(Right, angle);
    DirectX::XMVECTOR Dir = DirectX::XMLoadFloat3(&dir);
    DirectX::XMVECTOR Pos = DirectX::XMLoadFloat3(&pos);
    DirectX::XMVECTOR Ev = DirectX::XMVectorAdd(Dir, Right);
    DirectX::XMVECTOR Ep = DirectX::XMVectorAdd(Pos, Ev);
    Ep = DirectX::XMVectorSubtract(Ep, Pos);
    DirectX::XMFLOAT3 ep;
    DirectX::XMStoreFloat3(&ep, Ep);
    dir.x = ep.x;
    dir.y = 0.0f;
    dir.z = ep.z;
    //projectile = new ProjectileStraight(&ProjectileManager::Instance(), category,lineEffect.get());
    projectile = new ProjectileStraight(&ProjectileManager::Instance(), category);
    projectile->Launch(dir, pos);
}

void Character::ProjectileRicochetShotting(DirectX::XMFLOAT3 ne,int category, float angle, int vector)
{
    Ricochet(ne, vector);
}


void Character::Ricochet(DirectX::XMFLOAT3 ne, int vector)
{
    //����
    ProjectileStraight* projectile{};
    //�O����
    DirectX::XMFLOAT3 dir;
    //DirectX::XMFLOAT3 axis = { 0,1,0 };
    //DirectX::XMVECTOR Axis;
    //DirectX::XMFLOAT3 s={ scale.x / 0.1f,scale.y / 0.1f ,scale.z / 0.1f };
    DirectX::XMFLOAT3 s = { 0.1f / scale.x,0.1f / scale.y ,0.1f / scale.z };


    Enemy* ne1 = EnemyManager::Instance().NearEnemy(position);

    Enemy* ne2 = EnemyManager::Instance().NearEnemy(ne);

    if (ne2 == nullptr)return;
    DirectX::XMVECTOR NE1 = DirectX::XMLoadFloat3(&ne);
    DirectX::XMVECTOR NE2 = DirectX::XMLoadFloat3(&ne2->GetPosition());
    DirectX::XMVECTOR Vec = DirectX::XMVector3Normalize(DirectX::XMVectorSubtract(NE2, NE1));
    DirectX::XMFLOAT3 ND;
    DirectX::XMStoreFloat3(&ND, Vec);

    dir.x = vector * ND.x * 100.0f * s.x;
    dir.y = 0.0f;
    dir.z = vector * ND.z * 100.0f * s.z;
    DirectX::XMFLOAT3 right;
    right.x = ND.x * 100.0f * s.x;
    right.y = 0.0f;
    right.z = ND.z * 100.0f * s.z;
    //���ˈʒu�i�v���C���[�̍�������j
    DirectX::XMFLOAT3 pos;
    pos.x = ne.x;
    pos.y = ne.y + height * 0.5f;
    pos.z = ne.z;

    DirectX::XMVECTOR Right = DirectX::XMLoadFloat3(&right);
    Right = DirectX::XMVectorScale(Right, 0.0f);
    DirectX::XMVECTOR Dir = DirectX::XMLoadFloat3(&dir);
    DirectX::XMVECTOR Pos = DirectX::XMLoadFloat3(&pos);
    DirectX::XMVECTOR Ev = DirectX::XMVectorAdd(Dir, Right);
    DirectX::XMVECTOR Ep = DirectX::XMVectorAdd(Pos, Ev);
    Ep = DirectX::XMVectorSubtract(Ep, Pos);
    DirectX::XMFLOAT3 ep;
    DirectX::XMStoreFloat3(&ep, Ep);
    dir.x = ep.x;
    dir.y = 0.0f;
    dir.z = ep.z;
    projectile = new ProjectileStraight(&ProjectileManager::Instance(), category);
    projectile->Launch(dir, pos);

}


void Character::ProjectileStraightShotting(float angle, int vector)
{
    ProjectileStraightShotting(category, angle, vector);
}
