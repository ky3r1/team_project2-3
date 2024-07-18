#pragma once

#include "Scene.h"

//�V�[���}�l�[�W���[
class SceneManager
{
private:
    SceneManager() {}
    ~SceneManager() {}
public:
    //�B��̃C���X�^���X
    static SceneManager& Instance()
    {
        static SceneManager instance;
        return instance;
    }

    //�X�V����
    void Update(float elapsedTime);

    //�`�揈��
    void Render();

    void Render(Scene* scene);
    //�V�[���N���A
    void Clear();

    //�V�[���؂�ւ�
    void ChangeScene(Scene* scene);

    void HoldChangeScene(Scene* scene);

    bool GetGameLoop() {
        return game_loop;
    }

    void ChangeGameLoop() { game_loop = !game_loop; }

    Scene* destScene = nullptr;
private:
    bool game_loop = true;
    Scene* currentScene = nullptr;
    Scene* nextScene = nullptr;
};