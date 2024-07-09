#include"StageMain.h"

//コンストラクタ
StageMain::StageMain()
{
    //ステージモデルの読み込み
    model = new Model("Data/Model/Stage/stage01.mdl");
    scale = { 0.10f,0.10f,0.10f };
    color = { 1,1,1,1 };
    UpdateTransform();
    model->UpdateTransform(transform);
}

//デストラクタ
StageMain::~StageMain()
{
    //ステージモデルの破棄
    if (model)
    {
        delete model;
        model = nullptr;
    }
}

//更新処理
void StageMain::Update(float elapsedTime)
{
    UpdateTransform();
    model->UpdateTransform(transform);
}

//描画処理
void StageMain::Render(ID3D11DeviceContext* dc, Shader* shader)
{
    //シェーダーのモデル描画
    shader->Draw(dc, model,color);
}

bool StageMain::RayCast(const DirectX::XMFLOAT3& start, const DirectX::XMFLOAT3& end, HitResult& hit)
{
    return Collision::IntersectRayVsModel(start, end, model, hit);
}

void StageMain::DrawDebugGUI()
{

}
