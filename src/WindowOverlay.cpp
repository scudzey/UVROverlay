#include "WindowOverlay.h"
#include <math.h>


WindowOverlay::WindowOverlay(OverlayTexture* d3dTexture)
	:m_OverlayTexture(d3dTexture)
	, m_targetHwnd(NULL)
	, m_overlayUID(boost::uuids::random_generator()())
	, m_updateThread(NULL)
	, m_timer(m_io, boost::posix_time::millisec(17))
{
	memset(&m_overlayDistanceMtx, 0, sizeof(m_overlayDistanceMtx));
	m_timer.async_wait(boost::bind(&WindowOverlay::asyncUpdate, this));
}


WindowOverlay::~WindowOverlay()
{
}

void WindowOverlay::setupThread()
{
	m_io.run();
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
		m_updateThread = new boost::thread(&WindowOverlay::setupThread, this);
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
	m_io.stop();

	//Stop Thread and cleanup
	m_updateThread->join();
	delete m_updateThread;
	m_updateThread = NULL;
}

void WindowOverlay::handleEvent(const vr::VREvent_t & event)
{
}

void WindowOverlay::asyncUpdate()
{
	updateTexture();
	m_timer.expires_from_now(boost::posix_time::milliseconds(17));
	m_timer.async_wait(boost::bind(&WindowOverlay::asyncUpdate, this));
}

void WindowOverlay::updateTexture()
{
	//ShowOverlay();
	boost::lock_guard<boost::mutex> guard(mtx_);
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
	//shaderResource->Release();
	shaderResource = NULL;


	vr::VROverlay()->SetOverlayTransformTrackedDeviceRelative(m_ulOverlayHandle, vr::k_unTrackedDeviceIndex_Hmd, &m_overlayDistanceMtx);
	//vr::VROverlay()->ShowOverlay(m_ulOverlayHandle);

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

void WindowOverlay::setRotate(const int axis, const int value)
{
	switch (axis)
	{
	case X_AXIS:
		m_xRotate = value;
		break;
	case Y_AXIS:
		m_yRotate = value;
		break;
	case Z_AXIS:
		m_zRotate = value;
	}

	updateTransform();
}

void WindowOverlay::setTrans(const int axis, const int value)
{
	switch (axis)
	{
	case X_AXIS:
		m_xTrans = value;
		break;
	case Y_AXIS:
		m_yTrans = value;
		break;
	case Z_AXIS:
		m_zTrans = value;
	}
	updateTransform();
}

int WindowOverlay::getRotate(const int axis)
{
	switch (axis)
	{
	case X_AXIS:
		return m_xRotate;
	case Y_AXIS:
		return m_yRotate;	
	case Z_AXIS:
		return m_zRotate;
	}

	return 0;
}

int WindowOverlay::getTrans(const int axis)
{
	switch (axis)
	{
	case X_AXIS:
		return m_xTrans;
	case Y_AXIS:
		return m_yTrans;
		break;
	case Z_AXIS:
		return m_zTrans;
	}
	return 0;
}

void WindowOverlay::updateTransform()
{
	vr::HmdMatrix44_t Translation, xRotate, yRotate, zRotate, scale;
	vr::HmdMatrix44_t result;
	memset(&scale, 0, sizeof(scale));
	memset(&Translation, 0, sizeof(Translation));
	memset(&xRotate, 0, sizeof(xRotate));
	memset(&yRotate, 0, sizeof(yRotate));
	memset(&zRotate, 0, sizeof(zRotate));

	//Setup Scale (Currently fixed)
	scale.m[0][0] = 1.0f;
	scale.m[1][1] = 1.0f;
	scale.m[2][2] = 1.0f;
	scale.m[2][2] = 1.0f;


	//Setup Translation
	Translation.m[0][0] = 1.0f;
	Translation.m[1][1] = 1.0f;
	Translation.m[2][2] = 1.0f;
	Translation.m[2][2] = 1.0f;
	Translation.m[0][3] = (float)(m_xTrans) / 25.0f;
	Translation.m[1][3] = (float)(m_yTrans) / 25.0f;
	Translation.m[2][3] = (float)(m_zTrans) / 25.0f;
	Translation.m[3][3] = 1;

	//Setup xRotate matrix
	xRotate.m[0][0] = 1;
	xRotate.m[1][1] = std::cos(((float)m_xRotate)*(3.14159f / 180.0f));
	xRotate.m[1][2] = std::sin(((float)m_xRotate)*(3.14159f / 180.0f)) * -1.0f;
	xRotate.m[2][1] = std::sin(((float)m_xRotate)*(3.14159f / 180.0f));
	xRotate.m[2][2] = std::cos(((float)m_xRotate)*(3.14159f / 180.0f));
	xRotate.m[3][3] = 1;

	//Setup yRotate matrix
	yRotate.m[0][0] = std::cos(((float)m_yRotate)*(3.14159f / 180.0f));
	yRotate.m[0][2] = std::sin(((float)m_yRotate)*(3.14159f / 180.0f));
	yRotate.m[1][1] = 1;
	yRotate.m[2][0] = std::sin(((float)m_yRotate)*(3.14159f / 180.0f)) * -1.0f;
	yRotate.m[2][2] = std::cos(((float)m_yRotate)*(3.14159f / 180.0f));
	yRotate.m[3][3] = 1;

	//Setup zRotate matrix
	zRotate.m[0][0] = std::cos(((float)m_zRotate)*(3.14159f / 180.0f));
	zRotate.m[0][1] = std::sin(((float)m_zRotate)*(3.14159f / 180.0f)) * -1.0f;
	zRotate.m[1][0] = std::sin(((float)m_zRotate)*(3.14159f / 180.0f));
	zRotate.m[1][1] = std::cos(((float)m_zRotate)*(3.14159f / 180.0f));
	zRotate.m[2][2] = 1;
	zRotate.m[3][3] = 1;

	//Matrix product all of them
	result = multMatrix(scale, Translation);
	result = multMatrix(result, xRotate);
	result = multMatrix(result, yRotate);
	result = multMatrix(result, zRotate);

	for (int i= 0; i < 3; i++)
		for (int j = 0; j < 4; j++)
		{
			m_overlayDistanceMtx.m[i][j] = result.m[i][j];
		}
}

vr::HmdMatrix44_t WindowOverlay::multMatrix(vr::HmdMatrix44_t m1, vr::HmdMatrix44_t m2)
{
	vr::HmdMatrix44_t result;
	memset(&result, 0, sizeof(result));

	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			for (int k = 0; k < 4; k++)
				result.m[i][j] += m1.m[i][k] * m2.m[k][j];

	return result;
}

void WindowOverlay::setName(const std::wstring& name)
{
	m_wndName = name;
}

void WindowOverlay::setExeName(const std::wstring& name)
{
	m_exeName = name;
}

std::wstring WindowOverlay::getName()
{
	return std::wstring(m_wndName);
}

std::wstring WindowOverlay::getExeName()
{
	return std::wstring(m_exeName);
}


