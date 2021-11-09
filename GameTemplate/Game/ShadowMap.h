#pragma once
//�V���h�E�}�b�v�N���X
class ShadowMap : public IGameObject
{
public:
	/// <summary>
	/// �������B
	/// </summary>
	/// <param name="filePath">�V���h�E�}�b�v�쐬�p���f���B</param>
	/// <param name="BGfilePath">�V���h�E���󂯂郂�f���B</param>
	void Init(const char* filePath, const char* BGfilePath);
	/// <summary>
	void InitShadow(const char* filePath, const char* BGfilePath);
	/// <summary>

	void Draw(RenderContext& renderContext);
	void BGDraw(RenderContext& renderContext);
private:
	int m_wide = 1024;
	int m_high = 1024;
	int m_mipLv = 1;
	int m_arraySize = 1;
	const char* m_tkmFilePath = nullptr; //�V���h�E�����ptkm�t�@�C���̃t�@�C���p�X
	const char* m_bgTkmFilePath = nullptr; //�V���h�E���Ƃ��ptkm�t�@�C���̃t�@�C���p�X
	float m_clearColor[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
	RenderTarget m_shadowMap;
	Camera m_lightCamera;
	ModelInitData m_shadowModelInitData;
	Model m_shadowModel;
	ModelInitData m_bgModelInitData;
	Model m_bgModel;
	Quaternion m_rotation;
	Vector3 m_scale = { 1.0f, 1.0f, 1.0f };
	Vector3 m_shadowPos = { -20.0f, 50.0f, 20.0f };
	Vector3 m_cameraSetPos = { 0.0f, 500.0f, 0.0f };
	Vector3 m_cameraSetTarget = { 0.0f, 0.0f, 0.0f };
	Vector3 m_cameraSetUp = { 1.0f, 0.0f, 0.0f };

public:
	void SetPosition(const Vector3 v)
	{
		m_shadowPos = v;
	}
};

