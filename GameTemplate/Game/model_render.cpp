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

	/*g_camera3D->SetPosition({ 0.0f, -50.0f, 150.0f });
	g_camera3D->SetTarget({ 0.0f, 0.0f, 0.0f });*/

	//モデルの初期化するための情報を設定
	ModelInitData initData;
	//tkmファイルのファイルパスを設定
	initData.m_tkmFilePath = filePath;

	//使用するシェーダーファイルパスを設定
	initData.m_fxFilePath = "Assets/shader/model.fx";
	//モデルの上方向を指定
	initData.m_modelUpAxis = modelUpAxis;
	initData.m_expandConstantBuffer = &light;
	initData.m_expandConstantBufferSize = sizeof(light);
	//初期化情報を使ってモデル表示処理を初期化する
	initData.m_colorBufferFormat[0] = DXGI_FORMAT_R32G32B32A32_FLOAT;
	model.Init(initData);
}

//////////////////////////////
// ライトごとの初期化
//////////////////////////////

void ModelRender::InitDirectionLight()
{
	
	//ライトのカラーの設定（ライトの強さ）
	light.dirColor.x = 0.8f;
	light.dirColor.y = 0.8f;
	light.dirColor.z = 0.8f;
	//ライトは斜め上から当たっている。
	
	light.dirDirection.x = 0.0f;
	light.dirDirection.y = 0.0f;
	light.dirDirection.z = -1.0f;
	//正規化する。
	light.dirDirection.Normalize();
	

	//視点の位置を設定
	light.eyePos = g_camera3D->GetPosition();
}
void ModelRender::InitPointLight()
{
	//ポイントライトの初期座標を設定する
	light.ptPosition.x = 0.0f;
	light.ptPosition.y = 50.0f;
	light.ptPosition.z = 50.0f;

	//ポイントライトの初期カラーを設定する
	light.ptColor.x = 0.0f;
	light.ptColor.y = 0.0f;
	light.ptColor.z = 0.0f;

	//ポイントライトの影響範囲を設定する
	light.ptRange = 100.0f;
}
void ModelRender::InitSpotLight()
{//スポットライト
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
	//環境光
	light.ambientLight.x = 0.3f;
	light.ambientLight.y = 0.3f;
	light.ambientLight.z = 0.3f;
}

////////////////////////////////////////////////////////////
// 描画処理
////////////////////////////////////////////////////////////

void ModelRender::Render(RenderContext& renderContext)
{
	// mainRenderTargetに各種モデルを描画する
	model.Draw(renderContext);


	
	light.dirColor.x += g_pad[0]->GetLStickXF() * 0.5f;
	light.dirColor.y += g_pad[0]->GetLStickXF() * 0.5f;
	light.dirColor.z += g_pad[0]->GetLStickXF() * 0.5f;
	
}

void ModelRender::Update()
{//未初期化時
	if (m_finishInit == false) {
		return;
	}
	//モデルの座標更新
	
	model.UpdateWorldMatrix(m_position, m_rotation, m_scale);

}


