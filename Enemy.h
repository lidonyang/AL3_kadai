#pragma once
#include "Model.h"
#include "Vector3.h"
#include "ViewProjection.h"
#include "WorldTransform.h"



class Enemy {
public:
	void Initialize(Model* model, const Vector3& position, const Vector3& velocity);

	void Update();

	void Draw(const ViewProjection& viewProjection);

	void UpdateApproach();// 接近する関数

	void UpdateLeave();// 離脱する関数
	//移動フェ－ズ
	enum class Phase {
		Approach, // 接近する
		Leave,    // 離脱する
	};

private:
	WorldTransform worldTransform_;
	Model* model_;
	uint32_t textureHandle_;
	// 速度
	Vector3 velocity_;

	//フェ－ズ
	Phase phase_ = Phase::Approach;
};
