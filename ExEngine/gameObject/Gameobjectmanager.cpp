/*!
 *@brief	CGameObject�̃}�l�[�W��
 */
#include "stdafx.h"
#include "ExEngine.h"
#include "GameObjectManager.h"


GameObjectManager* GameObjectManager::m_instance = nullptr;

GameObjectManager::GameObjectManager()
{
	InitRenderTargets();
	InitSprites();
	InitShadow();
	if (m_instance != nullptr) {
		//�C���X�^���X�����łɍ���Ă���B
		std::abort();
	}
	m_instance = this;
}
GameObjectManager::~GameObjectManager()
{
	m_instance = nullptr;
}
void GameObjectManager::ExecuteUpdate()
{	
	//���S�t���O�����Ă���Q�[���I�u�W�F�N�g��j������B
	for (auto& goList : m_gameObjectListArray) {
		goList.remove_if([&](IGameObject* go) {	
			if (go->IsDead()) {
				delete go;
				return true;
			}
			return false;	 
		});
	}

	for (auto& goList : m_gameObjectListArray) {
		for (auto& go : goList) {
			go->StartWrapper();
		}
	}


	for (auto& goList : m_gameObjectListArray) {
		for (auto& go : goList) {
			go->UpdateWrapper();
		}
	}
	//�����G���W���̃A�b�v�f�[�g���Ăяo���B
	PhysicsWorld::GetInstance()->Update(1.0f/60.0f);
}
void GameObjectManager::ExecuteRender(RenderContext& renderContext)
{
	
	
		
	
	
	teaShadow.z -= g_pad[0]->GetLStickYF();
	teaShadow.x -= g_pad[0]->GetLStickXF();

	
	teapotShadowModel.UpdateWorldMatrix(
		teaShadow,
		g_quatIdentity,
		g_vec3One
	);
	qAddRot.SetRotationDegX(g_pad[0]->GetLStickYF());
	g_camera3D->RotateOriginTarget(qAddRot);
	// �V���h�E�}�b�v�Ƀ����_�����O
	   // �����_�����O�^�[�Q�b�g���V���h�E�}�b�v�ɕύX����
	renderContext.WaitUntilToPossibleSetRenderTarget(shadowMap);
	renderContext.SetRenderTargetAndViewport(shadowMap);
	renderContext.ClearRenderTargetView(shadowMap);

	// �e���f����`��
	teapotShadowModel.Draw(renderContext, lightCamera);

	// �������݊����҂�
	renderContext.WaitUntilFinishDrawingToRenderTarget(shadowMap);
	// �ʏ탌���_�����O
	  // �����_�����O�^�[�Q�b�g���t���[���o�b�t�@�ɖ߂�
	renderContext.SetRenderTarget(
		g_graphicsEngine->GetCurrentFrameBuffuerRTV(),
		g_graphicsEngine->GetCurrentFrameBuffuerDSV()
	);
	renderContext.SetViewportAndScissor(g_graphicsEngine->GetFrameBufferViewport());

	//�����_���[��ύX����Ȃ炱�����������Ă����Ɨǂ��B
	// step-1 �����_�����O�^�[�Q�b�g��mainRenderTarget�ɐݒ肷��
	renderContext.WaitUntilToPossibleSetRenderTarget(mainRenderTarget);
	// �����_�����O�^�[�Q�b�g��ݒ�
	renderContext.SetRenderTargetAndViewport(mainRenderTarget);
	// �����_�����O�^�[�Q�b�g���N���A
	renderContext.ClearRenderTargetView(mainRenderTarget);
	for (auto& goList : m_gameObjectListArray) {
		for (auto& go : goList) {
			go->RenderWrapper(renderContext);
			// �e���󂯂�w�i��`��
			
		}
	}
	bgModel.Draw(renderContext);
	
	// �����_�����O�^�[�Q�b�g�ւ̏������ݏI���҂�
	renderContext.WaitUntilFinishDrawingToRenderTarget(mainRenderTarget);
	// step-2 �u���[���̏���
	// �P�x���o���u���[������
	// �P�x���o
	// �P�x���o�p�̃����_�����O�^�[�Q�b�g�ɕύX
	renderContext.WaitUntilToPossibleSetRenderTarget(luminnceRenderTarget);

	// �����_�����O�^�[�Q�b�g��ݒ�
	renderContext.SetRenderTargetAndViewport(luminnceRenderTarget);

	// �����_�����O�^�[�Q�b�g���N���A
	renderContext.ClearRenderTargetView(luminnceRenderTarget);

	// �P�x���o���s��
	luminanceSprite.Draw(renderContext);

	// �����_�����O�^�[�Q�b�g�ւ̏������ݏI���҂�
	renderContext.WaitUntilFinishDrawingToRenderTarget(luminnceRenderTarget);

	// step-3 �K�E�V�A���u���[��4����s����
	gaussianBlur[0].Render(renderContext, 10);
	gaussianBlur[1].Render(renderContext, 10);
	gaussianBlur[2].Render(renderContext, 10);
	gaussianBlur[3].Render(renderContext, 10);
	// step-4 �{�P�摜���������ă��C�������_�����O�^�[�Q�b�g�ɉ��Z����
	renderContext.WaitUntilToPossibleSetRenderTarget(mainRenderTarget);
	renderContext.SetRenderTargetAndViewport(mainRenderTarget);
	finalSprite.Draw(renderContext);
	renderContext.WaitUntilFinishDrawingToRenderTarget(mainRenderTarget);
	// step-3 ���C�������_�����O�^�[�Q�b�g�̓��e���t���[���o�b�t�@�ɃR�s�[����
	renderContext.SetRenderTarget(
		g_graphicsEngine->GetCurrentFrameBuffuerRTV(),
		g_graphicsEngine->GetCurrentFrameBuffuerDSV()
	);
	//bgModel.Draw(renderContext);

	copyToFrameBufferSprite.Draw(renderContext);

}

