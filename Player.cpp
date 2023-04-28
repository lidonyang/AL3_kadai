﻿#include"Player.h"
#include<cassert>

#include "ImGuiManager.h"

void Player::Initialize(Model* model, uint32_t textureHandle) 
{ 
	//NULLポインタチェック
	assert(model); 

	//受け渡し
	textureHandle_ = textureHandle;
	model_ = model;

	//ワールド変換の初期化
	worldTransform_.Initialize();

	//シングルトンインスタンスを取得する
	input_ = Input::GetInstance();
}

void Player::Update() 
{
	// キャラクターの移動ベクトル
	Vector3 move = {0, 0, 0};

	// キャラクターの移動速さ
	const float kCharacterSpeed = 0.2f;

	// 押した方向移動
	if (input_->PushKey(DIK_UP)) {
		move.y += kCharacterSpeed;
	} 
	else if (input_->PushKey(DIK_DOWN)) {
		move.y -= kCharacterSpeed;
	} 
	else if (input_->PushKey(DIK_LEFT)) {
		move.x -= kCharacterSpeed;
	} 
	else if (input_->PushKey(DIK_RIGHT)) {
		move.x += kCharacterSpeed;
	}

	// 移動制限
	const float kMoveLimitX = 30.0f;
	const float kMoveLimitY = 30.0f;

	//範囲を超えない処理
	worldTransform_.translation_.x = max(worldTransform_.translation_.x, -kMoveLimitX);
	worldTransform_.translation_.x = min(worldTransform_.translation_.x, +kMoveLimitX);
	worldTransform_.translation_.y = max(worldTransform_.translation_.y, -kMoveLimitY);
	worldTransform_.translation_.y = min(worldTransform_.translation_.y, +kMoveLimitY);

	//移動
	worldTransform_.translation_.x += move.x;
	worldTransform_.translation_.y += move.y;
	worldTransform_.translation_.z += move.z;

	//行列更新
	worldTransform_.matWorld_ = MakeAffineMatrix(worldTransform_.scale_, 
		worldTransform_.rotation_, worldTransform_.translation_);
	
	//行列転送
	worldTransform_.TransferMatrix();

	//ImGui
	ImGui::Begin("player");
	float sliderValue[3] = {
	    worldTransform_.translation_.x, worldTransform_.translation_.y,
	    worldTransform_.translation_.z};
	ImGui::SliderFloat3("position", sliderValue, 20.0f, 20.0f);
	worldTransform_.translation_ = {sliderValue[0], sliderValue[1], sliderValue[2]};
	ImGui::End();

	


}

void Player::Draw(ViewProjection& viewProjection) 
{
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}
