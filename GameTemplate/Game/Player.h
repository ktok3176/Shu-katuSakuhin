#pragma once
#include "model_render.h"
#include "constant.h"
#include "Vulcan.h"
#include "Beam.h"
#include "Bomb.h"
class Vulcan;
class Beam;
class Bomb;
class Player : public IGameObject
{
public:
    Player();
    ~Player();
    bool Start() override final;
    void Update() override final;

    void Move();

    void VulcanShot();
    void BeamShot();
    void BombShot();

private:
    ModelRender* m_modelRender = nullptr;	//プレイヤーキャラクターのモデル
    ModelRender* m_modelRender1 = nullptr;	//プレイヤーキャラクターのモデル
    Vector3 m_playerPos = { 0.0f,50.0f,0.0f };


    Vulcan* m_vulcan = nullptr;
    Beam* m_beam = nullptr;
    Bomb* m_bomb = nullptr;
  
    
    bool m_weponCanUseFlag = true;
    int m_weponCanUseTimer = 0;


    //?何に使うのか忘れたのでいったんおいとく
    bool m_vulcanFlag = true;
    int m_vulcunTimer = 0;

    bool m_beamFlag = true;
    int m_beamTimer = 0;

    bool m_bombFlag = true;
    int m_bombTimer = 0;
    //?

    float m_gravity = 0.5f;
    float m_boost = 1.0f;

public://Get関数
    const Vector3 GetPosition() {
        return m_playerPos;
    }

};

