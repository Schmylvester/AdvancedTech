#pragma once
#include "Geometry.h"

class Cube : public Geometry
{
public:
	Cube() = default;
	virtual ~Cube() = default;

	virtual void init() override;
};