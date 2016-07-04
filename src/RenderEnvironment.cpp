#include "RenderEnvironment.h"

RenderEnvironment::RenderEnvironment(int32_t adapterIndex)
{
	HRESULT err;

	REFIID iidVal = __uuidof(IDXGIFactory1);

	if (FAILED(err = CreateDXGIFactory1(iidVal, (void**)&m_factory)))
		return;
	
	IDXGIAdapter1 *adapter;
	if (FAILED(err = m_factory->EnumAdapters1(0, &adapter)))
		return;

	DXGI_SWAP_CHAIN_DESC swapDesc;
	memset(&swapDesc, 0, sizeof(swapDesc));
	swapDesc.BufferCount = 2;
	swapDesc.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	swapDesc.BufferDesc.Width = 1280;
	swapDesc.BufferDesc.Height = 720;
	swapDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapDesc.OutputWindow = hWnd;
	swapDesc.SampleDesc.Count = 1;
	swapDesc.Windowed = TRUE;
	swapDesc.Flags = DXGI_SWAP_CHAIN_FLAG_GDI_COMPATIBLE;

	m_bDisableCompatibilityMode = 1;

#ifdef _DEBUG
	UINT createFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT | D3D11_CREATE_DEVICE_DEBUG;
#else
	UINT createFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;
#endif

	WCHAR *adapterName;
	DXGI_ADAPTER_DESC desc;
	
	err = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, createFlags, NULL, 0, D3D11_SDK_VERSION, &swapDesc, &m_swap, &m_d3d, NULL, &m_d3d_ctx);
	if (FAILED(err))
	{
		m_bDisableCompatibilityMode = !m_bDisableCompatibilityMode;
		err = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, createFlags, NULL, 0, D3D11_SDK_VERSION, &swapDesc, &m_swap, &m_d3d, NULL, &m_d3d_ctx);
	}

	if (FAILED(err))
	{
		return;
	}

	adapter->Release();

	//------------------------------------------------------------------

	D3D11_DEPTH_STENCIL_DESC depthDesc;
	memset(&depthDesc, 0, sizeof(depthDesc));
	depthDesc.DepthEnable = FALSE;

	err = m_d3d->CreateDepthStencilState(&depthDesc, &m_depthState);
	if (FAILED(err))
		return;

	m_d3d_ctx->OMSetDepthStencilState(m_depthState, 0);

	//------------------------------------------------------------------

	D3D11_RASTERIZER_DESC rasterizerDesc;
	memset(&rasterizerDesc, 0, sizeof(rasterizerDesc));
	rasterizerDesc.FillMode = D3D11_FILL_SOLID;
	rasterizerDesc.CullMode = D3D11_CULL_NONE;
	rasterizerDesc.FrontCounterClockwise = FALSE;
	rasterizerDesc.DepthClipEnable = TRUE;

	err = m_d3d->CreateRasterizerState(&rasterizerDesc, &m_rasterizerState);
	if (FAILED(err))
		return;

	m_d3d_ctx->RSSetState(m_rasterizerState);

	//------------------------------------------------------------------

	rasterizerDesc.ScissorEnable = TRUE;

	err = m_d3d->CreateRasterizerState(&rasterizerDesc, &m_scissorState);
	if (FAILED(err))
		return;

	//------------------------------------------------------------------

	ID3D11Texture2D *backBuffer = NULL;
	err = m_swap->GetBuffer(0, IID_ID3D11Texture2D, (void**)&backBuffer);
	if (FAILED(err))
		return;

	err = m_d3d->CreateRenderTargetView(backBuffer, NULL, &m_swapRenderView);
	if (FAILED(err))
		return;

	backBuffer->Release();

	//------------------------------------------------------------------

	D3D11_BLEND_DESC disabledBlendDesc;
	memset(&disabledBlendDesc, 0, sizeof(disabledBlendDesc));
	disabledBlendDesc.AlphaToCoverageEnable = TRUE;
	disabledBlendDesc.IndependentBlendEnable = TRUE;
	for (int i = 0; i<8; i++)
	{
		disabledBlendDesc.RenderTarget[i].BlendEnable = TRUE;
		disabledBlendDesc.RenderTarget[i].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
		disabledBlendDesc.RenderTarget[i].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		disabledBlendDesc.RenderTarget[i].BlendOp = D3D11_BLEND_OP_ADD;
		disabledBlendDesc.RenderTarget[i].SrcBlendAlpha = D3D11_BLEND_ONE;
		disabledBlendDesc.RenderTarget[i].DestBlendAlpha = D3D11_BLEND_ZERO;
		disabledBlendDesc.RenderTarget[i].SrcBlend = D3D11_BLEND_ONE;
		disabledBlendDesc.RenderTarget[i].DestBlend = D3D11_BLEND_ZERO;
	}
	

	err = m_d3d->CreateBlendState(&disabledBlendDesc, &m_disabledBlend);
	if (FAILED(err))
		return;
	m_bBlendingEnabled = true;


	ID3D11Texture2D *pBackBuffer;
	m_swap->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);

	// use the back buffer address to create the render target
	m_d3d->CreateRenderTargetView(pBackBuffer, NULL, &m_swapRenderView);
	pBackBuffer->Release();

	// set the render target as the back buffer
	m_d3d_ctx->OMSetRenderTargets(1, &m_swapRenderView, NULL);
	

	

}

RenderEnvironment::~RenderEnvironment()
{

}

ID3D11Device* RenderEnvironment::getDevice()
{
	return m_d3d;
}

ID3D11DeviceContext* RenderEnvironment::getContext()
{
	return m_d3d_ctx;
}

void RenderEnvironment::setRenderTgt(ID3D11RenderTargetView* tgt)
{
	m_renderTgt = tgt;

}

IDXGISwapChain* RenderEnvironment::getSwap()
{
	return m_swap;
}

void RenderEnvironment::RenderFrame()
{
	float clearColor[4] = {0.0f, 0.0f, 0.0f, 1.0f};
	
	m_d3d_ctx->ClearRenderTargetView(m_swapRenderView, clearColor);
		
	//RenderItems
	m_swap->Present(0, 0);
}
