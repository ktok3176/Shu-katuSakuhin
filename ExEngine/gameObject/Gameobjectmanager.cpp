/*!
 *@brief	CGameObject�̃}�l�[�W��
 */
#include "stdafx.h"
#include "ExEngine.h"
#include "GameObjectManager.h"


GameObjectManager* GameObjectManager::m_instance = nullptr;

GameObjectManager::GameObjectManager()
{
	m_shadowMap.Init(filePath::TKM_CHAEACTER_MODEL, filePath::TKM_BG);
	bloom.Init();
	if (m_instance != nullptr) {
		//�C���X�^���X�����łɍ���Ă���B
		std::abort();
	}
	m_instance = this;
}
GameObjectManager::~GameObjectManager()
{
	m_instance = nullptr;
}
void GameObjectManager::ExecuteUpdate()
{
	//���S�t���O�����Ă���Q�[���I�u�W�F�N�g��j������B
	for (auto& goList : m_gameObjectListArray) {
		goList.remove_if([&](IGameObject* go) {
			if (go->IsDead()) {
				delete go;
				return true;
			}
			return false;
			});
	}

	for (auto& goList : m_gameObjectListArray) {
		for (auto& go : goList) {
			go->StartWrapper();
		}
	}


	for (auto& goList : m_gameObjectListArray) {
		for (auto& go : goList) {
			go->UpdateWrapper();
		}
	}
	//�����G���W���̃A�b�v�f�[�g���Ăяo���B
	PhysicsWorld::GetInstance()->Update(1.0f / 60.0f);
}
void GameObjectManager::ExecuteRender(RenderContext& renderContext)
{






	/*qAddRot.SetRotationDegX(g_pad[0]->GetLStickYF());
	g_camera3D->RotateOriginTarget(qAddRot);*/
#if 1
	m_shadowMap.Draw(renderContext);
#else
	// �V���h�E�}�b�v�Ƀ����_�����O
	   // �����_�����O�^�[�Q�b�g���V���h�E�}�b�v�ɕύX����
	renderContext.WaitUntilToPossibleSetRenderTarget(shadowMap);
	renderContext.SetRenderTargetAndViewport(shadowMap);
	renderContext.ClearRenderTargetView(shadowMap);

	// �e���f����`��
	teapotShadowModel.Draw(renderContext, lightCamera);

	// �������݊����҂�
	renderContext.WaitUntilFinishDrawingToRenderTarget(shadowMap);
#endif	
	renderContext.SetRenderTarget(
		g_graphicsEngine->GetCurrentFrameBuffuerRTV(),
		g_graphicsEngine->GetCurrentFrameBuffuerDSV()
	);
	renderContext.SetViewportAndScissor(g_graphicsEngine->GetFrameBufferViewport());
	bloom.PreparationRT(renderContext);

	for (auto& goList : m_gameObjectListArray) {
		for (auto& go : goList) {
			go->RenderWrapper(renderContext);

		}
	}
	bloom.PreparationDraw(renderContext);
	renderContext.SetRenderTarget(
		g_graphicsEngine->GetCurrentFrameBuffuerRTV(),
		g_graphicsEngine->GetCurrentFrameBuffuerDSV()
	);
	bloom.FinalDraw(renderContext);
}

