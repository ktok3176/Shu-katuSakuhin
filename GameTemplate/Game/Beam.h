#pragma once
#include "model_render.h"
#include "constant.h"
class Beam : public IGameObject
{
public:
	Beam();
	~Beam();
	bool Start() override final;
	void Update() override final;
private:
	ModelRender* m_modelRender = nullptr;
	Vector3 m_beamPos = g_vec3Zero;
	Vector3 m_beamScale = { 1.3f,1.3f,1.3f };
	Vector3 m_dirPower = { 6.5f,6.5f,6.5f };
	float m_moveSpeed = 0.0f;	//à⁄ìÆë¨ìxÅB
	int m_beamLifeTime = 0;


public:
	void SetPosition(Vector3 v) {
		m_beamPos = v;
	}

};

