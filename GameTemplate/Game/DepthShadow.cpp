#include "stdafx.h"
#include "DepthShadow.h"

void DepthShadow::Init(const char* filePath, const char* BGfilePath) {
	m_tkmFilePath = filePath;
	m_bgTkmFilePath = BGfilePath;
	InitShadow(filePath, BGfilePath);
}
void DepthShadow::InitShadow(const char* filePath, const char* BGfilePath) {
	// カメラの位置を設定。これはライトの位置
	lightCamera.SetPosition(cameraSetPos);

	// カメラの注視点を設定。これがライトが照らしている場所
	lightCamera.SetTarget(cameraSetTarget);

	// 上方向を設定。今回はライトが真下を向いているので、X方向を上にしている
	lightCamera.SetUp(cameraSetUp);

	// ライトビュープロジェクション行列を計算している
	lightCamera.Update();

	InitRenderTargets();

	//シャドウマップ描画用のシェーダーを指定する。
	m_shadowModelInitData.m_fxFilePath = "Assets/shader/sampleDrawShadowMap.fx";
	m_shadowModelInitData.m_tkmFilePath = filePath;

	//【注目】カラーバッファのフォーマットに変更が入ったので、こちらも変更する。
	m_shadowModelInitData.m_colorBufferFormat[0] = DXGI_FORMAT_R32_FLOAT;


	m_shadowModel.Init(m_shadowModelInitData);
	m_shadowModel.UpdateWorldMatrix(
		m_shadow,
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
	bgModelInitData.m_tkmFilePath = BGfilePath;

	bgModel.Init(bgModelInitData);
}

void DepthShadow::Render(RenderContext& renderContext) {
	bgModel.Draw(renderContext);
}
void DepthShadow::InitRenderTargets() {
	shadowMap.Create(
		1024,
		1024,
		1,
		1,
		DXGI_FORMAT_R32_FLOAT,
		DXGI_FORMAT_D32_FLOAT,
		clearColor
	);
}
