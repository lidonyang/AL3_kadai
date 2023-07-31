#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>

#include "ImGuiManager.h"
#include"AxisIndicator.h"
#include<fstream>

GameScene::GameScene() {}

GameScene::~GameScene() 
{
	delete player_;
	delete model_;
	delete debugCamera_;
	for (Enemy* enemy : enemy_)
	{
		delete enemy;
		//delete enemy_;
	}
	delete modelSkydome_;
	delete railCamera_;
	// buller_の解放
	for (EnemyBullet* bullet : bullets_) {
	    delete bullet;
	}
	delete winsprite_;
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	// ファイル名を指定してテクスチャを読み込む
	textureHandle_ = TextureManager::Load("sample.png");
	model_ = Model::Create();
	//worldTransform_.Initialize();
	viewProjection_.Initialize();

	// playerの生成
	player_ = new Player();
	player_->Initialize(model_, textureHandle_, {0.f, -5.f, 15.f});

	AddEnemy({0.f, 5.f, 30.f});

	//デバッグカメラの生成
	debugCamera_ = new DebugCamera(WinApp::kWindowWidth, WinApp::kWindowHeight);

	//軸方向表示の表示を有効にする
	AxisIndicator::GetInstance()->SetVisible(true);
	//軸方向表示が参照するビュープロジェクションを指定する（アドレス渡し）
	AxisIndicator::GetInstance()->SetTargetViewProjection(&viewProjection_);

	//// 敵の速度
	//const float kenemySpeed = 0.2f;
	//Vector3 velocity(kenemySpeed, kenemySpeed, kenemySpeed);
	////敵の生成
	//enemy_ = new Enemy();
	//enemy_->Initialize(model_, {15, 2, 50}, velocity);
	////敵キャラにゲームシーンを渡す
	//enemy_->SetGameScene(this);

	////敵キャラに自キャラのアドレス渡す
	//enemy_->SetPlayer(player_);

	//3Dモデルの生成
	modelSkydome_ = Model::CreateFromOBJ("skydome", true);
	skydome_ = new Skydome();
	skydome_->Initialize(modelSkydome_);

	//レールカメラの生成
	railCamera_ = new RailCamera();
	railCamera_->Initialize(viewProjection_.translation_,viewProjection_.rotation_);

	//自キャラとレールカメラの親子関係
	player_->SetParent(&railCamera_->GetWorldProjection());

	//敵発生データを読み込み
	LoadEnemyPopDate();

	TextureManager::Load("target.png");

	wintextureHandle = TextureManager::Load("flag.png");
	winsprite_ = Sprite::Create(wintextureHandle, {40, 30});
	win1sprite_ = Sprite::Create(wintextureHandle, {80, 30});
	win2sprite_ = Sprite::Create(wintextureHandle, {120, 30});

	starttextureHandle = TextureManager::Load("Title.png");
	startsprite_ = Sprite::Create(starttextureHandle, {0, 0});
	gamewintextureHandle = TextureManager::Load("Win.png");
	gamewinsprite_ = Sprite::Create(gamewintextureHandle, {0, 0});
	gameovertextureHandle = TextureManager::Load("Lost.png");
	gameoversprite_ = Sprite::Create(gameovertextureHandle, {0, 0});
}

