#include "stdafx.h"
#include "ShadowMap.h"
void ShadowMap::Init(const char* filePath, const char* BGfilePath) {
	m_tkmFilePath = filePath;
	m_bgTkmFilePath = BGfilePath;
	InitShadow(filePath, BGfilePath);
}
void ShadowMap::InitShadow(const char* filePath, const char* BGfilePath) {
	// カメラの位置を設定。これはライトの位置
	m_lightCamera.SetPosition(m_cameraSetPos);

	// カメラの注視点を設定。これがライトが照らしている場所
	m_lightCamera.SetTarget(m_cameraSetTarget);

	// 上方向を設定。今回はライトが真下を向いているので、X方向を上にしている
	m_lightCamera.SetUp(m_cameraSetUp);

	// ライトビュープロジェクション行列を計算している
	m_lightCamera.Update();

	//レンダリングターゲット初期化
	m_shadowMap.Create(
		m_wide,//レンダリングターゲットの幅
		m_high,//レンダリングターゲットの高さ
		m_mipLv,//ミップマップレベル
		m_arraySize,//テクスチャ配列のサイズ
		//シャドウマップのカラーバッファのフォーマットを変更している。
		DXGI_FORMAT_R32_FLOAT,
		DXGI_FORMAT_D32_FLOAT,
		m_clearColor
	);
	//シャドウマップ描画用のシェーダーを指定する。
	m_shadowModelInitData.m_fxFilePath = "Assets/shader/sampleDrawShadowMap.fx";
	m_shadowModelInitData.m_tkmFilePath = filePath;

	//カラーバッファのフォーマットに変更が入ったので、こちらも変更する。
	m_shadowModelInitData.m_colorBufferFormat[0] = DXGI_FORMAT_R32_FLOAT;


	m_shadowModel.Init(m_shadowModelInitData);
	m_shadowModel.UpdateWorldMatrix(
		m_shadowPos,
		g_quatIdentity,
		g_vec3One
	);
	// シャドウレシーバー（影が落とされるモデル）用のシェーダーを指定する
	m_bgModelInitData.m_fxFilePath = "Assets/shader/sampleShadowReciever.fx";

	// シャドウマップを拡張SRVに設定する
	m_bgModelInitData.m_expandShaderResoruceView = &m_shadowMap.GetRenderTargetTexture();

	// ライトビュープロジェクション行列を拡張定数バッファーに設定する
	m_bgModelInitData.m_expandConstantBuffer = (void*)&m_lightCamera.GetViewProjectionMatrix();
	m_bgModelInitData.m_expandConstantBufferSize = sizeof(m_lightCamera.GetViewProjectionMatrix());
	m_bgModelInitData.m_tkmFilePath = BGfilePath;

	m_bgModel.Init(m_bgModelInitData);
}

void ShadowMap::Draw(RenderContext& renderContext) {
	
	m_shadowPos.z -= g_pad[0]->GetLStickYF();
	m_shadowPos.x -= g_pad[0]->GetLStickXF();


	m_shadowModel.UpdateWorldMatrix(
		m_shadowPos,
		g_quatIdentity,
		g_vec3One
	);

	// シャドウマップにレンダリング
	   // レンダリングターゲットをシャドウマップに変更する
	renderContext.WaitUntilToPossibleSetRenderTarget(m_shadowMap);
	renderContext.SetRenderTargetAndViewport(m_shadowMap);
	renderContext.ClearRenderTargetView(m_shadowMap);

	// 影モデルを描画
	m_shadowModel.Draw(renderContext, m_lightCamera);

	// 書き込み完了待ち
	renderContext.WaitUntilFinishDrawingToRenderTarget(m_shadowMap);
	
}
void ShadowMap::BGDraw(RenderContext& renderContext) {
	m_bgModel.Draw(renderContext);
}