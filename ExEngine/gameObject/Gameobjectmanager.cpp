/*!
 *@brief	CGameObjectのマネージャ
 */
#include "stdafx.h"
#include "ExEngine.h"
#include "GameObjectManager.h"


GameObjectManager* GameObjectManager::m_instance = nullptr;

GameObjectManager::GameObjectManager()
{
	InitRenderTargets();
	InitSprites();
	InitShadow();
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
	PhysicsWorld::GetInstance()->Update(1.0f/60.0f);
}
void GameObjectManager::ExecuteRender(RenderContext& renderContext)
{
	
	
		
	
	
	teaShadow.z -= g_pad[0]->GetLStickYF();
	teaShadow.x -= g_pad[0]->GetLStickXF();

	
	teapotShadowModel.UpdateWorldMatrix(
		teaShadow,
		g_quatIdentity,
		g_vec3One
	);
	qAddRot.SetRotationDegX(g_pad[0]->GetLStickYF());
	g_camera3D->RotateOriginTarget(qAddRot);
	// シャドウマップにレンダリング
	   // レンダリングターゲットをシャドウマップに変更する
	renderContext.WaitUntilToPossibleSetRenderTarget(shadowMap);
	renderContext.SetRenderTargetAndViewport(shadowMap);
	renderContext.ClearRenderTargetView(shadowMap);

	// 影モデルを描画
	teapotShadowModel.Draw(renderContext, lightCamera);

	// 書き込み完了待ち
	renderContext.WaitUntilFinishDrawingToRenderTarget(shadowMap);
	// 通常レンダリング
	  // レンダリングターゲットをフレームバッファに戻す
	renderContext.SetRenderTarget(
		g_graphicsEngine->GetCurrentFrameBuffuerRTV(),
		g_graphicsEngine->GetCurrentFrameBuffuerDSV()
	);
	renderContext.SetViewportAndScissor(g_graphicsEngine->GetFrameBufferViewport());

	//レンダラーを変更するならここを改造していくと良い。
	// step-1 レンダリングターゲットをmainRenderTargetに設定する
	renderContext.WaitUntilToPossibleSetRenderTarget(mainRenderTarget);
	// レンダリングターゲットを設定
	renderContext.SetRenderTargetAndViewport(mainRenderTarget);
	// レンダリングターゲットをクリア
	renderContext.ClearRenderTargetView(mainRenderTarget);
	for (auto& goList : m_gameObjectListArray) {
		for (auto& go : goList) {
			go->RenderWrapper(renderContext);
			// 影を受ける背景を描画
			
		}
	}
	bgModel.Draw(renderContext);
	
	// レンダリングターゲットへの書き込み終了待ち
	renderContext.WaitUntilFinishDrawingToRenderTarget(mainRenderTarget);
	// step-2 ブルームの処理
	// 輝度抽出→ブラー→合成
	// 輝度抽出
	// 輝度抽出用のレンダリングターゲットに変更
	renderContext.WaitUntilToPossibleSetRenderTarget(luminnceRenderTarget);

	// レンダリングターゲットを設定
	renderContext.SetRenderTargetAndViewport(luminnceRenderTarget);

	// レンダリングターゲットをクリア
	renderContext.ClearRenderTargetView(luminnceRenderTarget);

	// 輝度抽出を行う
	luminanceSprite.Draw(renderContext);

	// レンダリングターゲットへの書き込み終了待ち
	renderContext.WaitUntilFinishDrawingToRenderTarget(luminnceRenderTarget);

	// step-3 ガウシアンブラーを4回実行する
	gaussianBlur[0].Render(renderContext, 10);
	gaussianBlur[1].Render(renderContext, 10);
	gaussianBlur[2].Render(renderContext, 10);
	gaussianBlur[3].Render(renderContext, 10);
	// step-4 ボケ画像を合成してメインレンダリングターゲットに加算合成
	renderContext.WaitUntilToPossibleSetRenderTarget(mainRenderTarget);
	renderContext.SetRenderTargetAndViewport(mainRenderTarget);
	finalSprite.Draw(renderContext);
	renderContext.WaitUntilFinishDrawingToRenderTarget(mainRenderTarget);
	// step-3 メインレンダリングターゲットの内容をフレームバッファにコピーする
	renderContext.SetRenderTarget(
		g_graphicsEngine->GetCurrentFrameBuffuerRTV(),
		g_graphicsEngine->GetCurrentFrameBuffuerDSV()
	);
	//bgModel.Draw(renderContext);

	copyToFrameBufferSprite.Draw(renderContext);

}

