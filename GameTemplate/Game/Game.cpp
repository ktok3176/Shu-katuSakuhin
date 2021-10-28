#include "stdafx.h"
#include "Game.h"

namespace //constant
{
	////////////////////////////////////////////////////////////
	// 位置情報
	////////////////////////////////////////////////////////////

	const Vector3 STAGE_POSITION = //プレイヤーの初期座標
	{ 0.0f, 0.0f, 0.0f };											//プレイヤー１
										//プレイヤー４

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