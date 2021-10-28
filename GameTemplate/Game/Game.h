#pragma once
#include "model_render.h"
#include "constant.h"
class Game : public IGameObject
{
public:
    Game();
    ~Game();
    bool Start() override final;
    void Update() override final;


private:
    ModelRender* m_modelRender = nullptr;	//�v���C���[�L�����N�^�[�̃��f��
    ModelRender* m_bgmodelRender=  nullptr ;	//�X�e�[�W�̃��f��
    Vector3 potPos = { 0.0f,50.0f,0.0f};
};


