/*
**  VROverlay -- OpenVR window overlay management tool
**		Copyright(C) 2016 Joshua New
**
**	This program is free software : you can redistribute it and / or modify
**		it under the terms of the GNU General Public License as published by
**		the Free Software Foundation, either version 3 of the License, or
**		(at your option) any later version.
**
**		This program is distributed in the hope that it will be useful,
**		but WITHOUT ANY WARRANTY; without even the implied warranty of
**		MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the
**		GNU General Public License for more details.
**
**		You should have received a copy of the GNU General Public License
**		along with this program.If not, see <http://www.gnu.org/licenses/>.
*/
#include "VertexObject.h"



VertexObject::VertexObject(RenderEnvironment* env)
	:m_d3dEnv(env)
	,m_indexBuffer(NULL)
	,m_vertexBuffer(NULL)
{
}


VertexObject::~VertexObject()
{
}

void VertexObject::Init()
{
	VertexType* vertices = new VertexType[6];
	unsigned long* indices = new unsigned long[6];
	int i = 0;

	memset(vertices, 0, sizeof(VertexType) * 6);

	for (i = 0; i < 6; i++)
		indices[i] = i;



	D3D11_BUFFER_DESC bufferDesc;
	bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	bufferDesc.ByteWidth = sizeof(VertexType) * 6;
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	bufferDesc.MiscFlags = 0;
	bufferDesc.StructureByteStride = 0;


	D3D11_SUBRESOURCE_DATA InitData;
	InitData.pSysMem = vertices;
	InitData.SysMemPitch = 0;
	InitData.SysMemSlicePitch = 0;

	if (FAILED(m_d3dEnv->getDevice()->CreateBuffer(&bufferDesc, &InitData, &m_vertexBuffer)))
		return;

	D3D11_BUFFER_DESC indexBufferDesc;
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * 6;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA indexInitData;
	indexInitData.pSysMem = indices;
	indexInitData.SysMemPitch = 0;
	indexInitData.SysMemSlicePitch = 0;
	if (FAILED(m_d3dEnv->getDevice()->CreateBuffer(&indexBufferDesc, &indexInitData, &m_indexBuffer)))
		return;


}

void VertexObject::updateBuffer()
{
	float left, right, top, bottom;
	VertexType* vertices;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	VertexType* verticesPtr;
	HRESULT result;


	// If the position we are rendering this bitmap to has not changed then don't update the vertex buffer since it
	// currently has the correct parameters.


	// If it has changed then update the position it is being rendered to.


	// Calculate the screen coordinates of the left side of the bitmap.
	left = (float)((1280 / 2) * -1);

	// Calculate the screen coordinates of the right side of the bitmap.
	right = left + 1280;

	// Calculate the screen coordinates of the top of the bitmap.
	top = (float)(720 / 2 * -1);

	// Calculate the screen coordinates of the bottom of the bitmap.
	bottom = top - 720;

	// Create the vertex array.
	vertices = new VertexType[6];
	if (!vertices)
	{
		return;
	}

	// Load the vertex array with data.
	// First triangle.
	vertices[0].position = D3DXVECTOR3(left, top, 0.0f);  // Top left.
	vertices[0].texture = D3DXVECTOR2(0.0f, 0.0f);

	vertices[1].position = D3DXVECTOR3(right, bottom, 0.0f);  // Bottom right.
	vertices[1].texture = D3DXVECTOR2(1.0f, 1.0f);

	vertices[2].position = D3DXVECTOR3(left, bottom, 0.0f);  // Bottom left.
	vertices[2].texture = D3DXVECTOR2(0.0f, 1.0f);

	// Second triangle.
	vertices[3].position = D3DXVECTOR3(left, top, 0.0f);  // Top left.
	vertices[3].texture = D3DXVECTOR2(0.0f, 0.0f);

	vertices[4].position = D3DXVECTOR3(right, top, 0.0f);  // Top right.
	vertices[4].texture = D3DXVECTOR2(1.0f, 0.0f);

	vertices[5].position = D3DXVECTOR3(right, bottom, 0.0f);  // Bottom right.
	vertices[5].texture = D3DXVECTOR2(1.0f, 1.0f);

	// Lock the vertex buffer so it can be written to.
	result = m_d3dEnv->getContext()->Map(m_vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result))
	{
		return;
	}

	// Get a pointer to the data in the vertex buffer.
	verticesPtr = (VertexType*)mappedResource.pData;

	// Copy the data into the vertex buffer.
	memcpy(verticesPtr, (void*)vertices, (sizeof(VertexType) * 6));

	// Unlock the vertex buffer.
	m_d3dEnv->getContext()->Unmap(m_vertexBuffer, 0);

	// Release the vertex array as it is no longer needed.
	delete[] vertices;
	vertices = 0;

	unsigned int stride;
	unsigned int offset;


	// Set vertex buffer stride and offset.
	stride = sizeof(VertexType);
	offset = 0;

	m_d3dEnv->getContext()->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);

	// Set the index buffer to active in the input assembler so it can be rendered.
	m_d3dEnv->getContext()->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// Set the type of primitive that should be rendered from this vertex buffer, in this case triangles.
	m_d3dEnv->getContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

}
void VertexObject::draw()
{
	//m_d3dEnv->getContext()->Draw(6, 0);
}