#include "OverlayTexture.h"
#include "..\inc\OverlayTexture.h"



OverlayTexture::OverlayTexture(RenderEnvironment* d3dEnvObj)
	:m_d3dEnv(d3dEnvObj)
{
	
}

void OverlayTexture::GenerateTexture(unsigned int width, unsigned int height)
{
	//TODO: Refactor this into a helper class
	HRESULT err;

	//Construct the texture description
	D3D11_TEXTURE2D_DESC td;
	memset(&td, 0, sizeof(td));
	td.Width = width;
	td.Height = height;
	td.MipLevels = 1;
	td.ArraySize = 1;
	td.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	td.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET ;
	td.SampleDesc.Count = 1;
	td.Usage = D3D11_USAGE_DEFAULT;
	td.MiscFlags = D3D11_RESOURCE_MISC_GDI_COMPATIBLE;

	UINT32 *pixels = new UINT32[width * height];
	for (int y = 0; y < width * height; ++y)
	{
		pixels[y] = 0xffffffff;
	}
	D3D11_SUBRESOURCE_DATA srd;
	memset(&srd, 0, sizeof(srd));
	srd.pSysMem = (void *)pixels;
	srd.SysMemPitch = width * 4;
	srd.SysMemSlicePitch = 0;

	if (FAILED(err = m_d3dEnv->getDevice()->CreateTexture2D(&td, &srd, &m_texVal)))
	{
		return;
	}

	//------------------------------------------
	
	D3D11_SHADER_RESOURCE_VIEW_DESC resourceDesc;
	memset(&resourceDesc, 0, sizeof(resourceDesc));
	resourceDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	resourceDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	resourceDesc.Texture2D.MipLevels = 1;
#ifdef _DEBUG
	m_texVal->SetPrivateData(WKPDID_D3DDebugObjectName, sizeof("displayTexture") - 1, "displayTexture");
#endif
	
	if (FAILED(err = m_d3dEnv->getDevice()->CreateShaderResourceView(m_texVal, NULL, &m_resource)))
	{
		return;
	}
}

bool OverlayTexture::setTextureFromWindow(HWND targetHWND, int x, int y)
{
	HDC textureDC;
	m_d3dEnv->lockD3D();

	HDC targetDC = GetDC(targetHWND);
	if(FAILED(m_texVal->QueryInterface(__uuidof(IDXGISurface1), (void **)&m_surface)))
		return false;
	
	if (FAILED(m_surface->GetDC(TRUE, &textureDC)))
		return false;

	BitBlt(textureDC, 0, 0, x, y, targetDC, 0, 0, SRCCOPY);
	m_surface->ReleaseDC(NULL);
	m_surface->Release();
	ReleaseDC(targetHWND, targetDC);

	m_d3dEnv->getContext()->PSSetShaderResources(0, 1, &m_resource);
	m_d3dEnv->unlockD3d();
	return true;
}

//Copies texture to Shader Resource
void OverlayTexture::updateTexture(ID3D11ShaderResourceView* pDstResource)
{
	m_d3dEnv->lockD3D();
	ID3D11Resource * dstTexture;
	pDstResource->GetResource(&dstTexture);
	m_d3dEnv->getContext()->CopyResource(dstTexture, m_texVal);
	dstTexture->Release();
	dstTexture = NULL;
	
	m_d3dEnv->unlockD3d();
}

ID3D11Texture2D* OverlayTexture::getTexture()
{
	return m_texVal;
}


OverlayTexture::~OverlayTexture()
{
}
