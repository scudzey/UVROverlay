#include "WindowOverlay.h"



WindowOverlay::WindowOverlay()
{
}


WindowOverlay::~WindowOverlay()
{
}

bool WindowOverlay::ShowOverlay()
{
	return false;
}

void WindowOverlay::HideOverlay()
{
}

void WindowOverlay::handleEvent(const vr::VREvent_t & event)
{
}

void WindowOverlay::updateTexture()
{
}

void WindowOverlay::setOverlayMatrix(const vr::HmdMatrix34_t & relativePosition)
{
}

boost::uuids::uuid WindowOverlay::getOverlayUuid() const
{
	return boost::uuids::uuid();
}

void WindowOverlay::setHwnd(HWND)
{
}
