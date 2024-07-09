#pragma once
#include "All.h"
#include "Graphics/Sprite.h"
class Ui
{
public:
	Ui() {}
	~Ui() {}

    //唯一のインスタンス取得
    static Ui& Instance()
    {
        static Ui instance;
        return instance;
    }

    void title(ID3D11DeviceContext* dc);
    void game(ID3D11DeviceContext* dc);
    void result(ID3D11DeviceContext* dc);

private:
    std::unique_ptr<Sprite> sprite01 = nullptr;
    std::unique_ptr<Sprite> sprite02 = nullptr;
    std::unique_ptr<Sprite> sprite03 = nullptr;
};