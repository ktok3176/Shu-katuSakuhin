#include "stdafx.h"
#include "Player.h"

Player::Player()
{

}

Player::~Player()
{
	DeleteGO(m_modelRender);
	DeleteGO(m_vulcan);
	DeleteGO(m_beam);
	DeleteGO(m_bomb);
}

bool Player::Start()
{
	m_modelRender = NewGO<ModelRender>(0);
	m_modelRender->Init(filePath::TKM_CHAEACTER_MODEL);
	m_modelRender->SetPosition(m_playerPos);
	m_modelRender->SetRotationY(-70.0f);

	return true;
}

void Player::Update()
{
	Move();
	if (m_weponCanUseFlag == false) {
		m_weponCanUseTimer += 1;
		if (m_weponCanUseTimer >= 20) {
			m_weponCanUseFlag = true;
			m_weponCanUseTimer = 0;
		}
	}
	if (m_weponCanUseFlag == true) {
		if (g_pad[0]->IsTrigger(enButtonA))
		{
			VulcanShot();
			m_weponCanUseFlag = false;
		}

		if (g_pad[0]->IsPress(enButtonY))
		{
			BeamShot();
			m_weponCanUseFlag = false;
		}

		if (g_pad[0]->IsTrigger(enButtonB))
		{
			BombShot();
			m_weponCanUseFlag = false;
		}
	}
}

void Player::Move() {
	if (m_playerPos.y < -400.0f) {

		m_gravity = 0.0f;
	}

	if (g_pad[0]->GetLStickYF() >= 0.7f) {
		m_gravity = 0.0f;
	}

	if (g_pad[0]->GetLStickXF() != 0.0f || g_pad[0]->GetLStickYF() != 0.0f) {

		if (m_boost <= 3.5f) {
			m_boost += 0.1f;
		}
	}
	if (g_pad[0]->GetLStickXF() == 0.0f && g_pad[0]->GetLStickYF() == 0.0f) {
		m_boost = 1.0f;
	}
	
	m_playerPos.x -= g_pad[0]->GetLStickXF() * m_boost * m_boost;
	m_playerPos.y += g_pad[0]->GetLStickYF() * m_boost * m_boost;
	if (m_playerPos.y <= -400.0f) {
		m_playerPos.y = -400.0f;
	
	}
	if (m_playerPos.y >= 350.0f) {
		m_playerPos.y = 350.0f;

	}
	if (m_playerPos.y > -400.0f) {

		if (m_gravity <= 5.0f && g_pad[0]->GetLStickYF() == 0.0f && g_pad[0]->GetLStickXF() == 0.0f) {
			m_gravity += 0.2f;
		}
		m_playerPos.y -= m_gravity * m_gravity;
	}



	m_modelRender->SetPosition(m_playerPos);
}

void Player::VulcanShot() {
	m_vulcan = NewGO<Vulcan>(0);
	m_vulcan->SetPosition(m_playerPos);	
}

void Player::BeamShot() {
	m_beam = NewGO<Beam>(0);
	m_beam->SetPosition(m_playerPos);
}

void Player::BombShot() {
	m_bomb = NewGO<Bomb>(0);
	m_bomb->SetPosition(m_playerPos);
}