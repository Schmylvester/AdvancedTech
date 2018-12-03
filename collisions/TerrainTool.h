#pragma once
class TerrainTool
{
public:
	TerrainTool();
	~TerrainTool();

	void readFile();
	void splitFile(int width, int height);
	void writeHeightFile();
	void createNormalFile();
};