#pragma once

/// <summary>
/// デプスシャドウクラス。
/// </summary>
class DepthShadow : public IGameObject
{
public:
	/// <summary>
	/// 初期化。
	/// </summary>
	/// <param name="filePath">シャドウマップ作成用モデル。</param>
	void Init(const char* filePath, const char* BGfilePath);
	/// <summary>
	void InitShadow(const char* filePath, const char* BGfilePath);
	/// <summary>

/// <param name="rc">レンダリングターゲット</param>
	void Render(RenderContext& renderContext) override final;


private:
	/// <summary>
	/// レンダリングターゲットを初期化。
	/// </summary>
	void InitRenderTargets();


private:
	const char* m_tkmFilePath = nullptr; //シャドウ生成用tkmファイルのファイルパス
	const char* m_bgTkmFilePath = nullptr; //シャドウ落とす用tkmファイルのファイルパス
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



