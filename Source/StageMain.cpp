#include"StageMain.h"

//�R���X�g���N�^
StageMain::StageMain()
{
    //�X�e�[�W���f���̓ǂݍ���
    model = new Model("Data/Model/Cube/Cube.mdl");
    scale = { 100.0f,2.0f,100.0f };
    color = { 1,1,1,1 };
    UpdateTransform();
    model->UpdateTransform(transform);
}

//�f�X�g���N�^
StageMain::~StageMain()
{
    //�X�e�[�W���f���̔j��
    if (model)
    {
        delete model;
        model = nullptr;
    }
}

//�X�V����
void StageMain::Update(float elapsedTime)
{
    UpdateTransform();
    model->UpdateTransform(transform);
}

//�`�揈��
void StageMain::Render(ID3D11DeviceContext* dc, Shader* shader)
{
    //�V�F�[�_�[�̃��f���`��
    shader->Draw(dc, model,color);
}

bool StageMain::RayCast(const DirectX::XMFLOAT3& start, const DirectX::XMFLOAT3& end, HitResult& hit)
{
    return Collision::IntersectRayVsModel(start, end, model, hit);
}

void StageMain::DrawDebugGUI()
{

}
