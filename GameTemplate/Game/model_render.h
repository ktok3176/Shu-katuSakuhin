#pragma once
#include "Gaussianblur.h"

struct Light {
	//ディレクションライト
	Vector3 dirDirection = { 0.0f, 0.0f, 0.0f }; //ライトの方向
	//HLSL側の定数バッファのfloat3型の変数は16の倍数のアドレスに配置されるため、
	//C++側にはパディングを埋めておく。
	float pad = 0.0f;
	Vector3 dirColor = { 0.0f, 0.0f, 0.0f };
	float pad1 = 0.0f;
	//ポイントライト
	Vector3 ptPosition = { 0.0f, 0.0f, 0.0f };
	float pad2 = 0.0f;
	Vector3 ptColor = { 0.0f, 0.0f, 0.0f };
	float ptRange = 0.0f;
	//スポットライト
	Vector3 spPosition = { 0.0f, 0.0f, 0.0f };
	float pad3 = 0.0f;
	Vector3 spColor = { 0.0f, 0.0f, 0.0f };
	float spRange = 0.0f;
	Vector3 spDirection = { 0.0f, 0.0f, 0.0f };
	float spAngle = 0.0f;
	//構造体に視点の位置を追加する
	Vector3 eyePos = { 0.0f, 0.0f, 0.0f }; //視点の位置
	float pad4 = 0.0f;

	//環境光
	Vector3 ambientLight = { 0.0f, 0.0f, 0.0f };   // アンビエントライト
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
	const char* m_tkmFilePath = nullptr; //tkmファイルのファイルパス
	Light m_light;

	Vector3 m_position = g_vec3Zero;			//位置
	Quaternion m_rotation = { 0.0f, 0.0f, 0.0f,1.0f };		//回転
	Vector3 m_scale = g_vec3One;				//拡大

	bool m_finishInit = false; //初期化が終わったか

	Vector3 m_dirPower = { 0.9f,0.9f,0.9f };//ディレクションライトの強さ
	Vector3 m_dirDirection = { 0.0f, 0.0f, -1.0f };//ディレクションライトの向き
	
	Vector3 m_ptPower = { 0.0f, 0.0f, 0.0f };//ポイントライトの強さ
	Vector3 m_ptPos = { 0.0f, 0.0f, 0.0f };//ポイントライトの位置
	float m_ptRange = 100.0f;//ポイントライトの影響範囲
	
	Vector3 m_spPower = { 0.0f, 0.0f, 0.0f };//スポットライトの強さ
	Vector3 m_spPos = { 0.0f, 0.0f, 0.0f };//スポットライトの位置
	Vector3 m_spDirection = { 0.0f, 0.0f, 0.0f };//スポットライトの向き
	float m_spRange = 300.0f;//スポットライトの影響範囲
	float m_spAngle = 25.0f;//スポットライトの射出角度

	Vector3 ambientPower = { 0.3f, 0.3f, 0.3f };//環境光の強さ
public:
	/**
	 * @brief 初期化関数
	 * @param filePath tkmファイルのファイルパス
	*/
void Init(const char* filePath,
		modelUpAxis::EnModelUpAxis modelUpAxis = modelUpAxis::enModelUpAxisZ
	);

private:
	/**
	 * @brief モデルの初期化
	 * @param filePath tkmファイルのファイルパス
	*/
	void InitModel(const char* filePath, modelUpAxis::EnModelUpAxis = modelUpAxis::enModelUpAxisZ);

	void InitDirectionLight(); //ディレクションライト
	void InitPointLight(); //ポイントライト
	void InitSpotLight(); //ポイントライト

	void InitAmbientLight(); //環境光

public:
	
public: //Get関数

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



public: //Set関数

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