#pragma once
#include "Gaussianblur.h"





class Bloom : public IGameObject
{
public:
	Bloom();
	~Bloom();
	bool Start();
	void Update();
	void Render(RenderContext& renderContext) override final;
private:
	RenderTarget mainRenderTarget;
	RenderTarget luminnceRenderTarget;
	SpriteInitData finalSpriteInitData;
	SpriteInitData spriteInitData;
	Sprite luminanceSprite;
	Gaussianblur gaussianBlur[4];
	Sprite copyToFrameBufferSprite;
	Sprite finalSprite;
	Texture* m_originalTexture = nullptr;	//オリジナルテクスチャ。
	SpriteInitData luminanceSpriteInitData;
	
public:
	void Init();
	void InitBloom();
	void InitRenderTargets();
	void InitSprites();

};

