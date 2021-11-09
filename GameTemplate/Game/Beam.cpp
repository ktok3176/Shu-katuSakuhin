#include "stdafx.h"
#include "Beam.h"
Beam::Beam() {

}

Beam::~Beam() {
	DeleteGO(m_modelRender);
}
bool Beam::Start() {
	m_modelRender = NewGO<ModelRender>(0);
	m_modelRender->SetDirectiontColor(m_dirPower);
	m_modelRender->Init(filePath::TKM_BEAM);
	
	m_modelRender->SetPosition(m_beamPos);
//	m_modelRender->SetScale(m_beamScale);
	m_beamPos.y += 45.0f;
	m_moveSpeed = 20.0f;
	return true;
}

void Beam::Update() {
	m_beamPos.x -= m_moveSpeed;
	m_beamLifeTime += 1;
	m_modelRender->SetPosition(m_beamPos);

	if (m_beamLifeTime > 60) {

		DeleteGO(this);
	}
}