#pragma once
#include <Vector3.h>
/// <summary>
/// 4x4行列
/// </summary>
struct Matrix4x4 final {
	float m[4][4];
};

//スケ－リング
Matrix4x4 MakeScaleMatrix(const Vector3& scale);

// 回転行列
Matrix4x4 MakeRotationXMatrix(float theta);
Matrix4x4 MakeRotationYMatrix(float theta);
Matrix4x4 MakeRotationZMatrix(float theta);

//平行移動行列
Matrix4x4 MakeTranslateMatrix(const Vector3& translate);

//合成オーバーロード 
Matrix4x4& operator *=( Matrix4x4& m1, const Matrix4x4& m2);

//2公演算子オーバーロード
Matrix4x4 operator*(const Matrix4x4& m1, const Matrix4x4& m2);

//アフィン変換行列
Matrix4x4 MakeAffineMatrix(const Vector3& scale, const Vector3& rot, const Vector3& translate);
	
//ベクトル変換
Vector3 TransformNormal(const Vector3& v, const Matrix4x4& m);

// 逆行列
Matrix4x4 Inverse(const Matrix4x4& m);