#include "stdafx.h"
#include "Bomb.h"
Bomb::Bomb() {

}

Bomb::~Bomb() {
	DeleteGO(m_modelRender[0]);
	DeleteGO(m_modelRender[1]);
	DeleteGO(m_modelRender[2]);
}

bool Bomb::Start() {
	m_modelRender[0] = NewGO<ModelRender>(0);
	m_modelRender[0]->SetDirectiontColor(m_dirPower);
	m_modelRender[0]->Init(filePath::TKM_BOMB);
	m_modelRender[0]->SetPosition(m_bombPos[0]);
	m_bombPos[0].y += 45.0f;
	m_moveSpeed = 20.0f;
	return true;
}

void Bomb::Update() {

	m_bombPos[0].x -= m_moveSpeed;
	m_bombLifeTime += 1;
	m_modelRender[0]->SetPosition(m_bombPos[0]);

	if (m_bombLifeTime == 3) {
		m_modelRender[1] = NewGO<ModelRender>(0);
		m_modelRender[1]->SetDirectiontColor(m_dirPower);
		m_modelRender[1]->Init(filePath::TKM_BOMB);
		m_modelRender[1]->SetPosition(m_bombPos[1]);
		m_generateBomb[0] = true;
		m_bombPos[1].y += 45.0f;


	}
	if (m_bombLifeTime == 5) {
		m_modelRender[2] = NewGO<ModelRender>(0);
		m_modelRender[2]->SetDirectiontColor(m_dirPower);
		m_modelRender[2]->Init(filePath::TKM_BOMB);
		m_modelRender[2]->SetPosition(m_bombPos[2]);
		m_generateBomb[1] = true;
		m_bombPos[2].y += 45.0f;

	}

	if (m_generateBomb[0] == true) {
		m_bombPos[1].x -= m_moveSpeed;
		m_modelRender[1]->SetPosition(m_bombPos[1]);
	}
	if (m_generateBomb[1] == true) {
		m_bombPos[2].x -= m_moveSpeed;
		m_modelRender[2]->SetPosition(m_bombPos[2]);
	}


	if (m_bombLifeTime > 60) {

		DeleteGO(this);
	}
}