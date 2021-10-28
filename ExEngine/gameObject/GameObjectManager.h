﻿/*!
 *@brief	CGameObjectのマネージャー。
 */

#pragma once

#include "gameObject/IGameObject.h"
#include "RenderTarget.h"
#include "GraphicsEngine.h"
#include "Sprite.h"
#include <functional>
#include <vector>
#include <Model.h>
#include <HID/GamePad.h>
#include <MiniEngine.h>
#include "../../GameTemplate/Game/Gaussianblur.h"

/// <summary>
/// GameObjectManagerクラス
/// <summary>
/// <remark>
/// このクラスはシングルトンパターンで設計されています。
/// インスタンスの生成はCreateInstance()、
/// インスタンスの破棄はDeleteInstance()、
/// インスタンスの取得はCreateInstance()を呼び出してください。
/// シングルトンパターンは下記の二つの機能を提供するパターンです。
/// 1.　グローバルなアクセスポイント
///		->グローバル変数の機能
/// 2　インスタンスの数を一つに制限する機能。
/// </reramk>
/// 
class GameObjectManager {
private:
	GameObjectManager();
	~GameObjectManager();
public: //静的メンバ関数
	/// <summary>
	/// インスタンスの作成。
	/// </summary>
	static void CreateInstance()
	{
		m_instance = new GameObjectManager;
	}
	/// <summary>
	/// インスタンスの破棄。
	/// </summary>
	static void DeleteInstance()
	{
		delete m_instance;
	}
	/// <summary>
	/// インスタンスを取得。
	/// </summary>
	/// <returns></returns>
	static GameObjectManager* GetInstance()
	{
		return m_instance;
	}
public:
	/// <summary>
	/// 更新処理を実行
	/// </summary>
	void ExecuteUpdate();
	/// <summary>
	/// 描画処理を実行。
	/// </summary>
	/// <param name="rc"></param>
	void ExecuteRender(RenderContext& rc);
	
	
	/*!
	*@brief	ゲームオブジェクトのnew
	*@details
	* この関数を使用してnewしたオブジェクトは必ずDeleteGameObjectを実行することでdeleteされます。
	*@param[in]	prio		実行優先順位。
	*@param[in]	objectName	オブジェクト名。
	*/
	template<class T>
	T* NewGameObject(int prio, const char* objectName)
	{
		T* newObject = new T();
		m_gameObjectListArray.at(prio).push_back(newObject);
		return newObject;
	}
	/*!
		*@brief	ゲームオブジェクトの削除。
		*/
	void DeleteGameObject( IGameObject* gameObject )
	{
		if (gameObject != nullptr) {
			gameObject->Dead();
		}
	}
	/*!
	*@brief	ゲームオブジェクトの検索。
	*@details
	* 重いよ！
	*@param[in]	objectName		オブジェクト名。
	*/
	template<class T>
	T* FindGameObject(const char* objectName)
	{
		
		for (auto goList : m_gameObjectListArray) {
			for (auto go : goList) {
				if (go->IsMatchName(objectName)) {
					//見つけた。
					T* p = dynamic_cast<T*>(go);
					return p;
				}
			}
		}
		
		//見つからなかった。
		return nullptr;
	}
	template<class T>
	void QueryGameObjects(const char* objectName, std::function<bool(T* go)> func)
	{
		for (auto goList : m_gameObjectListArray) {
			for (auto go : goList) {
				if (strcmp(go->m_name.cstr(), objectName) == 0) {
					//見つけた。
					T* p = dynamic_cast<T*>(go);
					if (func(p) == false) {
						//クエリ中断。
						return;
					}
				}
			}
		}
	}
	
private:
	//ここからシャドウ関係
	float clearColor[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
	RenderTarget shadowMap; 
	Camera lightCamera;
	ModelInitData teapotShadowModelInitData;
	Model teapotShadowModel;
	ModelInitData bgModelInitData;
	Model bgModel;
	Quaternion qAddRot;
	Vector3 teaShadow = { 0.0f, 50.0f, 0.0f };
	Vector3 cameraSetPos = { 0.0f, 500.0f, 0.0f };
	Vector3 cameraSetTarget = { 0.0f, 0.0f, 0.0f };
	Vector3 cameraSetUp = { 1.0f, 0.0f, 0.0f };

	void InitShadow();
	//ここまでシャドウ関係
	
	//ここからブルーム関係
	RenderTarget mainRenderTarget;
	RenderTarget luminnceRenderTarget;
	SpriteInitData finalSpriteInitData;
	SpriteInitData spriteInitData;
	Sprite luminanceSprite;
	Gaussianblur gaussianBlur[4];
	Sprite copyToFrameBufferSprite;
	Sprite finalSprite;
	SpriteInitData luminanceSpriteInitData;
	void InitRenderTargets();
	void InitSprites();
	//ここまでブルーム関係
	enum { GAME_OBJECT_PRIO_MAX = 255 };		//!<ゲームオブジェクトの優先度の最大値。
	typedef std::list<IGameObject*>	 GameObjectList;
	std::array<GameObjectList, GAME_OBJECT_PRIO_MAX>	m_gameObjectListArray;							//!<ゲームオブジェクトの優先度付きリスト。
	static GameObjectManager* m_instance;		//唯一のインスタンスのアドレスを記録する変数。
};


/*!
*@brief	ゲームオブジェクトの検索のヘルパー関数。
*@details
* 名前の検索が入るため遅いです。
*@param[in]	objectName	ゲームオブジェクトの名前。
*@return 見つかったインスタンスのアドレス。見つからなかった場合はnullptrを返す。
*/
template<class T>
static inline T* FindGO(const char* objectName)
{
	return GameObjectManager::GetInstance()->FindGameObject<T>(objectName);
}
/*!
*@brief	ゲームオブジェクトの検索のヘルパー関数。
*@details
* 同名のゲームオブジェクトに全てに対して、何らかの処理を行いたい場合に使用してください。
*@param[in]	objectName	ゲームオブジェクトの名前。
*@param[in]	func		ゲームオブジェクトが見つかったときに呼ばれるコールバック関数。
*/
template<class T>
static inline void QueryGOs(const char* objectName, std::function<bool(T* go)> func)
{
	return GameObjectManager::GetInstance()->QueryGameObjects<T>(objectName, func);
}
/*!
	*@brief	ゲームオブジェクト生成のヘルパー関数。
	*@param[in]	priority	プライオリティ。
	*@param[in]	objectName	オブジェクト名。(NULLの指定可）
	*/
template<class T>
static inline T* NewGO( int priority, const char* objectName = nullptr)
{
	return GameObjectManager::GetInstance()->NewGameObject<T>( priority, objectName);
}
	
/*!
	*@brief	ゲームオブジェクト削除のヘルパー関数。
	* NewGOを使用して作成したオブジェクトは必ずDeleteGOを実行するように。
	*@param[in]	go		削除するゲームオブジェクト。
	*/
static inline void DeleteGO(IGameObject* go)
{
	GameObjectManager::GetInstance()->DeleteGameObject(go);
}



	

 