void GameObjectManager::InitRenderTargets() {
	mainRenderTarget.Create(
		1280,
		720,
		1,
		1,
		// �y���ځz�J���[�o�b�t�@�[�̃t�H�[�}�b�g��32bit���������_�ɂ��Ă���
		DXGI_FORMAT_R32G32B32A32_FLOAT,
		DXGI_FORMAT_D32_FLOAT
	);

	luminnceRenderTarget.Create(
		1280,       // �𑜓x�̓��C�������_�����O�^�[�Q�b�g�Ɠ���
		720,        // �𑜓x�̓��C�������_�����O�^�[�Q�b�g�Ɠ���
		1,
		1,
		// �y���ځz�J���[�o�b�t�@�[�̃t�H�[�}�b�g��32bit���������_�ɂ��Ă���
		DXGI_FORMAT_R32G32B32A32_FLOAT,
		DXGI_FORMAT_D32_FLOAT
	);
}
void GameObjectManager::InitSprites() {
	// �P�x���o�p�̃V�F�[�_�[�̃t�@�C���p�X���w�肷��
	luminanceSpriteInitData.m_fxFilePath = "Assets/shader/samplePostEffect.fx";

	// ���_�V�F�[�_�[�̃G���g���[�|�C���g���w�肷��
	luminanceSpriteInitData.m_vsEntryPointFunc = "VSMain";

	// �s�N�Z���V�F�[�_�[�̃G���g���[�|�C���g���w�肷��
	luminanceSpriteInitData.m_psEntryPoinFunc = "PSSamplingLuminance";

	// �X�v���C�g�̕��ƍ�����luminnceRenderTarget�Ɠ���
	luminanceSpriteInitData.m_width = 1280;
	luminanceSpriteInitData.m_height = 720;

	// �e�N�X�`���̓��C�������_�����O�^�[�Q�b�g�̃J���[�o�b�t�@�[
	luminanceSpriteInitData.m_textures[0] = &mainRenderTarget.GetRenderTargetTexture();

	// �`�����ރ����_�����O�^�[�Q�b�g�̃t�H�[�}�b�g���w�肷��
	luminanceSpriteInitData.m_colorBufferFormat[0] = DXGI_FORMAT_R32G32B32A32_FLOAT;


	luminanceSprite.Init(luminanceSpriteInitData);

	// �K�E�V�A���u���[��������

	gaussianBlur[0].Init(&luminnceRenderTarget.GetRenderTargetTexture());
	gaussianBlur[1].Init(&gaussianBlur[0].GetBokeTexture());
	gaussianBlur[2].Init(&gaussianBlur[1].GetBokeTexture());
	gaussianBlur[3].Init(&gaussianBlur[2].GetBokeTexture());

	// �{�P�摜���������ď������ނ��߂̃X�v���C�g��������


	finalSpriteInitData.m_textures[0] = &gaussianBlur[0].GetBokeTexture();
	finalSpriteInitData.m_textures[1] = &gaussianBlur[1].GetBokeTexture();
	finalSpriteInitData.m_textures[2] = &gaussianBlur[2].GetBokeTexture();
	finalSpriteInitData.m_textures[3] = &gaussianBlur[3].GetBokeTexture();

	finalSpriteInitData.m_width = 1280;
	finalSpriteInitData.m_height = 720;

	finalSpriteInitData.m_fxFilePath = "Assets/shader/samplePostEffect.fx";
	finalSpriteInitData.m_psEntryPoinFunc = "PSBloomFinal";
	finalSpriteInitData.m_alphaBlendMode = AlphaBlendMode_Add;
	finalSpriteInitData.m_colorBufferFormat[0] = DXGI_FORMAT_R32G32B32A32_FLOAT;

	finalSprite.Init(finalSpriteInitData);
	// mainRenderTarget�̃e�N�X�`�����t���[���o�b�t�@�[�ɓ\��t���邽�߂̃X�v���C�g������������
	// �X�v���C�g�̏������I�u�W�F�N�g���쐬����


	// �e�N�X�`����mainRenderTarget�̃J���[�o�b�t�@�[
	spriteInitData.m_textures[0] = &mainRenderTarget.GetRenderTargetTexture();
	spriteInitData.m_width = 1280;
	spriteInitData.m_height = 720;

	// ���m�N���p�̃V�F�[�_�[���w�肷��
	spriteInitData.m_fxFilePath = "Assets/shader/sample2D.fx";


	copyToFrameBufferSprite.Init(spriteInitData);
}
void GameObjectManager::InitShadow() {

	// �J�����̈ʒu��ݒ�B����̓��C�g�̈ʒu
	lightCamera.SetPosition(cameraSetPos);

	// �J�����̒����_��ݒ�B���ꂪ���C�g���Ƃ炵�Ă���ꏊ
	lightCamera.SetTarget(cameraSetTarget);

	// �������ݒ�B����̓��C�g���^���������Ă���̂ŁAX��������ɂ��Ă���
	lightCamera.SetUp(cameraSetUp);

	// ���C�g�r���[�v���W�F�N�V�����s����v�Z���Ă���
	lightCamera.Update();

	// step-1 �V���h�E�}�b�v�`��p�̃����_�����O�^�[�Q�b�g���쐬����
	float clearColor[4] = { 1.0f, 1.0f, 1.0f, 1.0f };

	shadowMap.Create(
		1024,
		1024,
		1,
		1,
		//�y���ځz�V���h�E�}�b�v�̃J���[�o�b�t�@�̃t�H�[�}�b�g��ύX���Ă���B
		DXGI_FORMAT_R32_FLOAT,
		DXGI_FORMAT_D32_FLOAT,
		clearColor
	);
	// step-2 �V���h�E�}�b�v�ɕ`�悷�郂�f��������������

	//�V���h�E�}�b�v�`��p�̃V�F�[�_�[���w�肷��B
	teapotShadowModelInitData.m_fxFilePath = "Assets/shader/sampleDrawShadowMap.fx";
	teapotShadowModelInitData.m_tkmFilePath = "Assets/modelData/teapot.tkm";

	//�y���ځz�J���[�o�b�t�@�̃t�H�[�}�b�g�ɕύX���������̂ŁA��������ύX����B
	teapotShadowModelInitData.m_colorBufferFormat[0] = DXGI_FORMAT_R32_FLOAT;
	//teaShadow.x -= g_pad[0]->GaetLStickXF();
	//if (g_pad[0]->IsPress(enButtonB)) {
	//	teaShadow.y += g_pad[0]->GetLStickYF();
	//}
	//else {
	//	teaShadow.z -= g_pad[0]->GetLStickYF();
	//}

	teapotShadowModel.Init(teapotShadowModelInitData);
	teapotShadowModel.UpdateWorldMatrix(
		teaShadow,
		g_quatIdentity,
		g_vec3One
	);
	// �e���󂯂�w�i���f����������

	// �V���h�E���V�[�o�[�i�e�����Ƃ���郂�f���j�p�̃V�F�[�_�[���w�肷��
	bgModelInitData.m_fxFilePath = "Assets/shader/sampleShadowReciever.fx";

	// �V���h�E�}�b�v���g��SRV�ɐݒ肷��
	bgModelInitData.m_expandShaderResoruceView = &shadowMap.GetRenderTargetTexture();

	// ���C�g�r���[�v���W�F�N�V�����s����g���萔�o�b�t�@�[�ɐݒ肷��
	bgModelInitData.m_expandConstantBuffer = (void*)&lightCamera.GetViewProjectionMatrix();
	bgModelInitData.m_expandConstantBufferSize = sizeof(lightCamera.GetViewProjectionMatrix());
	bgModelInitData.m_tkmFilePath = "Assets/modelData/bg/bg.tkm";

	bgModel.Init(bgModelInitData);
}