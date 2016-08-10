/*
**		VROverlay -- OpenVR window overlay management tool
**		Copyright(C) 2016 Joshua New
**
**		This program is free software : you can redistribute it and / or modify
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
#pragma comment(lib, "windowscodecs.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "d3d11.lib")

#include <boost/thread.hpp>
#include <Windows.h>
#include <D3D11_1.h>
#include <D3DX11.h>
#include <DXGI.h>
#include <D3dx9math.h>
#include <openvr.h>

struct VertexType
{
	D3DXVECTOR3 position;
	D3DXVECTOR2 texture;
};

class RenderEnvironment
{
public:
	RenderEnvironment(int32_t adapterIndex, HWND hWnd);
	virtual ~RenderEnvironment();
	ID3D11Device* getDevice();
	ID3D11DeviceContext * getContext();
	void setRenderTgt(ID3D11RenderTargetView * tgt);
	void RenderFrame();
	IDXGISwapChain* getSwap();

	void lockD3D();
	void unlockD3d();
	//void updateTexture(ID3D11Texture2D * tex);

private:
	IDXGIFactory1           *m_factory;
	ID3D11Device			*m_d3d;
	ID3D11DeviceContext    *m_d3d_ctx;
	IDXGISwapChain          *m_swap;
	ID3D11RenderTargetView  *m_swapRenderView;

	ID3D11DepthStencilState *m_depthState;
	ID3D11RasterizerState   *m_rasterizerState;
	ID3D11RasterizerState   *m_scissorState;

	ID3D11BlendState        *m_curBlendState;
	ID3D11BlendState        *m_disabledBlend;
	BOOL                    m_bBlendingEnabled;

	boost::mutex			m_mtx;

	

	ID3D11RenderTargetView  *m_renderTgt;

	bool m_bDisableCompatibilityMode;
};


