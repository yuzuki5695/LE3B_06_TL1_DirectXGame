#include "LevelLoader.h"
#include<json.hpp>
#include <fstream>

const std::string LevelLoader::kDefaultBaseDirectory = "Resources/levels/";
const std::string LevelLoader::kExtension = ".json";

LevelData* LevelLoader::LoadFile(const std::string& fileName) {
	
	// 連結してフルパスを得る
	const std::string fullpath = kDefaultBaseDirectory + fileName + kExtension;

	// ファイルストリーム
	std::ifstream file;

	// ファイルを開く
	file.open(fullpath);
	// ファイルオープン失敗をチェック
	if (file.fail()) {
		assert(0);
	}

	// JSON文字列から解凍したデータ
	nlohmann::json deserialized;
	
	// 解凍
	file >> deserialized;

	// 正しいレベルデータファイルかチェック
	assert(deserialized.is_object());
	assert(deserialized.contains("name"));
	assert(deserialized["name"].is_string());

	// "name"を文字列として
	std::string name = deserialized["name"].get<std::string>();
	// 正しいレベルデータファイルかチェック
	assert(name.compare("scene") == 0);

	// レベルデータ格納用インスタンスを生成
	levelData = new LevelData();
	
    // 各オブジェクトに対して再帰処理を呼び出す
    for (const auto& objectJson : deserialized["objects"]) {
        LoadObjectsRecursive(objectJson, *levelData);
    }

	return levelData;
}

void LevelLoader::LoadObjectsRecursive(const nlohmann::json& objectJson, LevelData& levelData) {
    assert(objectJson.contains("type"));
    	
    // 種別を取得
    std::string type = objectJson["type"].get<std::string>();
    
    // 種類ごとの処理
 	
    // MESH
    if (type == "MESH") { 			
        // 要素追加
        levelData.objects.emplace_back(LevelData::ObjectData{});	
        // 今追加した要素の参照を得る
        LevelData::ObjectData& objectData = levelData.objects.back();

        if (objectJson.contains("file_name")) {
            // ファイル名
            objectData.fileName = objectJson["file_name"];
        }
        // トランスフォームのパラメータ読み込み
        const auto& transform = objectJson["transform"]; 	
        // 平行移動
        objectData.translation.x = (float)transform["translation"][0];
        objectData.translation.y = (float)transform["translation"][2];
        objectData.translation.z = (float)transform["translation"][1];
       // 回転角
        objectData.rotation.x = -(float)transform["rotation"][0];
        objectData.rotation.y = -(float)transform["rotation"][2];
        objectData.rotation.z = -(float)transform["rotation"][1];
        // スケーリング
        objectData.scaling.x = (float)transform["scaling"][0];
        objectData.scaling.y = (float)transform["scaling"][2];
        objectData.scaling.z = (float)transform["scaling"][1];
    }

    // 子ノードがあれば再帰呼び出し
    if (objectJson.contains("children") && objectJson["children"].is_array()) {
        for (const auto& childJson : objectJson["children"]) {
            LoadObjectsRecursive(childJson, levelData);
        }
    }
}