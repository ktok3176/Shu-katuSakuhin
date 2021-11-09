#include "stdafx.h"
#include "model_render.h"


ModelRender::ModelRender()
{

}

ModelRender::~ModelRender()
{

}

bool ModelRender::Start()
{		
	return true;
}

////////////////////////////////////////////////////////////
// ������
////////////////////////////////////////////////////////////



void ModelRender::Init(const char* filePath,
	modelUpAxis::EnModelUpAxis modelUpAxis
)
{	
	////tkm�t�@�C���̃t�@�C���p�X��ێ�
	m_tkmFilePath = filePath;
	////���f���̏�����
	InitModel(filePath, modelUpAxis);
	//����������
	m_finishInit = true;
}

	


void ModelRender::InitModel(const char* filePath, modelUpAxis::EnModelUpAxis modelUpAxis)
{

	//���C�g�̐ݒ������B
	//�f�B���N�V�������C�g������������
	InitDirectionLight();

	//�|�C���g���C�g������������
    InitPointLight();

	//�X�|�b�g���C�g������������
	InitSpotLight();
	
	//����������������
	InitAmbientLight();

	//���f���̏��������邽�߂̏���ݒ�
	ModelInitData initData;
	//tkm�t�@�C���̃t�@�C���p�X��ݒ�
	initData.m_tkmFilePath = filePath;

	//�g�p����V�F�[�_�[�t�@�C���p�X��ݒ�
	initData.m_fxFilePath = "Assets/shader/model.fx";
	//���f���̏�������w��
	initData.m_modelUpAxis = modelUpAxis;
	initData.m_expandConstantBuffer = &m_light;
	initData.m_expandConstantBufferSize = sizeof(m_light);
	//�����������g���ă��f���\������������������
	initData.m_colorBufferFormat[0] = DXGI_FORMAT_R32G32B32A32_FLOAT;
	m_model.Init(initData);
}

//////////////////////////////
// ���C�g���Ƃ̏�����
//////////////////////////////

void ModelRender::InitDirectionLight()
{
	//�f�B���N�V�������C�g�̏����J���[��ݒ肷��
	m_light.dirColor = m_dirPower;
	//�f�B���N�V�������C�g�̕�����ݒ肷��
	m_light.dirDirection = m_dirDirection;
	//���K������B
	m_light.dirDirection.Normalize();
	
	//���_�̈ʒu��ݒ�
	m_light.eyePos = g_camera3D->GetPosition();
}
void ModelRender::InitPointLight()
{
	//�|�C���g���C�g�̏����J���[��ݒ肷��
	m_light.ptColor = m_ptPower;

	//�|�C���g���C�g�̏������W��ݒ肷��
	m_light.ptPosition = m_ptPos;

	//�|�C���g���C�g�̉e���͈͂�ݒ肷��
	m_light.ptRange = m_ptRange;
}
void ModelRender::InitSpotLight()
{//�X�|�b�g���C�g�̏����J���[��ݒ肷��
	m_light.spColor = m_spPower;

	//�X�|�b�g���C�g�̏������W��ݒ肷��
	m_light.spPosition = m_spPos;

	//�X�|�b�g���C�g�̕�����ݒ肷��
	m_light.spDirection = m_spDirection;
	//���K������B
	m_light.spDirection.Normalize();

	m_light.spRange = m_spRange;

	m_light.spAngle = Math::DegToRad(m_spAngle);
}

void ModelRender::InitAmbientLight()
{
	//�����̋���
	m_light.ambientLight = ambientPower;
}

////////////////////////////////////////////////////////////
// �`�揈��
////////////////////////////////////////////////////////////

void ModelRender::Render(RenderContext& renderContext)
{
	// mainRenderTarget�Ɋe�탂�f����`�悷��
	m_model.Draw(renderContext);
}

void ModelRender::Update()
{//����������
	if (m_finishInit == false) {
		return;
	}
	//���f���̍��W�X�V
	m_model.UpdateWorldMatrix(m_position, m_rotation, m_scale);
}


