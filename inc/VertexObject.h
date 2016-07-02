#pragma once
#include "RenderEnvironment.h"

class VertexObject
{
public:
	VertexObject(RenderEnvironment* env);
	virtual ~VertexObject();

	void Init();
	void updateBuffer();

	void draw();

private:
	RenderEnvironment		*m_d3dEnv;
	ID3D11Buffer			*m_vertexBuffer;
	ID3D11Buffer			*m_indexBuffer;
};

