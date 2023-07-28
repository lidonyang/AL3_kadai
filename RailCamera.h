#pragma once
#include "WorldTransform.h"
#include "ViewProjection.h"

class RailCamera 
{
public:
	void Initialize(Vector3 trans,Vector3 rot);
	void Update();

	ViewProjection& GetViewProjection() { return viewProjection_; };
	WorldTransform& GetWorldProjection() { return worldTransform_; };


private:
	WorldTransform worldTransform_;
	ViewProjection viewProjection_;

};
