﻿#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include "Input.h"

class Player
{
public:
	///<summary>
	///初期化
	/// </summary>
	void Initialize(Model*model,uint32_t textureHandle);

	///< summary>
	/// 更新
	///  </summary>
	void Update();

	///< summary>
	/// 描画
	///  </summary>
	void Draw(ViewProjection& viewProjection);

private:
	//ワ－ルド変換デ－タ
	WorldTransform worldTransform_;
	//モデル
	Model* model_ = nullptr;
	//テクスチャハンドル
	uint32_t textureHandle_ = 0u;
	//キー入力
	Input* input_ = nullptr;

};