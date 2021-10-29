#include "stdafx.h"
#include "Bloom.h"

Bloom::Bloom() {

}
Bloom::~Bloom() {

}
bool Bloom::Start() {
	return true;
}
void Bloom::Update() {

}
void Bloom::InitRenderTargets() {
    m_mainRenderTarget.Create(
        m_wide,//レンダリングターゲットの幅
        m_high,//レンダリングターゲットの高さ
        m_mipLv,//ミップマップレベル
        m_arraySize,//テクスチャ配列のサイズ
        // カラーバッファーのフォーマットを32bit浮動小数点にしている
        DXGI_FORMAT_R32G32B32A32_FLOAT,
        DXGI_FORMAT_D32_FLOAT
    );

    m_luminnceRenderTarget.Create(
        m_wide,       // 解像度はメインレンダリングターゲットと同じ
        m_high,       // 解像度はメインレンダリングターゲットと同じ
        m_mipLv,//ミップマップレベル
        m_arraySize,//テクスチャ配列のサイズ
        // カラーバッファーのフォーマットを32bit浮動小数点にしている
        DXGI_FORMAT_R32G32B32A32_FLOAT,
        DXGI_FORMAT_D32_FLOAT
    );
}
void Bloom::InitSprites() {
   // 輝度抽出用のスプライトを初期化
   // 初期化情報を作成する
   // 輝度抽出用のシェーダーのファイルパスを指定する
    m_luminanceSpriteInitData.m_fxFilePath = "Assets/shader/samplePostEffect.fx";

    // 頂点シェーダーのエントリーポイントを指定する
    m_luminanceSpriteInitData.m_vsEntryPointFunc = "VSMain";

    // ピクセルシェーダーのエントリーポイントを指定する
    m_luminanceSpriteInitData.m_psEntryPoinFunc = "PSSamplingLuminance";

    // スプライトの幅と高さはluminnceRenderTargetと同じ
    m_luminanceSpriteInitData.m_width = 1280;
    m_luminanceSpriteInitData.m_height = 720;

    // テクスチャはメインレンダリングターゲットのカラーバッファー
    m_luminanceSpriteInitData.m_textures[0] = &m_mainRenderTarget.GetRenderTargetTexture();

    // 描き込むレンダリングターゲットのフォーマットを指定する
    m_luminanceSpriteInitData.m_colorBufferFormat[0] = DXGI_FORMAT_R32G32B32A32_FLOAT;

    // 作成した初期化情報をもとにスプライトを初期化する
    m_luminanceSprite.Init(m_luminanceSpriteInitData);

    //ガウシアンブラーを初期化
    m_gaussianBlur[0].Init(&m_luminnceRenderTarget.GetRenderTargetTexture());
    m_gaussianBlur[1].Init(&m_gaussianBlur[0].GetBokeTexture());
    m_gaussianBlur[2].Init(&m_gaussianBlur[1].GetBokeTexture());
    m_gaussianBlur[3].Init(&m_gaussianBlur[2].GetBokeTexture());

    //ボケ画像を合成して書き込むためのスプライトを初期化

    m_finalSpriteInitData.m_textures[0] = &m_gaussianBlur[0].GetBokeTexture();
    m_finalSpriteInitData.m_textures[1] = &m_gaussianBlur[1].GetBokeTexture();
    m_finalSpriteInitData.m_textures[2] = &m_gaussianBlur[2].GetBokeTexture();
    m_finalSpriteInitData.m_textures[3] = &m_gaussianBlur[3].GetBokeTexture();

    m_finalSpriteInitData.m_width = 1280;
    m_finalSpriteInitData.m_height = 720;

    m_finalSpriteInitData.m_fxFilePath = "Assets/shader/samplePostEffect.fx";
    m_finalSpriteInitData.m_psEntryPoinFunc = "PSBloomFinal";
    m_finalSpriteInitData.m_alphaBlendMode = AlphaBlendMode_Add;
    m_finalSpriteInitData.m_colorBufferFormat[0] = DXGI_FORMAT_R32G32B32A32_FLOAT;

    m_finalSprite.Init(m_finalSpriteInitData);
    // mainRenderTargetのテクスチャをフレームバッファーに貼り付けるためのスプライトを初期化する
    // スプライトの初期化オブジェクトを作成する

    // テクスチャはmainRenderTargetのカラーバッファー
    m_spriteInitData.m_textures[0] = &m_mainRenderTarget.GetRenderTargetTexture();
    m_spriteInitData.m_width = 1280;
    m_spriteInitData.m_height = 720;

    // モノクロ用のシェーダーを指定する
    m_spriteInitData.m_fxFilePath = "Assets/shader/sample2D.fx";

    // 初期化オブジェクトを使って、スプライトを初期化する
    m_copyToFrameBufferSprite.Init(m_spriteInitData);
}
void Bloom::Init() {
    
    //レンダリングターゲットを初期化。
    InitRenderTargets();
    //スプライトを初期化。
    InitSprites();
}

void Bloom::PreparationRT(RenderContext& renderContext) {	// step-1 レンダリングターゲットをmainRenderTargetに設定する
    renderContext.WaitUntilToPossibleSetRenderTarget(m_mainRenderTarget);
    // レンダリングターゲットを設定
    renderContext.SetRenderTargetAndViewport(m_mainRenderTarget);
    // レンダリングターゲットをクリア
    renderContext.ClearRenderTargetView(m_mainRenderTarget);
}

void Bloom::PreparationDraw(RenderContext& renderContext) {
    // レンダリングターゲットへの書き込み終了待ち
    renderContext.WaitUntilFinishDrawingToRenderTarget(m_mainRenderTarget);

    // 輝度抽出
    // 輝度抽出用のレンダリングターゲットに変更
    renderContext.WaitUntilToPossibleSetRenderTarget(m_luminnceRenderTarget);

    // レンダリングターゲットを設定
    renderContext.SetRenderTargetAndViewport(m_luminnceRenderTarget);

    // レンダリングターゲットをクリア
    renderContext.ClearRenderTargetView(m_luminnceRenderTarget);

    // 輝度抽出を行う
    m_luminanceSprite.Draw(renderContext);

    // レンダリングターゲットへの書き込み終了待ち
    renderContext.WaitUntilFinishDrawingToRenderTarget(m_luminnceRenderTarget);

    // ガウシアンブラーを指定回数実行する
    for (int blurNumberOfTimes = 0;
        blurNumberOfTimes < m_blurNumberOfTimes;
        blurNumberOfTimes++) {
        m_gaussianBlur[blurNumberOfTimes].Render(renderContext, 10);
    }
    // ボケ画像を合成してメインレンダリングターゲットに加算合成
    renderContext.WaitUntilToPossibleSetRenderTarget(m_mainRenderTarget);
    renderContext.SetRenderTargetAndViewport(m_mainRenderTarget);
    m_finalSprite.Draw(renderContext);
    renderContext.WaitUntilFinishDrawingToRenderTarget(m_mainRenderTarget);
    
}

void Bloom::FinalDraw(RenderContext& renderContext) {
    m_copyToFrameBufferSprite.Draw(renderContext);
}
