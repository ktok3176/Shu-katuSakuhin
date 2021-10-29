/*!
 *@brief	CGameObjectのマネージャ
 */
#include "stdafx.h"
#include "ExEngine.h"
#include "GameObjectManager.h"


GameObjectManager* GameObjectManager::m_instance = nullptr;

GameObjectManager::GameObjectManager()
{
	m_shadowMap.Init(filePath::TKM_TEAPOT, filePath::TKM_BG);
	bloom.Init();
	if (m_instance != nullptr) {
		//インスタンスがすでに作られている。
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
	//死亡フラグがついているゲームオブジェクトを破棄する。
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
	//物理エンジンのアップデートを呼び出す。
	PhysicsWorld::GetInstance()->Update(1.0f / 60.0f);
}
void GameObjectManager::ExecuteRender(RenderContext& renderContext)
{






	qAddRot.SetRotationDegX(g_pad[0]->GetLStickYF());
	g_camera3D->RotateOriginTarget(qAddRot);
#if 1
	m_shadowMap.Draw(renderContext);
#else
	// シャドウマップにレンダリング
	   // レンダリングターゲットをシャドウマップに変更する
	renderContext.WaitUntilToPossibleSetRenderTarget(shadowMap);
	renderContext.SetRenderTargetAndViewport(shadowMap);
	renderContext.ClearRenderTargetView(shadowMap);

	// 影モデルを描画
	teapotShadowModel.Draw(renderContext, lightCamera);

	// 書き込み完了待ち
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
			// 影を受ける背景を描画

		}
	}
	m_shadowMap.BGDraw(renderContext);
	
	bloom.PreparationDraw(renderContext);
	renderContext.SetRenderTarget(
		g_graphicsEngine->GetCurrentFrameBuffuerRTV(),
		g_graphicsEngine->GetCurrentFrameBuffuerDSV()
	);
	bloom.FinalDraw(renderContext);

}

