#pragma once

/// <summary>
/// ガウシアンブラークラス。
/// </summary>
class Gaussianblur {
public:
	/// <summary>
	/// 初期化。
	/// </summary>
	/// <param name="originalTexture">ガウシアンブラーをかけるオリジナルテクスチャ。</param>
	void Init(Texture* originalTexture);
	/// <summary>
/// ガウシアンブラーをGPU上で実行。
/// </summary>
/// <remarks>
/// 本関数の呼び出しは、DirectX12を利用した描画コマンド生成中に呼び出す必要があります。
/// </remarks>
/// <param name="rc">レンダリングターゲット</param>
/// <param name="blurPower">ブラーの強さ。値が大きいほどボケが強くなる。</param>
	void Render(RenderContext& rc, float blurPower);
	/// <summary>
	/// ボケテクスチャを取得。
	/// </summary>
	/// <returns></returns>
	Texture& GetBokeTexture()
	{
		return m_yBlurRenderTarget.GetRenderTargetTexture();
	}

private:
	/// <summary>
	/// レンダリングターゲットを初期化。
	/// </summary>
	void InitRenderTargets();
	/// <summary>
	/// スプライトを初期化。
	/// </summary>
	void InitSprites();
	/// <summary>
	/// 重みテーブルを更新する。
	/// </summary>
	void UpdateWeightsTable(float blurPower);
private:
	int m_mipLv = 1;
	int m_arraySize = 1;
	enum { NUM_WEIGHTS = 8 };				//重みの数。
	float m_weights[NUM_WEIGHTS] = {0,0,0,0,0,0,0,0};			//重みテーブル。			//重みテーブル。
	Texture* m_originalTexture = nullptr;	//オリジナルテクスチャ。
	//RenderTarget mainRenderTarget;
	RenderTarget m_xBlurRenderTarget;		//横ボケ画像を描画するレンダリングターゲット。
	RenderTarget m_yBlurRenderTarget;		//縦ボケ画像を描画するレンダリングターゲット。
	
	SpriteInitData m_xBlurSpriteInitData;
	SpriteInitData m_yBlurSpriteInitData;
	
	Sprite m_xBlurSprite;					//横ボケ画像を描画するためのスプライト。
	Sprite m_yBlurSprite;					//縦ボケ画像を描画するためのスプライト。
};

