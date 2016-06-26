#pragma once
#include "Overlay.h"
#include <Windows.h>
class WindowOverlay : public virtual Overlay
{
public:
	WindowOverlay();
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


};

