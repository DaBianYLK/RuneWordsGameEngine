#pragma once

#include <vector>

using namespace std;

struct MaxVertex {
	float x;
	float y;
	float z;

	float nX;
	float nY;
	float nZ;

	float u;
	float v;

	int boneID[2];
	float blend[2];
};

struct MaxMeshHead {
	char name[256];
	unsigned int vertexNum;
	unsigned int triangleNum;
};

struct MaxMesh {
	MaxMeshHead head;
	vector<MaxVertex> vertices;
};

struct MaxModelHead {
	unsigned int boneNum;
	int startTime;
	int endTime;
	int frameInterval;
	int frameNum;

	unsigned int meshNum;
};

struct MaxAnimation {
	char name[256];
	unsigned int startFrame;
	unsigned int frameNum;
};