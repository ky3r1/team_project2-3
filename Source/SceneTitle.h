#pragma once

#include "Graphics/Sprite.h"
#include "Scene.h"
#include "Ui.h"

//�^�C�g���V�[��
class SceneTitle : public Scene
{
public:
    SceneTitle() {}
    ~SceneTitle() override {}

    //������
    void Initialize() override;

    //�I����
    void Finalize() override;

    //�X�V����
    void Update(float elapsedTime) override;

    //�`�揈��
    void Render() override;

private:
    std::unique_ptr<Sprite> sprite = nullptr;
    std::unique_ptr<Ui> ui = nullptr;
};