#pragma once
#include "Gaussianblur.h"




//このmodel_render.hとmodel_render.cppに存在するマジックナンバーを定数などに変更しておくこと。




struct Light {
	//ディレクションライト
	Vector3 dirDirection; //ライトの方向
	//HLSL側の定数バッファのfloat3型の変数は16の倍数のアドレスに配置されるため、
	//C++側にはパディングを埋めておく。
	float pad;
	Vector3 dirColor;
	float pad1;
	//ポイントライト
	Vector3 ptPosition;
	float pad2;
	Vector3 ptColor;
	float ptRange;
	//スポットライト
	Vector3 spPosition;
	float pad3;
	Vector3 spColor;
	float spRange;
	Vector3 spDirection;
	float spAngle;
	//構造体に視点の位置を追加する
	Vector3 eyePos; //視点の位置
	float pad4;

	//環境光
	Vector3 ambientLight;   // アンビエントライト
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
	const char* m_tkmFilePath = nullptr; //tkmファイルのファイルパス
	Light light;

	Vector3 m_position = g_vec3Zero;			//位置
	Quaternion m_rotation = g_quatIdentity;		//回転
	Vector3 m_scale = g_vec3One;				//拡大


	bool m_finishInit = false; //初期化が終わったか


public:
	/**
	 * @brief 初期化関数
	 * @param filePath tkmファイルのファイルパス
	*/
	//void Init(const char* filePath);


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