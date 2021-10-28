#pragma once
#include "Gaussianblur.h"




//����model_render.h��model_render.cpp�ɑ��݂���}�W�b�N�i���o�[��萔�ȂǂɕύX���Ă������ƁB




struct Light {
	//�f�B���N�V�������C�g
	Vector3 dirDirection; //���C�g�̕���
	//HLSL���̒萔�o�b�t�@��float3�^�̕ϐ���16�̔{���̃A�h���X�ɔz�u����邽�߁A
	//C++���ɂ̓p�f�B���O�𖄂߂Ă����B
	float pad;
	Vector3 dirColor;
	float pad1;
	//�|�C���g���C�g
	Vector3 ptPosition;
	float pad2;
	Vector3 ptColor;
	float ptRange;
	//�X�|�b�g���C�g
	Vector3 spPosition;
	float pad3;
	Vector3 spColor;
	float spRange;
	Vector3 spDirection;
	float spAngle;
	//�\���̂Ɏ��_�̈ʒu��ǉ�����
	Vector3 eyePos; //���_�̈ʒu
	float pad4;

	//����
	Vector3 ambientLight;   // �A���r�G���g���C�g
};

class Gaussianblur;

class ModelRender : public IGameObject
{
public:
	ModelRender();
	~ModelRender();
	bool Start() override final;
	void Update() override final;
	void Render(RenderContext& renderContext) override final;
	
private: //data menber
	Model model;	
	const char* m_tkmFilePath = nullptr; //tkm�t�@�C���̃t�@�C���p�X
	Light light;

	Vector3 m_position = g_vec3Zero;			//�ʒu
	Quaternion m_rotation = g_quatIdentity;		//��]
	Vector3 m_scale = g_vec3One;				//�g��


	bool m_finishInit = false; //���������I�������


public:
	/**
	 * @brief �������֐�
	 * @param filePath tkm�t�@�C���̃t�@�C���p�X
	*/
	//void Init(const char* filePath);


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

	void SetScale(const Vector3 v)
	{
		m_scale = v;
	}

	void SetDirectiontColor(const Vector3 v)
	{
		light.dirColor = v;
	}
	void SetPointColor(const Vector3 v)
	{
		light.ptColor = v;
	}
	void SetPointPos(const Vector3 v)
	{
		light.ptPosition = v;
	}




};