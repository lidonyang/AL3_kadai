#include"Player.h"
#include"Vector3.h"
#include<cassert>

#include "ImGuiManager.h"

Player::~Player()
{
	//buller_の解放
	for (PlayerBullet* bullet : bullets_) {
		delete bullet;
	}
	delete sprite2DReticle_;
}

void Player::Initialize(Model* model, uint32_t textureHandle, Vector3 position) { 
	//NULLポインタチェック
	assert(model); 

	//受け渡し
	textureHandle_ = textureHandle;
	model_ = model;

	worldTransform_.translation_ = position;
	//ワールド変換の初期化
	worldTransform_.Initialize();

	//シングルトンインスタンスを取得する
	input_ = Input::GetInstance();

	worldTransform3DReticle_.Initialize();

	model2_ = model;

	// レティクルスプライト
	uint32_t textureReticle = TextureManager::Load("target.png");
	sprite2DReticle_ = Sprite::Create(textureReticle, {640, 360}, {1, 1, 1, 1}, {0.5, 0.5});
}

void Player::Update(ViewProjection& viewProjection) {

	//デスフラグの立った弾の削除
	bullets_.remove_if([](PlayerBullet * bullet) {
		if (bullet->IsDead()) {
			delete bullet;
			return true;
		}
		return false;
	});


	// キャラクターの移動ベクトル
	Vector3 move = {0, 0, 0};

	// キャラクターの移動速さ
	const float kCharacterSpeed = 0.2f;

	// 押した方向移動
	if (input_->PushKey(DIK_UP)) {
		move.y += kCharacterSpeed;
	} else if (input_->PushKey(DIK_DOWN)) {
		move.y -= kCharacterSpeed;
	} else if (input_->PushKey(DIK_LEFT)) {
		move.x -= kCharacterSpeed;
	} else if (input_->PushKey(DIK_RIGHT)) {
		move.x += kCharacterSpeed;
	}

	// 旋回
	const float kRotSpeed = 0.02f;

	if (input_->PushKey(DIK_A)) {
		worldTransform_.rotation_.y -= kRotSpeed;
	} else if (input_->PushKey(DIK_D)) {
		worldTransform_.rotation_.y += kRotSpeed;
	}

	// キャラクターの攻撃処理
	Attack();

	//// 弾の更新
	/*if (bullet_) {
		bullet_->Update();
	}*/
	for (PlayerBullet* bullet : bullets_) {
		bullet->Update();
	}

	// 移動制限
	const float kMoveLimitX = 30.0f;
	const float kMoveLimitY = 30.0f;

	// 範囲を超えない処理
	worldTransform_.translation_.x = max(worldTransform_.translation_.x, -kMoveLimitX);
	worldTransform_.translation_.x = min(worldTransform_.translation_.x, +kMoveLimitX);
	worldTransform_.translation_.y = max(worldTransform_.translation_.y, -kMoveLimitY);
	worldTransform_.translation_.y = min(worldTransform_.translation_.y, +kMoveLimitY);

	// 移動
	worldTransform_.translation_.x += move.x;
	worldTransform_.translation_.y += move.y;
	worldTransform_.translation_.z += move.z;
	

	// 行列更新
	worldTransform_.matWorld_ = MakeAffineMatrix(
	    worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);

	 //行列転送
	worldTransform_.TransferMatrix();

	worldTransform_.UpdateMatrix();

	// ImGui
	ImGui::Begin("player");
	float sliderValue[3] = {
	    worldTransform_.translation_.x, worldTransform_.translation_.y,
	    worldTransform_.translation_.z};
	ImGui::SliderFloat3("position", sliderValue, 20.0f, 20.0f);
	worldTransform_.translation_ = {sliderValue[0], sliderValue[1], sliderValue[2]};
	ImGui::End();

	//自機のワールド座標から３Dレティクルのワールド座標を計算
	const float kDistancePlayerTo3DReticle = 50.0f;
	Vector3 offset={0, 0, 1.0f};

	offset = TransformNormal(offset, worldTransform_.matWorld_);
	offset = Normalize(offset) * kDistancePlayerTo3DReticle;

	worldTransform3DReticle_.translation_.x = GetWorldPosition().x + offset.x;
	worldTransform3DReticle_.translation_.y = GetWorldPosition().y + offset.y;
	worldTransform3DReticle_.translation_.z = GetWorldPosition().z + offset.z;
	
	//ワールド行列更新と転送
	worldTransform3DReticle_.UpdateMatrix();

	//３Dから2Dレティクルのスクリーン座標を計算
	Vector3 positionReticle = worldTransform3DReticle_.translation_;

	Matrix4x4 matViewport =
	 MakeViewportMatrix(0, 0, WinApp::kWindowWidth, WinApp::kWindowHeight, 0, 1); 
	Matrix4x4 matViewProjectionViewport =
	    viewProjection.matView * viewProjection.matProjection * matViewport;

	positionReticle = Transform(positionReticle, matViewProjectionViewport);
	sprite2DReticle_->SetPosition(Vector2(positionReticle.x, positionReticle.y));
}


void Player::Draw(ViewProjection& viewProjection) 
{
	model_->Draw(worldTransform_, viewProjection, textureHandle_);

	
	//弾描画
	/*if (bullet_)
	{
		bullet_->Draw(viewProjection);
	}*/
	for (PlayerBullet* bullet : bullets_)
	{
		bullet->Draw(viewProjection);
	}

	model2_->Draw(worldTransform3DReticle_, viewProjection, textureHandle_);
}

void Player::Attack() 
{
	if (input_->PushKey(DIK_SPACE)) 
	{
		//弾の速度
		const float kBullerSpeed = 1.0f;
		Vector3 velocity(0, 0, kBullerSpeed);

		//速度ベクトルを自機の向きに合わせて回転させる
		velocity = TransformNormal(velocity, worldTransform_.matWorld_);

		//弾を生成し、初期化
		PlayerBullet* newBullet = new PlayerBullet;
		newBullet->Initialize(model_, GetWorldPosition() /*worldTransform_.translation_*/, velocity);

		//弾を登録する
		//bullet_ = newBullet;
		bullets_.push_back(newBullet);
		
	}
}
Vector3 Player::GetWorldPosition()
{
	//ワールド座標を入れる変数
	Vector3 worldPos;
	//ワールド行列の平行移動成分を取得
	/*worldPos.x = worldTransform_.translation_.x;
	worldPos.y = worldTransform_.translation_.y;
	worldPos.z = worldTransform_.translation_.z;*/

	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
}

void Player::OnCollision() 
{
	
}

void Player::SetParent(const WorldTransform* parent) 
{
	worldTransform_.parent_ = parent;

}

void Player::DrawUI() 
{ 
	sprite2DReticle_->Draw();
}
