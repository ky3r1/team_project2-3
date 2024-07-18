#pragma once

#include "Graphics/Sprite.h"
#include "Scene.h"
#include "Audio/Audio.h"
#include "Graphics/Font.h"
//#include "Player.h"

//�^�C�g���V�[��
class SceneResult : public Scene
{
public:
    SceneResult(bool oc) { outcome = oc; }
    ~SceneResult() override {}

    //������
    void Initialize() override;

    //�I����
    void Finalize() override;

    //�X�V����
    void Update(float elapsedTime) override;

    //�`�揈��
    void Render() override;

private:
    std::unique_ptr<Font> font = nullptr;
    std::unique_ptr<Sprite> sprite = nullptr;
    std::unique_ptr<AudioSource> audio = nullptr;
    bool outcome = false;
};