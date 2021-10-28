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
        // �y���ځz�J���[�o�b�t�@�[�̃t�H�[�}�b�g��32bit���������_�ɂ��Ă���
        DXGI_FORMAT_R32G32B32A32_FLOAT,
        DXGI_FORMAT_D32_FLOAT
    );

    luminnceRenderTarget.Create(
        1280,       // �𑜓x�̓��C�������_�����O�^�[�Q�b�g�Ɠ���
        720,        // �𑜓x�̓��C�������_�����O�^�[�Q�b�g�Ɠ���
        1,
        1,
        // �y���ځz�J���[�o�b�t�@�[�̃t�H�[�}�b�g��32bit���������_�ɂ��Ă���
        DXGI_FORMAT_R32G32B32A32_FLOAT,
        DXGI_FORMAT_D32_FLOAT
    );
}
void Bloom::InitSprites() {
    // �P�x���o�p�̃X�v���C�g��������
   // �����������쐬����


   // �P�x���o�p�̃V�F�[�_�[�̃t�@�C���p�X���w�肷��
    luminanceSpriteInitData.m_fxFilePath = "Assets/shader/samplePostEffect.fx";

    // ���_�V�F�[�_�[�̃G���g���[�|�C���g���w�肷��
    luminanceSpriteInitData.m_vsEntryPointFunc = "VSMain";

    // �s�N�Z���V�F�[�_�[�̃G���g���[�|�C���g���w�肷��
    luminanceSpriteInitData.m_psEntryPoinFunc = "PSSamplingLuminance";

    // �X�v���C�g�̕��ƍ�����luminnceRenderTarget�Ɠ���
    luminanceSpriteInitData.m_width = 1280;
    luminanceSpriteInitData.m_height = 720;

    // �e�N�X�`���̓��C�������_�����O�^�[�Q�b�g�̃J���[�o�b�t�@�[
    luminanceSpriteInitData.m_textures[0] = &mainRenderTarget.GetRenderTargetTexture();

    // �`�����ރ����_�����O�^�[�Q�b�g�̃t�H�[�}�b�g���w�肷��
    luminanceSpriteInitData.m_colorBufferFormat[0] = DXGI_FORMAT_R32G32B32A32_FLOAT;

    // �쐬�����������������ƂɃX�v���C�g������������
    luminanceSprite.Init(luminanceSpriteInitData);

    //�K�E�V�A���u���[��������
    gaussianBlur[0].Init(&luminnceRenderTarget.GetRenderTargetTexture());
    gaussianBlur[1].Init(&gaussianBlur[0].GetBokeTexture());
    gaussianBlur[2].Init(&gaussianBlur[1].GetBokeTexture());
    gaussianBlur[3].Init(&gaussianBlur[2].GetBokeTexture());

    //�{�P�摜���������ď������ނ��߂̃X�v���C�g��������

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
    // mainRenderTarget�̃e�N�X�`�����t���[���o�b�t�@�[�ɓ\��t���邽�߂̃X�v���C�g������������
    // �X�v���C�g�̏������I�u�W�F�N�g���쐬����

    // �e�N�X�`����mainRenderTarget�̃J���[�o�b�t�@�[
    spriteInitData.m_textures[0] = &mainRenderTarget.GetRenderTargetTexture();
    spriteInitData.m_width = 1280;
    spriteInitData.m_height = 720;

    // ���m�N���p�̃V�F�[�_�[���w�肷��
    spriteInitData.m_fxFilePath = "Assets/shader/sample2D.fx";

    // �������I�u�W�F�N�g���g���āA�X�v���C�g������������
    copyToFrameBufferSprite.Init(spriteInitData);
}
void Bloom::Init() {
  

   
    //�����_�����O�^�[�Q�b�g���������B
    InitRenderTargets();
    //�X�v���C�g���������B
    InitSprites();
}
void Bloom::InitBloom() {

}
void Bloom::Render(RenderContext& renderContext) {
    // �����_�����O�^�[�Q�b�g��mainRenderTarget�ɕύX����
       // �����_�����O�^�[�Q�b�g�Ƃ��ė��p�ł���܂ő҂�
    renderContext.WaitUntilToPossibleSetRenderTarget(mainRenderTarget);
    // �����_�����O�^�[�Q�b�g��ݒ�
    renderContext.SetRenderTargetAndViewport(mainRenderTarget);
    // �����_�����O�^�[�Q�b�g���N���A
    renderContext.ClearRenderTargetView(mainRenderTarget);

    // �����_�����O�^�[�Q�b�g�ւ̏������ݏI���҂�
    renderContext.WaitUntilFinishDrawingToRenderTarget(mainRenderTarget);

    // �P�x���o
    // �P�x���o�p�̃����_�����O�^�[�Q�b�g�ɕύX
    renderContext.WaitUntilToPossibleSetRenderTarget(luminnceRenderTarget);

    // �����_�����O�^�[�Q�b�g��ݒ�
    renderContext.SetRenderTargetAndViewport(luminnceRenderTarget);

    // �����_�����O�^�[�Q�b�g���N���A
    renderContext.ClearRenderTargetView(luminnceRenderTarget);

    // �P�x���o���s��
    luminanceSprite.Draw(renderContext);

    // �����_�����O�^�[�Q�b�g�ւ̏������ݏI���҂�
    renderContext.WaitUntilFinishDrawingToRenderTarget(luminnceRenderTarget);

    // step-3 �K�E�V�A���u���[��4����s����
    gaussianBlur[0].Render(renderContext, 10);
    gaussianBlur[1].Render(renderContext, 10);
    gaussianBlur[2].Render(renderContext, 10);
    gaussianBlur[3].Render(renderContext, 10);
    // step-4 �{�P�摜���������ă��C�������_�����O�^�[�Q�b�g�ɉ��Z����
    renderContext.WaitUntilToPossibleSetRenderTarget(mainRenderTarget);
    renderContext.SetRenderTargetAndViewport(mainRenderTarget);
    finalSprite.Draw(renderContext);
    renderContext.WaitUntilFinishDrawingToRenderTarget(mainRenderTarget);
    // ���C�������_�����O�^�[�Q�b�g�̊G���t���[���o�b�t�@�[�ɃR�s�[
    renderContext.SetRenderTarget(
        g_graphicsEngine->GetCurrentFrameBuffuerRTV(),
        g_graphicsEngine->GetCurrentFrameBuffuerDSV()
    );
    copyToFrameBufferSprite.Draw(renderContext);
   
}