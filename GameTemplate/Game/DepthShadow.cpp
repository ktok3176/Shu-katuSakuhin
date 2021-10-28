#include "stdafx.h"
#include "DepthShadow.h"

void DepthShadow::Init(const char* filePath, const char* BGfilePath) {
	m_tkmFilePath = filePath;
	m_bgTkmFilePath = BGfilePath;
	InitShadow(filePath, BGfilePath);
}
void DepthShadow::InitShadow(const char* filePath, const char* BGfilePath) {
	// �J�����̈ʒu��ݒ�B����̓��C�g�̈ʒu
	lightCamera.SetPosition(cameraSetPos);

	// �J�����̒����_��ݒ�B���ꂪ���C�g���Ƃ炵�Ă���ꏊ
	lightCamera.SetTarget(cameraSetTarget);

	// �������ݒ�B����̓��C�g���^���������Ă���̂ŁAX��������ɂ��Ă���
	lightCamera.SetUp(cameraSetUp);

	// ���C�g�r���[�v���W�F�N�V�����s����v�Z���Ă���
	lightCamera.Update();

	InitRenderTargets();

	//�V���h�E�}�b�v�`��p�̃V�F�[�_�[���w�肷��B
	m_shadowModelInitData.m_fxFilePath = "Assets/shader/sampleDrawShadowMap.fx";
	m_shadowModelInitData.m_tkmFilePath = filePath;

	//�y���ځz�J���[�o�b�t�@�̃t�H�[�}�b�g�ɕύX���������̂ŁA��������ύX����B
	m_shadowModelInitData.m_colorBufferFormat[0] = DXGI_FORMAT_R32_FLOAT;


	m_shadowModel.Init(m_shadowModelInitData);
	m_shadowModel.UpdateWorldMatrix(
		m_shadow,
		g_quatIdentity,
		g_vec3One
	);
	// �e���󂯂�w�i���f����������
	// �V���h�E���V�[�o�[�i�e�����Ƃ���郂�f���j�p�̃V�F�[�_�[���w�肷��
	bgModelInitData.m_fxFilePath = "Assets/shader/sampleShadowReciever.fx";

	// �V���h�E�}�b�v���g��SRV�ɐݒ肷��
	bgModelInitData.m_expandShaderResoruceView = &shadowMap.GetRenderTargetTexture();

	// ���C�g�r���[�v���W�F�N�V�����s����g���萔�o�b�t�@�[�ɐݒ肷��
	bgModelInitData.m_expandConstantBuffer = (void*)&lightCamera.GetViewProjectionMatrix();
	bgModelInitData.m_expandConstantBufferSize = sizeof(lightCamera.GetViewProjectionMatrix());
	bgModelInitData.m_tkmFilePath = BGfilePath;

	bgModel.Init(bgModelInitData);
}

void DepthShadow::Render(RenderContext& renderContext) {
	bgModel.Draw(renderContext);
}
void DepthShadow::InitRenderTargets() {
	shadowMap.Create(
		1024,
		1024,
		1,
		1,
		DXGI_FORMAT_R32_FLOAT,
		DXGI_FORMAT_D32_FLOAT,
		clearColor
	);
}