void GameScene::Update() 
{
	if (scene == 0)
	{
		if (input_->TriggerKey(DIK_RETURN))
		{
			scene = 1;
		}
	}
	else if (scene == 1)
	{
		// プレーヤーの更新
		player_->Update(viewProjection_);

		// デスフラグの立った弾の削除
		bullets_.remove_if([](EnemyBullet* bullet) {
			if (bullet->IsDead()) {
				delete bullet;
				return true;
			}
			return false;
		});
		// 弾の更新
		for (EnemyBullet* bullet : bullets_) {
			bullet->Update();
		}

		railCamera_->Update();

#ifdef _DEBUG
		if (input_->TriggerKey(DIK_BACKSPACE)) {
			// デバッグカメラ有効フラグをトグル
			isDebugCameraActive_ = true;
		}
#endif
		// カメラの処理
		if (isDebugCameraActive_) {
			// デバッグカメラの更新
			debugCamera_->Update();

			viewProjection_.matView = debugCamera_->GetViewProjection().matView;
			viewProjection_.matProjection = debugCamera_->GetViewProjection().matProjection;
			// ビュープロジェクション行列転送
			viewProjection_.TransferMatrix();
		}

		else if (!isDebugCameraActive_) {
			// else
			//{
			//	//ビュープロジェクション行列の更新と転送
			//	viewProjection_.UpdateMatrix();
			//}
			viewProjection_.matView = railCamera_->GetViewProjection().matView;
			viewProjection_.matProjection = railCamera_->GetViewProjection().matProjection;
			viewProjection_.TransferMatrix();
		}

		// デスフラグの立った敵の削除
		enemy_.remove_if([](Enemy* enemy) {
			if (enemy->GetIsDead()) {
				delete enemy;
				return true;
			}
			return false;
		});

		for (Enemy* enemy : enemy_) {
			enemy->Update();
			// 敵の更新
			// enemy_->Update();
		}

		// 当たり判定読み込む
		CheckAllCollisions();

		// モデルの更新
		skydome_->Update();

		// 敵発生コマンド更新
		UpdateEnemyPopCommands();

		if (playerflag_ == 0)
		{
			scene = 3;
		}
		if (enemyflag == 0)
		{
			scene = 2;
		}
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
	//model_->Draw(worldTransform_, viewProjection_, textureHandle_);

	if (scene == 0)
	{

	}
	else if (scene == 1)
	{
		player_->Draw(viewProjection_);

		for (Enemy* enemy : enemy_) {
			enemy->Draw(viewProjection_);
			// enemy_->Draw(viewProjection_);
		}

		skydome_->Draw(viewProjection_);

		// 弾の描画
		for (EnemyBullet* bullet : bullets_) {
			bullet->Draw(viewProjection_);
		}
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
	 
	if (scene == 0)
	{
		 startsprite_->Draw();
	}
	else if (scene == 1)
	{
		 player_->DrawUI();
		 if (playerflag_ == 3) {
			 winsprite_->Draw();
			 win1sprite_->Draw();
			 win2sprite_->Draw();
		 }
		 if (playerflag_ == 2) {
			 winsprite_->Draw();
			 win1sprite_->Draw();
		 }
		 if (playerflag_ == 1) {
			 winsprite_->Draw();
		 }
	}
	else if (scene == 2)
	{
		 gamewinsprite_->Draw();
	}
	else if (scene == 3) {
		 gameoversprite_->Draw();
	}

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}

void GameScene::CheckAllCollisions()
{
	//判定対象AとBの座標
	Vector3 posA, posB;

	//自弾リストの取得
	const std::list<PlayerBullet*>& playerBullets = player_->GetBullets();

	// 敵弾リストの取得
	const std::list<EnemyBullet*>& enemyBullets = bullets_/*enemy_->GetBullets()*/;

	#pragma region 自キャラと敵弾の当たり判定

	// 自キャラの座標
	posA = player_->GetWorldPosition();

	// 自キャラと敵弾全ての当たり判定
	for (EnemyBullet* bullet : enemyBullets) {
		// 敵弾の座標
		posB = bullet->GetWorldPosition();

		// 当たり判定
		float dx = posB.x - posA.x;
		float dy = posB.y - posA.y;
		float dz = posB.z - posA.z;
		float d = dx * dx + dy * dy + dz * dz;
		
		float r = (player_->GetRadius() + bullet->GetRadius()) *  (player_->GetRadius() + bullet->GetRadius());
		// 球と球の交差判定
		if (d <= r) {
			player_->OnCollision();
			playerflag_ -= 1.0f;
			bullet->OnCollision();
		}
	}

#pragma endregion

#pragma region 自弾と敵キャラの当たり判定

	for (Enemy* enemy : enemy_)
	{
		// 敵キャラの座標
		posA = enemy->GetWorldPosition();

		// 自弾と敵全ての当たり判定
		for (PlayerBullet* bullet : playerBullets) {
			// 敵弾の座標
			posB = bullet->GetWorldPosition();

			// 当たり判定
			float dx = posB.x - posA.x;
			float dy = posB.y - posA.y;
			float dz = posB.z - posA.z;
			float d = dx * dx + dy * dy + dz * dz;

			float r = (enemy->GetRadius() + bullet->GetRadius()) *
			          (enemy->GetRadius() + bullet->GetRadius());
			// 球と球の交差判定
			if (d <= r) {
				enemy->OnCollision();
				enemyflag -= 1.0f;
				bullet->OnCollision();
			}
		}
	}
	
#pragma endregion

#pragma region 自弾と敵弾の当たり判定
	// 自弾と敵弾全ての当たり判定
	for (PlayerBullet* bullet : playerBullets) 
	{
		//自弾の座標
		posA = bullet->GetWorldPosition();
		for (EnemyBullet* bullets : enemyBullets)
		{
			// 敵弾の座標
			posB = bullets->GetWorldPosition();
			// 当たり判定
			float dx = posB.x - posA.x;
			float dy = posB.y - posA.y;
			float dz = posB.z - posA.z;
			float d = dx * dx + dy * dy + dz * dz;

			float r = (bullets->GetRadius() + bullet->GetRadius()) *
			          (bullets->GetRadius() + bullet->GetRadius());
			// 球と球の交差判定
			if (d <= r) {
				bullets->OnCollision();

				bullet->OnCollision();
			}
		}

	}

#pragma endregion
}
void GameScene::AddEnemyBullet(EnemyBullet* enemyBullet)
{
	bullets_.push_back(enemyBullet);
}

void GameScene::AddEnemy(Vector3 pos) 
{
	// 敵の速度
	const float kenemySpeed = 0.0f;
	Vector3 velocity(kenemySpeed, kenemySpeed, kenemySpeed);


	// 敵の生成
	Enemy* obj = new Enemy();
	
	
	/*obj->Initialize(model_, {15, 2, 50}, velocity);*/
	 obj->Initialize(model_, pos, velocity);

	// 敵キャラにゲームシーンを渡す
	obj->SetGameScene(this);

	// 敵キャラに自キャラのアドレス渡す
	obj->SetPlayer(player_);

	enemy_.push_back(obj);
}

void GameScene::LoadEnemyPopDate() 
{
	//ファイトを開く
	std::ifstream file;
	file.open("Resources/enemyPop.csv");
	assert(file.is_open());

	enemyPopCommands << file.rdbuf();

	//ファイトを閉じる
	file.close();
}

void GameScene::UpdateEnemyPopCommands() 
{ 
	if (isflag)
	{
		istime--;
		if (istime <= 0)
		{
			isflag = false;
		}
		return;
	}


	std::string line;

	while (getline(enemyPopCommands,line))
	{
		std::istringstream line_stream(line);

		std::string word;

		getline(line_stream, word, ',');

		if (word.find("//") == 0)
		{
			continue;
		}
		if (word.find("POP") == 0)
		{
			getline(line_stream, word, ',');
			float x = (float)std::atof(word.c_str());

			getline(line_stream, word, ',');
			float y = (float)std::atof(word.c_str());

			getline(line_stream, word, ',');
			float z = (float)std::atof(word.c_str());

			AddEnemy(Vector3(x, y, z));
		}
		else if (word.find("WAIT") == 0) {
			getline(line_stream, word, ',');

			int32_t waitTime = atoi(word.c_str());

			isflag = true;
			istime = waitTime;

			break;
		}
	}
}
