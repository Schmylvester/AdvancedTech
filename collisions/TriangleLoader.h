#pragma once

class ID3D10Blob;
class TriangleLoader
{
public:
	TriangleLoader();
	~TriangleLoader();

	ID3D10Blob *VS, *PS;
};

