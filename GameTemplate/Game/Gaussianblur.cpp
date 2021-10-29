#include "stdafx.h"
#include "Gaussianblur.h"

void Gaussianblur::Init(Texture* originalTexture)
{
	m_originalTexture = originalTexture;

	//�����_�����O�^�[�Q�b�g���������B
	InitRenderTargets();
	//�X�v���C�g���������B
	InitSprites();
}
void Gaussianblur::Render(RenderContext& rc, float blurPower)
{
	//�d�݃e�[�u�����X�V����B
	UpdateWeightsTable(blurPower);

	//���u���[�����s�B
	//�����_�����O�^�[�Q�b�g�Ƃ��ė��p�ł���悤�ɂȂ�܂�wait������B
	rc.WaitUntilToPossibleSetRenderTarget(m_xBlurRenderTarget);
	//�����_�����O�^�[�Q�b�g��ݒ�B
	rc.SetRenderTargetAndViewport(m_xBlurRenderTarget);
	//�����_�����O�^�[�Q�b�g���N���A�B
	rc.ClearRenderTargetView(m_xBlurRenderTarget);
	//�h���[�B
	m_xBlurSprite.Draw(rc);
	//�����_�����O�^�[�Q�b�g�ւ̏������ݏI���҂��B
	rc.WaitUntilFinishDrawingToRenderTarget(m_xBlurRenderTarget);

	//�c�u���[�����s�B
	//�����_�����O�^�[�Q�b�g�Ƃ��ė��p�ł���悤�ɂȂ�܂�wait������B
	rc.WaitUntilToPossibleSetRenderTarget(m_yBlurRenderTarget);
	//�����_�����O�^�[�Q�b�g��ݒ�B
	rc.SetRenderTargetAndViewport(m_yBlurRenderTarget);
	//�����_�����O�^�[�Q�b�g���N���A�B
	rc.ClearRenderTargetView(m_yBlurRenderTarget);
	//�h���[�B
	m_yBlurSprite.Draw(rc);
	//�����_�����O�^�[�Q�b�g�ւ̏������ݏI���҂��B
	rc.WaitUntilFinishDrawingToRenderTarget(m_yBlurRenderTarget);
}

void Gaussianblur::InitRenderTargets()
{
	//���u���[
	m_xBlurRenderTarget.Create(
		m_originalTexture->GetWidth() / 2,//�����_�����O�^�[�Q�b�g�̕�
		m_originalTexture->GetHeight(),//�����_�����O�^�[�Q�b�g�̍���
		m_mipLv,//�~�b�v�}�b�v���x��
		m_arraySize,//�e�N�X�`���z��̃T�C�Y
		m_originalTexture->GetFormat(),
		DXGI_FORMAT_D32_FLOAT
	);
 //�c�u���[
	m_yBlurRenderTarget.Create(
		m_originalTexture->GetWidth() / 2,//�����_�����O�^�[�Q�b�g�̕�
		m_originalTexture->GetHeight() / 2,//�����_�����O�^�[�Q�b�g�̍���
		m_mipLv,//�~�b�v�}�b�v���x��
		m_arraySize,//�e�N�X�`���z��̃T�C�Y
		m_originalTexture->GetFormat(),
		DXGI_FORMAT_D32_FLOAT
	);

}

void Gaussianblur::InitSprites()
{//��
	m_xBlurSpriteInitData.m_fxFilePath = "Assets/shader/preset/gaussianBlur.fx";
	m_xBlurSpriteInitData.m_vsEntryPointFunc = "VSXBlur";
	m_xBlurSpriteInitData.m_psEntryPoinFunc = "PSBlur";

	//�X�v���C�g�̉𑜓x��xBlurRenderTarget�Ɠ����B
	m_xBlurSpriteInitData.m_width = m_xBlurRenderTarget.GetWidth();
	m_xBlurSpriteInitData.m_height = m_xBlurRenderTarget.GetHeight();
	//�e�N�X�`���͋P�x�e�N�X�`���B
	m_xBlurSpriteInitData.m_textures[0] = m_originalTexture;

	//�y���ځz���[�U�[�g���̒萔�o�b�t�@�ɏd�݃e�[�u����ݒ肷��B
	m_xBlurSpriteInitData.m_expandConstantBuffer = &m_weights;
	m_xBlurSpriteInitData.m_expandConstantBufferSize = sizeof(m_weights);

	m_xBlurSprite.Init(m_xBlurSpriteInitData);

//�c
	m_yBlurSpriteInitData.m_fxFilePath = "Assets/shader/preset/gaussianBlur.fx";
	m_yBlurSpriteInitData.m_vsEntryPointFunc = "VSYBlur";
	m_yBlurSpriteInitData.m_psEntryPoinFunc = "PSBlur";
	//�X�v���C�g�̕��ƍ�����yBlurRenderTarget�Ɠ����B
	m_yBlurSpriteInitData.m_width = m_yBlurRenderTarget.GetWidth();
	m_yBlurSpriteInitData.m_height = m_yBlurRenderTarget.GetHeight();
	//�e�N�X�`���͉��u���[�����������́B
	m_yBlurSpriteInitData.m_textures[0] = &m_xBlurRenderTarget.GetRenderTargetTexture();

	//���[�U�[�g���̒萔�o�b�t�@�ɏd�݃e�[�u����ݒ肷��B
	m_yBlurSpriteInitData.m_expandConstantBuffer = &m_weights;
	m_yBlurSpriteInitData.m_expandConstantBufferSize = sizeof(m_weights);

	m_yBlurSprite.Init(m_yBlurSpriteInitData);
}

void Gaussianblur::UpdateWeightsTable(float blurPower)
{
	float total = 0;
	for (int distanceFromTexel = 0;
		distanceFromTexel < NUM_WEIGHTS;
		distanceFromTexel++) {
		m_weights[distanceFromTexel] = expf(-0.5f * (float)(distanceFromTexel * distanceFromTexel) / blurPower);
		total += 2.0f * m_weights[distanceFromTexel];

	}
	// �K�i��
	for (int i = 0; i < NUM_WEIGHTS; i++) {
		m_weights[i] /= total;
	}
}
