#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>

GameScene::GameScene() {}

GameScene::~GameScene() {}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	debugText_ = DebugText::GetInstance();
	
	// LevelLoader のインスタンスを生成
	levelLoader = new LevelLoader();

	// JSONファイルからレベルデータを読み込む
	levelData = levelLoader->LoadFile("untitled");

	// レベルデータに出現するモデルの読み込み
	for (auto& objectData : levelData->objects) {
		// ファイル名から登録済みモデルを検索
		decltype(models)::iterator it = models.find(objectData.fileName);

		// 見込みの場合、読み込む
		if (it == models.end()) {
			Model* model = Model::CreateFromOBJ(objectData.fileName);
			models[objectData.fileName] = model;
		}
	}

	// レベルデータからワールド行列を生成
	for (auto& objectData : levelData->objects) {
		// ワールド行列を生成
		WorldTransform* worldTransform = new WorldTransform();
		// 座標
		worldTransform->translation_ = objectData.translation;
		// 回転角
		worldTransform->rotation_ = objectData.rotation;
		// スケーリング
		worldTransform->scale_ = objectData.scaling;

		worldTransform->Initialize();

		// 配列に登録
		worldTransforms.push_back(worldTransform);
	}
	viewProjection.Initialize();
}

void GameScene::Update() {
	for (WorldTransform* worldTransform : worldTransforms) {
		//// TDOO:: ワールド行列の計算
		//worldTransform->matWorld_ = MathUtility::Matrix4Transform(worldTransform->scale_, worldTransform->rotation_, worldTransform->translation_);


		worldTransform->TransferMatrix();
	}
}

void GameScene::Draw() {

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>
			
	int i = 0;
	// レベルデータからオブジェクトを生成、配置
	for (auto& objectData : levelData->objects) {
		Model* model = nullptr;
		// ファイル名から登録済みモデルを検索
		decltype(models)::iterator it = models.find(objectData.fileName);
		if (it != models.end()) { model = it->second; }
		model->Draw(*worldTransforms[i], viewProjection);

		i++;
	}

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	// デバッグテキストの描画
	debugText_->DrawAll(commandList);
	//
	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}
