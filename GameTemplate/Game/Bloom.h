#pragma once
#include "Gaussianblur.h"
const int m_blurNumberOfTimes = 4;
class Bloom : public IGameObject
{
public:
	Bloom();
	~Bloom();
	bool Start();
	void Update();
	void PreparationRT(RenderContext& renderContext);
	void PreparationDraw(RenderContext& renderContext);
	void FinalDraw(RenderContext& renderContext);
private:
	int m_wide = 1280;
	int m_high = 720;
	int m_mipLv = 1;
	int m_arraySize = 1;

	Gaussianblur m_gaussianBlur[m_blurNumberOfTimes];
	RenderTarget m_mainRenderTarget;
	RenderTarget m_luminnceRenderTarget;
	Sprite m_luminanceSprite;
	Sprite m_finalSprite;
	Sprite m_copyToFrameBufferSprite;
	SpriteInitData m_spriteInitData;
	SpriteInitData m_luminanceSpriteInitData;
	SpriteInitData m_finalSpriteInitData;
	Texture* m_originalTexture = nullptr;	//オリジナルテクスチャ。

public:
	void Init();
	void InitRenderTargets();
	void InitSprites();

};

