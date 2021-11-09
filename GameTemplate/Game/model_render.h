#pragma once
#include "Gaussianblur.h"

struct Light {
	//�f�B���N�V�������C�g
	Vector3 dirDirection = { 0.0f, 0.0f, 0.0f }; //���C�g�̕���
	//HLSL���̒萔�o�b�t�@��float3�^�̕ϐ���16�̔{���̃A�h���X�ɔz�u����邽�߁A
	//C++���ɂ̓p�f�B���O�𖄂߂Ă����B
	float pad = 0.0f;
	Vector3 dirColor = { 0.0f, 0.0f, 0.0f };
	float pad1 = 0.0f;
	//�|�C���g���C�g
	Vector3 ptPosition = { 0.0f, 0.0f, 0.0f };
	float pad2 = 0.0f;
	Vector3 ptColor = { 0.0f, 0.0f, 0.0f };
	float ptRange = 0.0f;
	//�X�|�b�g���C�g
	Vector3 spPosition = { 0.0f, 0.0f, 0.0f };
	float pad3 = 0.0f;
	Vector3 spColor = { 0.0f, 0.0f, 0.0f };
	float spRange = 0.0f;
	Vector3 spDirection = { 0.0f, 0.0f, 0.0f };
	float spAngle = 0.0f;
	//�\���̂Ɏ��_�̈ʒu��ǉ�����
	Vector3 eyePos = { 0.0f, 0.0f, 0.0f }; //���_�̈ʒu
	float pad4 = 0.0f;

	//����
	Vector3 ambientLight = { 0.0f, 0.0f, 0.0f };   // �A���r�G���g���C�g
};

class ModelRender : public IGameObject
{

public:
	ModelRender();
	~ModelRender();
	bool Start() override final;
	void Update() override final;
	void Render(RenderContext& renderContext) override final;

private: //data menber
	Model m_model;
	const char* m_tkmFilePath = nullptr; //tkm�t�@�C���̃t�@�C���p�X
	Light m_light;

	Vector3 m_position = g_vec3Zero;			//�ʒu
	Quaternion m_rotation = { 0.0f, 0.0f, 0.0f,1.0f };		//��]
	Vector3 m_scale = g_vec3One;				//�g��

	bool m_finishInit = false; //���������I�������

	Vector3 m_dirPower = { 0.9f,0.9f,0.9f };//�f�B���N�V�������C�g�̋���
	Vector3 m_dirDirection = { 0.0f, 0.0f, -1.0f };//�f�B���N�V�������C�g�̌���
	
	Vector3 m_ptPower = { 0.0f, 0.0f, 0.0f };//�|�C���g���C�g�̋���
	Vector3 m_ptPos = { 0.0f, 0.0f, 0.0f };//�|�C���g���C�g�̈ʒu
	float m_ptRange = 100.0f;//�|�C���g���C�g�̉e���͈�
	
	Vector3 m_spPower = { 0.0f, 0.0f, 0.0f };//�X�|�b�g���C�g�̋���
	Vector3 m_spPos = { 0.0f, 0.0f, 0.0f };//�X�|�b�g���C�g�̈ʒu
	Vector3 m_spDirection = { 0.0f, 0.0f, 0.0f };//�X�|�b�g���C�g�̌���
	float m_spRange = 300.0f;//�X�|�b�g���C�g�̉e���͈�
	float m_spAngle = 25.0f;//�X�|�b�g���C�g�̎ˏo�p�x

	Vector3 ambientPower = { 0.3f, 0.3f, 0.3f };//�����̋���
public:
	/**
	 * @brief �������֐�
	 * @param filePath tkm�t�@�C���̃t�@�C���p�X
	*/
void Init(const char* filePath,
		modelUpAxis::EnModelUpAxis modelUpAxis = modelUpAxis::enModelUpAxisZ
	);

private:
	/**
	 * @brief ���f���̏�����
	 * @param filePath tkm�t�@�C���̃t�@�C���p�X
	*/
	void InitModel(const char* filePath, modelUpAxis::EnModelUpAxis = modelUpAxis::enModelUpAxisZ);

	void InitDirectionLight(); //�f�B���N�V�������C�g
	void InitPointLight(); //�|�C���g���C�g
	void InitSpotLight(); //�|�C���g���C�g

	void InitAmbientLight(); //����

public:
	
public: //Get�֐�

	const Vector3 GetPosition()
	{
		return m_position;
	}

	const Quaternion GetRotation()
	{
		return m_rotation;
	}

	const Vector3 GetScale()
	{
		return m_scale;
	}



public: //Set�֐�

	void SetPosition(const Vector3 v)
	{
		m_position = v;
	}


	void SetRotation(const Quaternion q)
	{
		m_rotation = q;
	}

	void SetRotationX(const float f) {
		m_rotation.SetRotationX(f);
	}
	void SetRotationY(const float f) {
		m_rotation.SetRotationY(f);
	}

	void SetScale(const Vector3 v)
	{
		m_scale = v;
	}

	void SetDirectiontColor(const Vector3 v)
	{
		m_dirPower = v;
	}
	void SetPointColor(const Vector3 v)
	{
		m_ptPower = v;
	}
	void SetPointPos(const Vector3 v)
	{
		m_ptPos = v;
	}
};