void GameObjectManager::InitRenderTargets() {
	mainRenderTarget.Create(
		1280,
		720,
		1,
		1,
		// 【注目】カラーバッファーのフォーマットを32bit浮動小数点にしている
		DXGI_FORMAT_R32G32B32A32_FLOAT,
		DXGI_FORMAT_D32_FLOAT
	);

	luminnceRenderTarget.Create(
		1280,       // 解像度はメインレンダリングターゲットと同じ
		720,        // 解像度はメインレンダリングターゲットと同じ
		1,
		1,
		// 【注目】カラーバッファーのフォーマットを32bit浮動小数点にしている
		DXGI_FORMAT_R32G32B32A32_FLOAT,
		DXGI_FORMAT_D32_FLOAT
	);
}
void GameObjectManager::InitSprites() {
	// 輝度抽出用のシェーダーのファイルパスを指定する
	luminanceSpriteInitData.m_fxFilePath = "Assets/shader/samplePostEffect.fx";

	// 頂点シェーダーのエントリーポイントを指定する
	luminanceSpriteInitData.m_vsEntryPointFunc = "VSMain";

	// ピクセルシェーダーのエントリーポイントを指定する
	luminanceSpriteInitData.m_psEntryPoinFunc = "PSSamplingLuminance";

	// スプライトの幅と高さはluminnceRenderTargetと同じ
	luminanceSpriteInitData.m_width = 1280;
	luminanceSpriteInitData.m_height = 720;

	// テクスチャはメインレンダリングターゲットのカラーバッファー
	luminanceSpriteInitData.m_textures[0] = &mainRenderTarget.GetRenderTargetTexture();

	// 描き込むレンダリングターゲットのフォーマットを指定する
	luminanceSpriteInitData.m_colorBufferFormat[0] = DXGI_FORMAT_R32G32B32A32_FLOAT;


	luminanceSprite.Init(luminanceSpriteInitData);

	// ガウシアンブラーを初期化

	gaussianBlur[0].Init(&luminnceRenderTarget.GetRenderTargetTexture());
	gaussianBlur[1].Init(&gaussianBlur[0].GetBokeTexture());
	gaussianBlur[2].Init(&gaussianBlur[1].GetBokeTexture());
	gaussianBlur[3].Init(&gaussianBlur[2].GetBokeTexture());

	// ボケ画像を合成して書き込むためのスプライトを初期化


	finalSpriteInitData.m_textures[0] = &gaussianBlur[0].GetBokeTexture();
	finalSpriteInitData.m_textures[1] = &gaussianBlur[1].GetBokeTexture();
	finalSpriteInitData.m_textures[2] = &gaussianBlur[2].GetBokeTexture();
	finalSpriteInitData.m_textures[3] = &gaussianBlur[3].GetBokeTexture();

	finalSpriteInitData.m_width = 1280;
	finalSpriteInitData.m_height = 720;

	finalSpriteInitData.m_fxFilePath = "Assets/shader/samplePostEffect.fx";
	finalSpriteInitData.m_psEntryPoinFunc = "PSBloomFinal";
	finalSpriteInitData.m_alphaBlendMode = AlphaBlendMode_Add;
	finalSpriteInitData.m_colorBufferFormat[0] = DXGI_FORMAT_R32G32B32A32_FLOAT;

	finalSprite.Init(finalSpriteInitData);
	// mainRenderTargetのテクスチャをフレームバッファーに貼り付けるためのスプライトを初期化する
	// スプライトの初期化オブジェクトを作成する


	// テクスチャはmainRenderTargetのカラーバッファー
	spriteInitData.m_textures[0] = &mainRenderTarget.GetRenderTargetTexture();
	spriteInitData.m_width = 1280;
	spriteInitData.m_height = 720;

	// モノクロ用のシェーダーを指定する
	spriteInitData.m_fxFilePath = "Assets/shader/sample2D.fx";


	copyToFrameBufferSprite.Init(spriteInitData);
}
void GameObjectManager::InitShadow() {

	// カメラの位置を設定。これはライトの位置
	lightCamera.SetPosition(cameraSetPos);

	// カメラの注視点を設定。これがライトが照らしている場所
	lightCamera.SetTarget(cameraSetTarget);

	// 上方向を設定。今回はライトが真下を向いているので、X方向を上にしている
	lightCamera.SetUp(cameraSetUp);

	// ライトビュープロジェクション行列を計算している
	lightCamera.Update();

	// step-1 シャドウマップ描画用のレンダリングターゲットを作成する
	float clearColor[4] = { 1.0f, 1.0f, 1.0f, 1.0f };

	shadowMap.Create(
		1024,
		1024,
		1,
		1,
		//【注目】シャドウマップのカラーバッファのフォーマットを変更している。
		DXGI_FORMAT_R32_FLOAT,
		DXGI_FORMAT_D32_FLOAT,
		clearColor
	);
	// step-2 シャドウマップに描画するモデルを初期化する

	//シャドウマップ描画用のシェーダーを指定する。
	teapotShadowModelInitData.m_fxFilePath = "Assets/shader/sampleDrawShadowMap.fx";
	teapotShadowModelInitData.m_tkmFilePath = "Assets/modelData/teapot.tkm";

	//【注目】カラーバッファのフォーマットに変更が入ったので、こちらも変更する。
	teapotShadowModelInitData.m_colorBufferFormat[0] = DXGI_FORMAT_R32_FLOAT;
	//teaShadow.x -= g_pad[0]->GaetLStickXF();
	//if (g_pad[0]->IsPress(enButtonB)) {
	//	teaShadow.y += g_pad[0]->GetLStickYF();
	//}
	//else {
	//	teaShadow.z -= g_pad[0]->GetLStickYF();
	//}

	teapotShadowModel.Init(teapotShadowModelInitData);
	teapotShadowModel.UpdateWorldMatrix(
		teaShadow,
		g_quatIdentity,
		g_vec3One
	);
	// 影を受ける背景モデルを初期化

	// シャドウレシーバー（影が落とされるモデル）用のシェーダーを指定する
	bgModelInitData.m_fxFilePath = "Assets/shader/sampleShadowReciever.fx";

	// シャドウマップを拡張SRVに設定する
	bgModelInitData.m_expandShaderResoruceView = &shadowMap.GetRenderTargetTexture();

	// ライトビュープロジェクション行列を拡張定数バッファーに設定する
	bgModelInitData.m_expandConstantBuffer = (void*)&lightCamera.GetViewProjectionMatrix();
	bgModelInitData.m_expandConstantBufferSize = sizeof(lightCamera.GetViewProjectionMatrix());
	bgModelInitData.m_tkmFilePath = "Assets/modelData/bg/bg.tkm";

	bgModel.Init(bgModelInitData);
}