#pragma once
#include "model_render.h"
#include "constant.h"
#include "Player.h"

class Game : public IGameObject
{
public:
    Game();
    ~Game();
    bool Start() override final;
    void Update() override final;


private:
    Player* m_player = nullptr;

};


