#pragma once
#include "Overlay.h"
#include <D3D11_1.h>
#include <D3DX11.h>
#include <DXGI.h>
#include <Windows.h>
#include "OverlayTexture.h"

class WindowOverlay : public virtual Overlay
{
public:
	WindowOverlay(OverlayTexture* d3dTexture);
	virtual ~WindowOverlay();
	virtual bool ShowOverlay();
	virtual void HideOverlay();
	virtual void handleEvent(const vr::VREvent_t& event);
	virtual void updateTexture();
	virtual void setOverlayMatrix(const vr::HmdMatrix34_t& relativePosition);
	virtual boost::uuids::uuid getOverlayUuid() const;
	virtual void setHwnd(HWND);

private:
	HWND m_targetHwnd;
	boost::uuids::uuid m_overlayUID;
	OverlayTexture* m_OverlayTexture;
	vr::VROverlayHandle_t m_ulOverlayHandle;
	vr::HmdMatrix34_t m_overlayDistanceMtx;
	vr::Texture_t m_texture;

};


