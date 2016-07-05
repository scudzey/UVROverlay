#pragma once
#include <Windows.h>
#include <D3D11_1.h>
#include <D3DX11.h>
#include <DXGI.h>
#include "Overlay.h"

#include "OverlayTexture.h"

enum e_axis
{
	X_AXIS = 0,
	Y_AXIS,
	Z_AXIS
};

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

	void setRotate(const int axis, const int value);
	void setTrans(const int axis, const int value);

private:
	HWND m_targetHwnd;
	boost::uuids::uuid m_overlayUID;
	OverlayTexture* m_OverlayTexture;
	vr::VROverlayHandle_t m_ulOverlayHandle;
	vr::HmdMatrix34_t m_overlayDistanceMtx;
	vr::Texture_t m_texture;

	void updateTransform();

	vr::HmdMatrix44_t multMatrix(vr::HmdMatrix44_t m1, vr::HmdMatrix44_t m2);

	//Position Values
	int m_xRotate;
	int m_yRotate;
	int m_zRotate;

	int			 m_xTrans;
	int			 m_yTrans;
	int			 m_zTrans;

};


