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
        m_wide,//�����_�����O�^�[�Q�b�g�̕�
        m_high,//�����_�����O�^�[�Q�b�g�̍���
        m_mipLv,//�~�b�v�}�b�v���x��
        m_arraySize,//�e�N�X�`���z��̃T�C�Y
        // �J���[�o�b�t�@�[�̃t�H�[�}�b�g��32bit���������_�ɂ��Ă���
        DXGI_FORMAT_R32G32B32A32_FLOAT,
        DXGI_FORMAT_D32_FLOAT
    );

    m_luminnceRenderTarget.Create(
        m_wide,       // �𑜓x�̓��C�������_�����O�^�[�Q�b�g�Ɠ���
        m_high,       // �𑜓x�̓��C�������_�����O�^�[�Q�b�g�Ɠ���
        m_mipLv,//�~�b�v�}�b�v���x��
        m_arraySize,//�e�N�X�`���z��̃T�C�Y
        // �J���[�o�b�t�@�[�̃t�H�[�}�b�g��32bit���������_�ɂ��Ă���
        DXGI_FORMAT_R32G32B32A32_FLOAT,
        DXGI_FORMAT_D32_FLOAT
    );
}
void Bloom::InitSprites() {
   // �P�x���o�p�̃X�v���C�g��������
   // �����������쐬����
   // �P�x���o�p�̃V�F�[�_�[�̃t�@�C���p�X���w�肷��
    m_luminanceSpriteInitData.m_fxFilePath = "Assets/shader/samplePostEffect.fx";

    // ���_�V�F�[�_�[�̃G���g���[�|�C���g���w�肷��
    m_luminanceSpriteInitData.m_vsEntryPointFunc = "VSMain";

    // �s�N�Z���V�F�[�_�[�̃G���g���[�|�C���g���w�肷��
    m_luminanceSpriteInitData.m_psEntryPoinFunc = "PSSamplingLuminance";

    // �X�v���C�g�̕��ƍ�����luminnceRenderTarget�Ɠ���
    m_luminanceSpriteInitData.m_width = 1280;
    m_luminanceSpriteInitData.m_height = 720;

    // �e�N�X�`���̓��C�������_�����O�^�[�Q�b�g�̃J���[�o�b�t�@�[
    m_luminanceSpriteInitData.m_textures[0] = &m_mainRenderTarget.GetRenderTargetTexture();

    // �`�����ރ����_�����O�^�[�Q�b�g�̃t�H�[�}�b�g���w�肷��
    m_luminanceSpriteInitData.m_colorBufferFormat[0] = DXGI_FORMAT_R32G32B32A32_FLOAT;

    // �쐬�����������������ƂɃX�v���C�g������������
    m_luminanceSprite.Init(m_luminanceSpriteInitData);

    //�K�E�V�A���u���[��������
    m_gaussianBlur[0].Init(&m_luminnceRenderTarget.GetRenderTargetTexture());
    m_gaussianBlur[1].Init(&m_gaussianBlur[0].GetBokeTexture());
    m_gaussianBlur[2].Init(&m_gaussianBlur[1].GetBokeTexture());
    m_gaussianBlur[3].Init(&m_gaussianBlur[2].GetBokeTexture());

    //�{�P�摜���������ď������ނ��߂̃X�v���C�g��������

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
    // mainRenderTarget�̃e�N�X�`�����t���[���o�b�t�@�[�ɓ\��t���邽�߂̃X�v���C�g������������
    // �X�v���C�g�̏������I�u�W�F�N�g���쐬����

    // �e�N�X�`����mainRenderTarget�̃J���[�o�b�t�@�[
    m_spriteInitData.m_textures[0] = &m_mainRenderTarget.GetRenderTargetTexture();
    m_spriteInitData.m_width = 1280;
    m_spriteInitData.m_height = 720;

    // ���m�N���p�̃V�F�[�_�[���w�肷��
    m_spriteInitData.m_fxFilePath = "Assets/shader/sample2D.fx";

    // �������I�u�W�F�N�g���g���āA�X�v���C�g������������
    m_copyToFrameBufferSprite.Init(m_spriteInitData);
}
void Bloom::Init() {
    
    //�����_�����O�^�[�Q�b�g���������B
    InitRenderTargets();
    //�X�v���C�g���������B
    InitSprites();
}

void Bloom::PreparationRT(RenderContext& renderContext) {	// step-1 �����_�����O�^�[�Q�b�g��mainRenderTarget�ɐݒ肷��
    renderContext.WaitUntilToPossibleSetRenderTarget(m_mainRenderTarget);
    // �����_�����O�^�[�Q�b�g��ݒ�
    renderContext.SetRenderTargetAndViewport(m_mainRenderTarget);
    // �����_�����O�^�[�Q�b�g���N���A
    renderContext.ClearRenderTargetView(m_mainRenderTarget);
}

void Bloom::PreparationDraw(RenderContext& renderContext) {
    // �����_�����O�^�[�Q�b�g�ւ̏������ݏI���҂�
    renderContext.WaitUntilFinishDrawingToRenderTarget(m_mainRenderTarget);

    // �P�x���o
    // �P�x���o�p�̃����_�����O�^�[�Q�b�g�ɕύX
    renderContext.WaitUntilToPossibleSetRenderTarget(m_luminnceRenderTarget);

    // �����_�����O�^�[�Q�b�g��ݒ�
    renderContext.SetRenderTargetAndViewport(m_luminnceRenderTarget);

    // �����_�����O�^�[�Q�b�g���N���A
    renderContext.ClearRenderTargetView(m_luminnceRenderTarget);

    // �P�x���o���s��
    m_luminanceSprite.Draw(renderContext);

    // �����_�����O�^�[�Q�b�g�ւ̏������ݏI���҂�
    renderContext.WaitUntilFinishDrawingToRenderTarget(m_luminnceRenderTarget);

    // �K�E�V�A���u���[���w��񐔎��s����
    for (int blurNumberOfTimes = 0;
        blurNumberOfTimes < m_blurNumberOfTimes;
        blurNumberOfTimes++) {
        m_gaussianBlur[blurNumberOfTimes].Render(renderContext, 10);
    }
    // �{�P�摜���������ă��C�������_�����O�^�[�Q�b�g�ɉ��Z����
    renderContext.WaitUntilToPossibleSetRenderTarget(m_mainRenderTarget);
    renderContext.SetRenderTargetAndViewport(m_mainRenderTarget);
    m_finalSprite.Draw(renderContext);
    renderContext.WaitUntilFinishDrawingToRenderTarget(m_mainRenderTarget);
    
}

void Bloom::FinalDraw(RenderContext& renderContext) {
    m_copyToFrameBufferSprite.Draw(renderContext);
}
