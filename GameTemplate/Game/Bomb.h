#pragma once
#include "model_render.h"
#include "constant.h"

const int BOMBNUMBER = 3;

class Bomb : public IGameObject
{
public:
	Bomb();
	~Bomb();
	bool Start() override final;
	void Update() override final;
private:

	ModelRender* m_modelRender[BOMBNUMBER] = { nullptr };
	Vector3 m_bombPos[BOMBNUMBER] = { g_vec3Zero ,g_vec3Zero ,g_vec3Zero };
	int m_bombLifeTime = 0;
	Vector3 m_dirPower = { 30.0f,30.0f,30.0f };
	float m_moveSpeed = 0.0f;	//à⁄ìÆë¨ìxÅB
	bool m_generateBomb[2] = { false,false };

public:
	void SetPosition(Vector3 v) {
		m_bombPos[0] = v;
		m_bombPos[1] = v;
		m_bombPos[2] = v;
	}

};
