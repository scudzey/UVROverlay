#include "WindowOverlay.h"



WindowOverlay::WindowOverlay(OverlayTexture* d3dTexture)
	:m_OverlayTexture(d3dTexture)
	, m_targetHwnd(NULL)
	, m_overlayUID(boost::uuids::random_generator()())
{

}


WindowOverlay::~WindowOverlay()
{
}

bool WindowOverlay::ShowOverlay()
{
	bool bSuccess = false;
	if (m_targetHwnd == NULL)
	{
		return false;
	}

	if (vr::VRCompositor() && vr::VROverlay())
	{
		//Hide overlay to destroy old overlay instance
		HideOverlay();

		//Setup DirectX texture
		RECT rect;
		if (!GetWindowRect(m_targetHwnd, &rect))
		{
			return false;
		}
		int x = rect.right - rect.left;
		int y = rect.bottom - rect.top;
		m_OverlayTexture->GenerateTexture(x, y);
		m_OverlayTexture->setTextureFromWindow(m_targetHwnd, x, y);


		//Use class UUID for overlay key
		const std::string overlayKey = to_string(m_overlayUID);
		vr::VROverlay()->CreateOverlay(overlayKey.c_str(), "Window Overlay", &m_ulOverlayHandle);


		vr::VROverlayError overlayError = vr::VROverlayError_None;

		//Generate relative tracking to HMD
		overlayError = vr::VROverlay()->SetOverlayTransformTrackedDeviceRelative(m_ulOverlayHandle, vr::k_unTrackedDeviceIndex_Hmd, &m_overlayDistanceMtx);
		if (overlayError != vr::VROverlayError_None)
			return false;

		//Set Overlay texture from file
		vr::Texture_t m_texture = { (void *)m_OverlayTexture->getTexture(), vr::API_DirectX ,  vr::ColorSpace_Gamma };
		vr::VROverlay()->SetOverlayTexture(m_ulOverlayHandle, &m_texture);

		//Show the overlay
		overlayError = vr::VROverlay()->ShowOverlay(m_ulOverlayHandle);
		bSuccess = overlayError == vr::VROverlayError_None;
	}
	return bSuccess;
}

void WindowOverlay::HideOverlay()
{
	if (m_ulOverlayHandle == vr::k_ulOverlayHandleInvalid)
		return;

	vr::VRCompositor();  // Required to call overlays...
	vr::VROverlay()->HideOverlay(m_ulOverlayHandle);
	vr::VROverlay()->DestroyOverlay(m_ulOverlayHandle);
	m_ulOverlayHandle = vr::k_ulOverlayHandleInvalid;
}

void WindowOverlay::handleEvent(const vr::VREvent_t & event)
{
}

void WindowOverlay::updateTexture()
{
	//ShowOverlay();
	vr::VRCompositor();
	vr::VROverlay();
	RECT rect;
	if (!GetWindowRect(m_targetHwnd, &rect))
	{
		return;
	}
	int x = rect.right - rect.left;
	int y = rect.bottom - rect.top;
	m_OverlayTexture->setTextureFromWindow(m_targetHwnd, x, y);
	

	ID3D11ShaderResourceView* shaderResource;
	uint32_t pwidth;
	uint32_t pHeight;
	uint32_t pNativeFormat;
	vr::EGraphicsAPIConvention pAPI;
	vr::EColorSpace pColorSpace;

	//Get reference to OpenVR ID3D11Texture2D
	vr::VROverlay()->GetOverlayTexture(m_ulOverlayHandle, (void **)&shaderResource, m_OverlayTexture->getTexture(), &pwidth, &pHeight, &pNativeFormat, &pAPI, &pColorSpace);
	
	//Copy Texture data from OverlayTexture to the openVR Texture reference
	m_OverlayTexture->updateTexture(shaderResource);

	//Release OpenVR texture reference
	vr::VROverlay()->ReleaseNativeOverlayHandle(m_ulOverlayHandle, (void*)shaderResource);

	vr::VROverlay()->ShowOverlay(m_ulOverlayHandle);

}

void WindowOverlay::setOverlayMatrix(const vr::HmdMatrix34_t & relativePosition)
{
	m_overlayDistanceMtx = relativePosition;
}

boost::uuids::uuid WindowOverlay::getOverlayUuid() const
{
	return boost::uuids::uuid();
}

void WindowOverlay::setHwnd(HWND hWnd)
{
	m_targetHwnd = hWnd;
}


