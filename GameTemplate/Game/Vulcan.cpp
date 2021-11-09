#include "stdafx.h"
#include "Vulcan.h"

Vulcan::Vulcan() {

}

Vulcan::~Vulcan() {
	DeleteGO(m_modelRender[0]);
	DeleteGO(m_modelRender[1]);
	DeleteGO(m_modelRender[2]);
}

bool Vulcan::Start() {
	m_modelRender[0] = NewGO<ModelRender>(0);
	m_modelRender[0]->SetDirectiontColor(m_dirPower);
	m_modelRender[0]->Init(filePath::TKM_VULCAN);
	m_modelRender[0]->SetPosition(m_vulcanPos[0]);
	m_vulcanPos[0].y += 45.0f;
	m_moveSpeed = 20.0f;
	return true;
}

void Vulcan::Update() {

	m_vulcanPos[0].x -= m_moveSpeed;
	m_vulcanLifeTime += 1;
	m_modelRender[0]->SetPosition(m_vulcanPos[0]);

	if (m_vulcanLifeTime == 3) {		
		m_modelRender[1] = NewGO<ModelRender>(0);
		m_modelRender[1]->SetDirectiontColor(m_dirPower);
		m_modelRender[1]->Init(filePath::TKM_VULCAN);
		m_modelRender[1]->SetPosition(m_vulcanPos[1]);
		m_generateVulcan[0] = true;
		m_vulcanPos[1].y += 45.0f;


	}
	if (m_vulcanLifeTime == 5) {
		m_modelRender[2] = NewGO<ModelRender>(0);
		m_modelRender[2]->SetDirectiontColor(m_dirPower);
		m_modelRender[2]->Init(filePath::TKM_VULCAN);
		m_modelRender[2]->SetPosition(m_vulcanPos[2]);
		m_generateVulcan[1] = true;
		m_vulcanPos[2].y += 45.0f;

	}
	
	if (m_generateVulcan[0] == true) {
		m_vulcanPos[1].x -= m_moveSpeed;
		m_modelRender[1]->SetPosition(m_vulcanPos[1]);
	}
	if (m_generateVulcan[1] == true) {
		m_vulcanPos[2].x -= m_moveSpeed;
		m_modelRender[2]->SetPosition(m_vulcanPos[2]);
	}


	if (m_vulcanLifeTime > 60) {
		
		DeleteGO(this);
	}

}