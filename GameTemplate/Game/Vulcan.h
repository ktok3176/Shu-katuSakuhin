#pragma once
#include "model_render.h"
#include "constant.h"

const int VULCANNUMBER=3;

class Vulcan : public IGameObject
{
public:
	Vulcan();
	~Vulcan();
	bool Start() override final;
	void Update() override final;
private:

	ModelRender* m_modelRender[VULCANNUMBER] = { nullptr };
	Vector3 m_vulcanPos[VULCANNUMBER] = { g_vec3Zero ,g_vec3Zero ,g_vec3Zero };
	int m_vulcanLifeTime = 0;
	Vector3 m_dirPower = { 12.0f,12.0f,12.0f };
	float m_moveSpeed = 0.0f;	//à⁄ìÆë¨ìxÅB
	bool m_generateVulcan[2] = { false,false };

public:
	void SetPosition(Vector3 v) {
		m_vulcanPos[0] = v;
		m_vulcanPos[1] = v;
		m_vulcanPos[2] = v;
	}

};

