#pragma once
#include <string>
#include <iostream>


#include "Overlay.h"
class StaticOverlay :
	public virtual Overlay
{
public:
	StaticOverlay();
	StaticOverlay(const std::string& path);
	virtual ~StaticOverlay();
	virtual bool ShowOverlay();
	virtual void HideOverlay();
	virtual void handleEvent(const vr::VREvent_t& event);
	virtual void updateTexture();
	virtual boost::uuids::uuid getOverlayUuid() const;

	virtual void setTexturePath(const std::string& path);
	virtual std::string getTexturePath() const;

	virtual void setOverlayMatrix(const vr::HmdMatrix34_t& relativePosition);

private:
	std::string m_texturePath;
	boost::uuids::uuid m_overlayUID;
	vr::VROverlayHandle_t m_ulOverlayHandle;
	vr::HmdMatrix34_t m_overlayDistanceMtx;
};

