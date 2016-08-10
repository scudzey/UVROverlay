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

