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

