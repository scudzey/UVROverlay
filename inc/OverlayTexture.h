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
#include <boost/thread.hpp>
#include <Windows.h>
#include <D3D11_1.h>
#include <D3DX11.h>
#include <DXGI.h>
#include "RenderEnvironment.h"

#define DBOUT( s )            \
{                             \
   std::wostringstream os_;    \
   os_ << s;                   \
   OutputDebugStringW( os_.str().c_str() );  \
}

class OverlayTexture
{
public:
	OverlayTexture(RenderEnvironment* env);
	void GenerateTexture(unsigned int width, unsigned int height);
	bool setTextureFromWindow(HWND targetHWND, int x, int y);
	void updateTexture(ID3D11ShaderResourceView* pDstResource);
	ID3D11Texture2D* getTexture();
	virtual ~OverlayTexture();
private:
	RenderEnvironment* m_d3dEnv;
	ID3D11Texture2D *m_texVal;
	ID3D11Texture2D *m_presentTex;
	ID3D11ShaderResourceView *m_resource;
	ID3D11ShaderResourceView *m_presentResource;
	IDXGISurface1 *m_surface;
	ID3D11RenderTargetView *m_view;
	ID3D11Buffer* m_buffer;
	boost::mutex mtx_;

	unsigned int m_curWidth;
	unsigned int m_curHeight;
	

};

