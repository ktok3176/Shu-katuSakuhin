#include "stdafx.h"
#include "Gaussianblur.h"

void Gaussianblur::Init(Texture* originalTexture)
{
	m_originalTexture = originalTexture;

	//レンダリングターゲットを初期化。
	InitRenderTargets();
	//スプライトを初期化。
	InitSprites();
}
void Gaussianblur::Render(RenderContext& rc, float blurPower)
{
	//重みテーブルを更新する。
	UpdateWeightsTable(blurPower);

	//横ブラーを実行。
	//レンダリングターゲットとして利用できるようになるまでwaitを入れる。
	rc.WaitUntilToPossibleSetRenderTarget(m_xBlurRenderTarget);
	//レンダリングターゲットを設定。
	rc.SetRenderTargetAndViewport(m_xBlurRenderTarget);
	//レンダリングターゲットをクリア。
	rc.ClearRenderTargetView(m_xBlurRenderTarget);
	//ドロー。
	m_xBlurSprite.Draw(rc);
	//レンダリングターゲットへの書き込み終了待ち。
	rc.WaitUntilFinishDrawingToRenderTarget(m_xBlurRenderTarget);

	//縦ブラーを実行。
	//レンダリングターゲットとして利用できるようになるまでwaitを入れる。
	rc.WaitUntilToPossibleSetRenderTarget(m_yBlurRenderTarget);
	//レンダリングターゲットを設定。
	rc.SetRenderTargetAndViewport(m_yBlurRenderTarget);
	//レンダリングターゲットをクリア。
	rc.ClearRenderTargetView(m_yBlurRenderTarget);
	//ドロー。
	m_yBlurSprite.Draw(rc);
	//レンダリングターゲットへの書き込み終了待ち。
	rc.WaitUntilFinishDrawingToRenderTarget(m_yBlurRenderTarget);
}

void Gaussianblur::InitRenderTargets()
{
	////メイン
	//mainRenderTarget.Create(
	//	1280,
	//	720,
	//	1,
	//	1,
	//	DXGI_FORMAT_R8G8B8A8_UNORM,
	//	DXGI_FORMAT_D32_FLOAT
	//);
	//横ブラー
	m_xBlurRenderTarget.Create(
		m_originalTexture->GetWidth() / 2,
		m_originalTexture->GetHeight(),
		1,
		1,
		m_originalTexture->GetFormat(),
		DXGI_FORMAT_D32_FLOAT
	);
 //縦ブラー
	m_yBlurRenderTarget.Create(
		m_originalTexture->GetWidth() / 2,
		m_originalTexture->GetHeight() / 2,
		1,
		1,
		m_originalTexture->GetFormat(),
		DXGI_FORMAT_D32_FLOAT
	);

}

void Gaussianblur::InitSprites()
{//横
	m_xBlurSpriteInitData.m_fxFilePath = "Assets/shader/preset/gaussianBlur.fx";
	m_xBlurSpriteInitData.m_vsEntryPointFunc = "VSXBlur";
	m_xBlurSpriteInitData.m_psEntryPoinFunc = "PSBlur";

	//スプライトの解像度はxBlurRenderTargetと同じ。
	m_xBlurSpriteInitData.m_width = m_xBlurRenderTarget.GetWidth();
	m_xBlurSpriteInitData.m_height = m_xBlurRenderTarget.GetHeight();
	//テクスチャは輝度テクスチャ。
	m_xBlurSpriteInitData.m_textures[0] = m_originalTexture;

	//【注目】ユーザー拡張の定数バッファに重みテーブルを設定する。
	m_xBlurSpriteInitData.m_expandConstantBuffer = &m_weights;
	m_xBlurSpriteInitData.m_expandConstantBufferSize = sizeof(m_weights);

	m_xBlurSprite.Init(m_xBlurSpriteInitData);

//縦
	m_yBlurSpriteInitData.m_fxFilePath = "Assets/shader/preset/gaussianBlur.fx";
	m_yBlurSpriteInitData.m_vsEntryPointFunc = "VSYBlur";
	m_yBlurSpriteInitData.m_psEntryPoinFunc = "PSBlur";
	//スプライトの幅と高さはyBlurRenderTargetと同じ。
	m_yBlurSpriteInitData.m_width = m_yBlurRenderTarget.GetWidth();
	m_yBlurSpriteInitData.m_height = m_yBlurRenderTarget.GetHeight();
	//テクスチャは横ブラーをかけたもの。
	m_yBlurSpriteInitData.m_textures[0] = &m_xBlurRenderTarget.GetRenderTargetTexture();

	//ユーザー拡張の定数バッファに重みテーブルを設定する。
	m_yBlurSpriteInitData.m_expandConstantBuffer = &m_weights;
	m_yBlurSpriteInitData.m_expandConstantBufferSize = sizeof(m_weights);

	m_yBlurSprite.Init(m_yBlurSpriteInitData);
}

void Gaussianblur::UpdateWeightsTable(float blurPower)
{
	float total = 0;
	for (int i = 0; i < NUM_WEIGHTS; i++) {
		m_weights[i] = expf(-0.5f * (float)(i * i) / blurPower);
		total += 2.0f * m_weights[i];

	}
	// 規格化
	for (int i = 0; i < NUM_WEIGHTS; i++) {
		m_weights[i] /= total;
	}
}
