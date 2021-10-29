#include "stdafx.h"
#include "Game.h"

namespace //constant
{
	////////////////////////////////////////////////////////////
	// �ʒu���
	////////////////////////////////////////////////////////////

	const Vector3 STAGE_POSITION = //�v���C���[�̏������W
	{ 0.0f, 0.0f, 0.0f };											//�v���C���[�P
										//�v���C���[�S

}

Game::Game()
{

}

Game::~Game()
{
    
}

bool Game::Start()
{
	m_modelRender = NewGO<ModelRender>(0);
	m_modelRender->Init(filePath::TKM_TEAPOT);
	m_modelRender->SetPosition(potPos);
	/*m_shadowMap = NewGO<ShadowMap>(0);
	m_shadowMap->Init(filePath::TKM_TEAPOT, filePath::TKM_BG);*/
	//m_modelRender->SetPointColor({ 0.0f,0.0f,0.0f });
	//m_modelRender->SetScale({ 1.0f,1.0f,1.0f });



	/*m_bgmodelRender = NewGO<ModelRender>(0);
	m_bgmodelRender->Init(filePath::TKM_BG);*/


    return true;

}

void Game::Update()
{
	potPos.x -= g_pad[0]->GetLStickXF();
	if (g_pad[0]->IsPress(enButtonB)) {
		potPos.y += g_pad[0]->GetLStickYF();
	}
	else {
		potPos.z -= g_pad[0]->GetLStickYF();
	}
	m_modelRender->SetPosition(potPos);

}