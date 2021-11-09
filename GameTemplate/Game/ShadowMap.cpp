#include "stdafx.h"
#include "ShadowMap.h"
void ShadowMap::Init(const char* filePath, const char* BGfilePath) {
	m_tkmFilePath = filePath;
	m_bgTkmFilePath = BGfilePath;
	InitShadow(filePath, BGfilePath);
}
void ShadowMap::InitShadow(const char* filePath, const char* BGfilePath) {
	// �J�����̈ʒu��ݒ�B����̓��C�g�̈ʒu
	m_lightCamera.SetPosition(m_cameraSetPos);

	// �J�����̒����_��ݒ�B���ꂪ���C�g���Ƃ炵�Ă���ꏊ
	m_lightCamera.SetTarget(m_cameraSetTarget);

	// �������ݒ�B����̓��C�g���^���������Ă���̂ŁAX��������ɂ��Ă���
	m_lightCamera.SetUp(m_cameraSetUp);

	// ���C�g�r���[�v���W�F�N�V�����s����v�Z���Ă���
	m_lightCamera.Update();

	//�����_�����O�^�[�Q�b�g������
	m_shadowMap.Create(
		m_wide,//�����_�����O�^�[�Q�b�g�̕�
		m_high,//�����_�����O�^�[�Q�b�g�̍���
		m_mipLv,//�~�b�v�}�b�v���x��
		m_arraySize,//�e�N�X�`���z��̃T�C�Y
		//�V���h�E�}�b�v�̃J���[�o�b�t�@�̃t�H�[�}�b�g��ύX���Ă���B
		DXGI_FORMAT_R32_FLOAT,
		DXGI_FORMAT_D32_FLOAT,
		m_clearColor
	);
	//�V���h�E�}�b�v�`��p�̃V�F�[�_�[���w�肷��B
	m_shadowModelInitData.m_fxFilePath = "Assets/shader/sampleDrawShadowMap.fx";
	m_shadowModelInitData.m_tkmFilePath = filePath;

	//�J���[�o�b�t�@�̃t�H�[�}�b�g�ɕύX���������̂ŁA��������ύX����B
	m_shadowModelInitData.m_colorBufferFormat[0] = DXGI_FORMAT_R32_FLOAT;


	m_shadowModel.Init(m_shadowModelInitData);

	// �V���h�E���V�[�o�[�i�e�����Ƃ���郂�f���j�p�̃V�F�[�_�[���w�肷��
	m_bgModelInitData.m_fxFilePath = "Assets/shader/sampleShadowReciever.fx";

	// �V���h�E�}�b�v���g��SRV�ɐݒ肷��
	m_bgModelInitData.m_expandShaderResoruceView = &m_shadowMap.GetRenderTargetTexture();

	// ���C�g�r���[�v���W�F�N�V�����s����g���萔�o�b�t�@�[�ɐݒ肷��
	m_bgModelInitData.m_expandConstantBuffer = (void*)&m_lightCamera.GetViewProjectionMatrix();
	m_bgModelInitData.m_expandConstantBufferSize = sizeof(m_lightCamera.GetViewProjectionMatrix());
	m_bgModelInitData.m_tkmFilePath = BGfilePath;

	m_bgModel.Init(m_bgModelInitData);
}

void ShadowMap::Draw(RenderContext& renderContext) {
	
	//m_shadowPos.z -= g_pad[0]->GetLStickYF();
	m_shadowPos.x -= g_pad[0]->GetLStickXF();
	m_scale.x -= g_pad[0]->GetLStickYF()*0.001f;
	m_scale.y -= g_pad[0]->GetLStickYF() * 0.001f;
	m_scale.z -= g_pad[0]->GetLStickYF() * 0.001f;
	m_rotation.SetRotationY(-70.0f);
	m_shadowModel.UpdateWorldMatrix(
		m_shadowPos,
		m_rotation,
		m_scale
	);

	// �V���h�E�}�b�v�Ƀ����_�����O
	   // �����_�����O�^�[�Q�b�g���V���h�E�}�b�v�ɕύX����
	renderContext.WaitUntilToPossibleSetRenderTarget(m_shadowMap);
	renderContext.SetRenderTargetAndViewport(m_shadowMap);
	renderContext.ClearRenderTargetView(m_shadowMap);

	// �e���f����`��
	m_shadowModel.Draw(renderContext, m_lightCamera);

	// �������݊����҂�
	renderContext.WaitUntilFinishDrawingToRenderTarget(m_shadowMap);
	
}
void ShadowMap::BGDraw(RenderContext& renderContext) {
	m_bgModel.Draw(renderContext);
}