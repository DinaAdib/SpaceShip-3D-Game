#pragma once
#include "ObjectModel.h"

 class Spaceship : public ObjectModel
{
	float Fuel; 
	float Velocity; 
	float Acceleration; 
	float const MaxVelocity = 25; 
	float const MinVelocity = 0;

 public:
	Spaceship();
	~Spaceship();



};