#include "StaticOverlay.h"


StaticOverlay::StaticOverlay()
	:m_texturePath("")
	, m_overlayUID(boost::uuids::random_generator()())
{
}


StaticOverlay::StaticOverlay(const std::string& path)
    :m_texturePath(path)
	,m_overlayUID(boost::uuids::random_generator()())
{
}


StaticOverlay::~StaticOverlay()
{
}

bool StaticOverlay::ShowOverlay()
{
	bool bSuccess = false;
	if (m_texturePath == "")
	{
		return false;
	}

	if (vr::VRCompositor() && vr::VROverlay())
	{
		//Hide overlay to destroy old overlay instance
		HideOverlay();

		//Use class UUID for overlay key
		const std::string overlayKey = to_string(m_overlayUID);
		vr::VROverlay()->CreateOverlay(overlayKey.c_str(), "Static Overlay", &m_ulOverlayHandle);


		vr::VROverlayError overlayError = vr::VROverlayError_None;

		//Generate relative tracking to HMD
		overlayError = vr::VROverlay()->SetOverlayTransformTrackedDeviceRelative(m_ulOverlayHandle, vr::k_unTrackedDeviceIndex_Hmd, &m_overlayDistanceMtx);
		if (overlayError != vr::VROverlayError_None)
			return false;

		//Set Overlay texture from file
		vr::VROverlay()->SetOverlayFromFile(m_ulOverlayHandle, m_texturePath.c_str());

		//Show the overlay
		overlayError = vr::VROverlay()->ShowOverlay(m_ulOverlayHandle);
		bSuccess = overlayError == vr::VROverlayError_None;
	}

	return bSuccess;
}

void StaticOverlay::HideOverlay()
{
	if (m_ulOverlayHandle == vr::k_ulOverlayHandleInvalid)
		return;

	vr::VRCompositor();  // Required to call overlays...
	vr::VROverlay()->HideOverlay(m_ulOverlayHandle);
	vr::VROverlay()->DestroyOverlay(m_ulOverlayHandle);
	m_ulOverlayHandle = vr::k_ulOverlayHandleInvalid;
}

void StaticOverlay::handleEvent(const vr::VREvent_t& event)
{
}

void StaticOverlay::updateTexture()
{
	vr::VROverlay()->SetOverlayFromFile(m_ulOverlayHandle, m_texturePath.c_str());
}

void StaticOverlay::setTexturePath(const std::string& path)
{
	m_texturePath = path;
	updateTexture();
}

std::string StaticOverlay::getTexturePath() const
{
	return m_texturePath;
}

void StaticOverlay::setOverlayMatrix(const vr::HmdMatrix34_t& relativePosition)
{
	m_overlayDistanceMtx = relativePosition;
}

boost::uuids::uuid StaticOverlay::getOverlayUuid() const
{
	return m_overlayUID;
}
