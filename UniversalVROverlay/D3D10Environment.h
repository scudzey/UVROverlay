#pragma once
#pragma comment(lib, "windowscodecs.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "D3D10_1.lib");

#include <D3D10_1.h>
#include <D3DX10.h>
#include <DXGI.h>
class D3D10Environment
{
public:
	D3D10Environment();
	virtual ~D3D10Environment();
private:
	IDXGIFactory1           *m_factory;
	ID3D10Device1           *m_d3d;
	IDXGISwapChain          *m_swap;
	ID3D10RenderTargetView  *m_swapRenderView;

	ID3D10DepthStencilState *m_depthState;
	ID3D10RasterizerState   *m_rasterizerState;
	ID3D10RasterizerState   *m_scissorState;

	ID3D10BlendState        *m_curBlendState;
	ID3D10BlendState        *m_disabledBlend;
	BOOL                    m_bBlendingEnabled;

	bool m_bDisableCompatibilityMode;
};

