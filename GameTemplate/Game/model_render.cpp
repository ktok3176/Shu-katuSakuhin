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
	//Gaussianblur* gaussianblur = nullptr;
	//gaussianblur = NewGO<Gaussianblur>(0);
	
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

	/*g_camera3D->SetPosition({ 0.0f, -50.0f, 150.0f });
	g_camera3D->SetTarget({ 0.0f, 0.0f, 0.0f });*/

	//���f���̏��������邽�߂̏���ݒ�
	ModelInitData initData;
	//tkm�t�@�C���̃t�@�C���p�X��ݒ�
	initData.m_tkmFilePath = filePath;

	//�g�p����V�F�[�_�[�t�@�C���p�X��ݒ�
	initData.m_fxFilePath = "Assets/shader/model.fx";
	//���f���̏�������w��
	initData.m_modelUpAxis = modelUpAxis;
	initData.m_expandConstantBuffer = &light;
	initData.m_expandConstantBufferSize = sizeof(light);
	//�����������g���ă��f���\������������������
	initData.m_colorBufferFormat[0] = DXGI_FORMAT_R32G32B32A32_FLOAT;
	model.Init(initData);
}

//////////////////////////////
// ���C�g���Ƃ̏�����
//////////////////////////////

void ModelRender::InitDirectionLight()
{
	
	//���C�g�̃J���[�̐ݒ�i���C�g�̋����j
	light.dirColor.x = 0.8f;
	light.dirColor.y = 0.8f;
	light.dirColor.z = 0.8f;
	//���C�g�͎΂ߏォ�瓖�����Ă���B
	
	light.dirDirection.x = 0.0f;
	light.dirDirection.y = 0.0f;
	light.dirDirection.z = -1.0f;
	//���K������B
	light.dirDirection.Normalize();
	

	//���_�̈ʒu��ݒ�
	light.eyePos = g_camera3D->GetPosition();
}
void ModelRender::InitPointLight()
{
	//�|�C���g���C�g�̏������W��ݒ肷��
	light.ptPosition.x = 0.0f;
	light.ptPosition.y = 50.0f;
	light.ptPosition.z = 50.0f;

	//�|�C���g���C�g�̏����J���[��ݒ肷��
	light.ptColor.x = 0.0f;
	light.ptColor.y = 0.0f;
	light.ptColor.z = 0.0f;

	//�|�C���g���C�g�̉e���͈͂�ݒ肷��
	light.ptRange = 100.0f;
}
void ModelRender::InitSpotLight()
{//�X�|�b�g���C�g
	light.spPosition.x = 0.0f;
	light.spPosition.z = 0.0f;
	light.spPosition.y = 50.0f;

	light.spColor.x = 0.0f;
	light.spColor.y = 0.0f;
	light.spColor.z = 0.0f;

	light.spDirection.x = 1.0f;
	light.spDirection.y = -1.0f;
	light.spDirection.z = 1.0f;

	light.spDirection.Normalize();

	light.spRange = 300.0f;

	light.spAngle = Math::DegToRad(25.0f);
}

void ModelRender::InitAmbientLight()
{
	//����
	light.ambientLight.x = 0.3f;
	light.ambientLight.y = 0.3f;
	light.ambientLight.z = 0.3f;
}

////////////////////////////////////////////////////////////
// �`�揈��
////////////////////////////////////////////////////////////

void ModelRender::Render(RenderContext& renderContext)
{
	// mainRenderTarget�Ɋe�탂�f����`�悷��
	model.Draw(renderContext);


	
	light.dirColor.x += g_pad[0]->GetLStickXF() * 0.5f;
	light.dirColor.y += g_pad[0]->GetLStickXF() * 0.5f;
	light.dirColor.z += g_pad[0]->GetLStickXF() * 0.5f;
	
}

void ModelRender::Update()
{//����������
	if (m_finishInit == false) {
		return;
	}
	//���f���̍��W�X�V
	
	model.UpdateWorldMatrix(m_position, m_rotation, m_scale);

}


