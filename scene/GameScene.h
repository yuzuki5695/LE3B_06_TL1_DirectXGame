#pragma once

#include "Audio.h"
#include "DirectXCommon.h"
#include "DebugText.h"
#include "Input.h"
#include "Model.h"
#include "SafeDelete.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"

#include<map>
#include<string>

/// <summary>
/// ゲームシーン
/// </summary>
class GameScene {
public: // メンバ関数	
	// レベルデータ
	struct LevelData {
		// オブジェクト1個分のデータ
		struct ObjectData {
			// ファイル名
			std::string fileName;
			Vector3 translation;
			Vector3 rotation;
			Vector3 scaling;
		};
		// オブジェクトのコンテナ
		std::vector<ObjectData> objects;
	};
public: // メンバ関数
	/// <summary>
	/// コンストクラタ
	/// </summary>
	GameScene();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameScene();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;
	DebugText* debugText_ = nullptr;

	/// <summary>
	/// ゲームシーン用
	/// </summary>


	// レベルデータ格納用インスタンスを生成
	LevelData* levelData = nullptr;
	// モデルデータコンテナ
	std::map<std::string, Model*> models;

	// ワールド行列
	std::vector<WorldTransform*> worldTransforms;

	ViewProjection viewProjection;
};
