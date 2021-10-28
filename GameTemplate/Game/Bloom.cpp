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
void Bloom::InitSprites() {
    // 輝度抽出用のスプライトを初期化
   // 初期化情報を作成する


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

    // 作成した初期化情報をもとにスプライトを初期化する
    luminanceSprite.Init(luminanceSpriteInitData);

    //ガウシアンブラーを初期化
    gaussianBlur[0].Init(&luminnceRenderTarget.GetRenderTargetTexture());
    gaussianBlur[1].Init(&gaussianBlur[0].GetBokeTexture());
    gaussianBlur[2].Init(&gaussianBlur[1].GetBokeTexture());
    gaussianBlur[3].Init(&gaussianBlur[2].GetBokeTexture());

    //ボケ画像を合成して書き込むためのスプライトを初期化

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

    // 初期化オブジェクトを使って、スプライトを初期化する
    copyToFrameBufferSprite.Init(spriteInitData);
}
void Bloom::Init() {
  

   
    //レンダリングターゲットを初期化。
    InitRenderTargets();
    //スプライトを初期化。
    InitSprites();
}
void Bloom::InitBloom() {

}
void Bloom::Render(RenderContext& renderContext) {
    // レンダリングターゲットをmainRenderTargetに変更する
       // レンダリングターゲットとして利用できるまで待つ
    renderContext.WaitUntilToPossibleSetRenderTarget(mainRenderTarget);
    // レンダリングターゲットを設定
    renderContext.SetRenderTargetAndViewport(mainRenderTarget);
    // レンダリングターゲットをクリア
    renderContext.ClearRenderTargetView(mainRenderTarget);

    // レンダリングターゲットへの書き込み終了待ち
    renderContext.WaitUntilFinishDrawingToRenderTarget(mainRenderTarget);

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
    // メインレンダリングターゲットの絵をフレームバッファーにコピー
    renderContext.SetRenderTarget(
        g_graphicsEngine->GetCurrentFrameBuffuerRTV(),
        g_graphicsEngine->GetCurrentFrameBuffuerDSV()
    );
    copyToFrameBufferSprite.Draw(renderContext);
   
}