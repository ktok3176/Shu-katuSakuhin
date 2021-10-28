#pragma once

/// <summary>
/// �f�v�X�V���h�E�N���X�B
/// </summary>
class DepthShadow : public IGameObject
{
public:
	/// <summary>
	/// �������B
	/// </summary>
	/// <param name="filePath">�V���h�E�}�b�v�쐬�p���f���B</param>
	void Init(const char* filePath, const char* BGfilePath);
	/// <summary>
	void InitShadow(const char* filePath, const char* BGfilePath);
	/// <summary>

/// <param name="rc">�����_�����O�^�[�Q�b�g</param>
	void Render(RenderContext& renderContext) override final;


private:
	/// <summary>
	/// �����_�����O�^�[�Q�b�g���������B
	/// </summary>
	void InitRenderTargets();


private:
	const char* m_tkmFilePath = nullptr; //�V���h�E�����ptkm�t�@�C���̃t�@�C���p�X
	const char* m_bgTkmFilePath = nullptr; //�V���h�E���Ƃ��ptkm�t�@�C���̃t�@�C���p�X
	float clearColor[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
	RenderTarget shadowMap;
	Camera lightCamera;
	ModelInitData m_shadowModelInitData;
	Model m_shadowModel;
	ModelInitData bgModelInitData;
	Model bgModel;
	Quaternion qAddRot;
	Vector3 m_shadow = { 0.0f, 50.0f, 0.0f };
	Vector3 cameraSetPos = { 0.0f, 500.0f, 0.0f };
	Vector3 cameraSetTarget = { 0.0f, 0.0f, 0.0f };
	Vector3 cameraSetUp = { 1.0f, 0.0f, 0.0f };
};



