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
// 初期化
////////////////////////////////////////////////////////////



void ModelRender::Init(const char* filePath,
	modelUpAxis::EnModelUpAxis modelUpAxis
)
{	
	////tkmファイルのファイルパスを保持
	m_tkmFilePath = filePath;
	////モデルの初期化
	InitModel(filePath, modelUpAxis);
	//初期化完了
	m_finishInit = true;
}

	


void ModelRender::InitModel(const char* filePath, modelUpAxis::EnModelUpAxis modelUpAxis)
{
	//ライトの設定をする。
	//ディレクションライトを初期化する
	InitDirectionLight();

	//ポイントライトを初期化する
    InitPointLight();

	//スポットライトを初期化する
	InitSpotLight();
	
	//環境光を初期化する
	InitAmbientLight();

	//モデルの初期化するための情報を設定
	ModelInitData initData;
	//tkmファイルのファイルパスを設定
	initData.m_tkmFilePath = filePath;

	//使用するシェーダーファイルパスを設定
	initData.m_fxFilePath = "Assets/shader/model.fx";
	//モデルの上方向を指定
	initData.m_modelUpAxis = modelUpAxis;
	initData.m_expandConstantBuffer = &m_light;
	initData.m_expandConstantBufferSize = sizeof(m_light);
	//初期化情報を使ってモデル表示処理を初期化する
	initData.m_colorBufferFormat[0] = DXGI_FORMAT_R32G32B32A32_FLOAT;
	m_model.Init(initData);
}

//////////////////////////////
// ライトごとの初期化
//////////////////////////////

void ModelRender::InitDirectionLight()
{
	//ディレクションライトの初期カラーを設定する
	m_light.dirColor = m_dirPower;
	//ディレクションライトの方向を設定する
	m_light.dirDirection = m_dirDirection;
	//正規化する。
	m_light.dirDirection.Normalize();
	
	//視点の位置を設定
	m_light.eyePos = g_camera3D->GetPosition();
}
void ModelRender::InitPointLight()
{
	//ポイントライトの初期カラーを設定する
	m_light.ptColor = m_ptPower;

	//ポイントライトの初期座標を設定する
	m_light.ptPosition = m_ptPos;

	//ポイントライトの影響範囲を設定する
	m_light.ptRange = m_ptRange;
}
void ModelRender::InitSpotLight()
{//スポットライトの初期カラーを設定する
	m_light.spColor = m_spPower;

	//スポットライトの初期座標を設定する
	m_light.spPosition = m_spPos;

	//スポットライトの方向を設定する
	m_light.spDirection = m_spDirection;
	//正規化する。
	m_light.spDirection.Normalize();

	m_light.spRange = m_spRange;

	m_light.spAngle = Math::DegToRad(m_spAngle);
}

void ModelRender::InitAmbientLight()
{
	//環境光の強さ
	m_light.ambientLight = ambientPower;
}

////////////////////////////////////////////////////////////
// 描画処理
////////////////////////////////////////////////////////////

void ModelRender::Render(RenderContext& renderContext)
{
	// mainRenderTargetに各種モデルを描画する
	m_model.Draw(renderContext);
	
	m_light.dirColor.x += g_pad[0]->GetLStickXF() * 0.5f;
	m_light.dirColor.y += g_pad[0]->GetLStickXF() * 0.5f;
	m_light.dirColor.z += g_pad[0]->GetLStickXF() * 0.5f;
}

void ModelRender::Update()
{//未初期化時
	if (m_finishInit == false) {
		return;
	}
	//モデルの座標更新
	m_model.UpdateWorldMatrix(m_position, m_rotation, m_scale);
}


