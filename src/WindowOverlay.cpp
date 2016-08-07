#include "WindowOverlay.h"
#include <math.h>



//overlay helper functions

vr::HmdMatrix44_t multMatrix(vr::HmdMatrix44_t m1, vr::HmdMatrix44_t m2)
{
	vr::HmdMatrix44_t result;
	memset(&result, 0, sizeof(result));

	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			for (int k = 0; k < 4; k++)
				result.m[i][j] += m1.m[i][k] * m2.m[k][j];

	return result;
}


//class functions


WindowOverlay::WindowOverlay(OverlayTexture* d3dTexture, vr::IVRSystem* vrsys)
	:m_OverlayTexture(d3dTexture)
	, m_targetHwnd(NULL)
	, m_overlayUID(boost::uuids::random_generator()())
	, m_updateThread(NULL)
	, m_io(boost::in_place())
	, m_rate(1000 / 60)
	, m_transparancy(1.0f)
	, m_timer( boost::in_place(boost::ref(m_io.get()), boost::posix_time::millisec(m_rate)))
	, m_TrackedDevice(1) //Set to track to head by default
	, m_vrSys(vrsys)
	, m_scale(100)
	, m_isVisible(false)
	, m_xTrans(0)
	, m_yTrans(0)
	, m_zTrans(-9)
	, m_xRotate(0)
	, m_yRotate(0)
	, m_zRotate(0)
{
	memset(&m_overlayDistanceMtx, 0, sizeof(m_overlayDistanceMtx));
	
	m_overlayDistanceMtx.m[2][3] = (-9.0f)/25.0f;

	m_timer->async_wait(boost::bind(&WindowOverlay::asyncUpdate, this));
}


WindowOverlay::~WindowOverlay()
{
	boost::lock_guard<boost::mutex> guard(mtx_);
	//hide the overlay, destroying it
	HideOverlay();
	m_io->stop();
	m_io->reset();
	//Clean up thread;
	if (m_updateThread)
	{
		
		m_updateThread->join();
		delete m_updateThread;
		m_updateThread = NULL;
	}
	//m_io = boost::none;

	//Clean up texture;
	if (m_OverlayTexture)
	{
		delete m_OverlayTexture;
		m_OverlayTexture = NULL;
	}
	m_timer = boost::none;
	m_io = boost::none;
	


}

void WindowOverlay::setupThread()
{
	m_io->run();
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
		if (!GetClientRect(m_targetHwnd, &rect))
		{
			return false;
		}
		unsigned int x = rect.right - rect.left;
		unsigned int y = rect.bottom - rect.top;
		m_texWidth = x;
		m_texHeight = y;
		m_OverlayTexture->GenerateTexture(x, y);
		m_OverlayTexture->setTextureFromWindow(m_targetHwnd, x, y);


		//Use class UUID for overlay key
		const std::string overlayKey = to_string(m_overlayUID);
		vr::VROverlay()->CreateOverlay(overlayKey.c_str(), "Window Overlay", &m_ulOverlayHandle);


		vr::VROverlayError overlayError = vr::VROverlayError_None;

		//Generate relative tracking to HMD
		setTracking(m_TrackedDevice);
		setOverlayTracking();
		if (overlayError != vr::VROverlayError_None)
			return false;

		//Set Overlay texture from file
		vr::Texture_t m_texture = { (void *)m_OverlayTexture->getTexture(), vr::API_DirectX ,  vr::ColorSpace_Gamma };
		vr::VROverlay()->SetOverlayTexture(m_ulOverlayHandle, &m_texture);
		vr::VROverlay()->SetOverlayAlpha(m_ulOverlayHandle, m_transparancy);

		//Show the overlay
		overlayError = vr::VROverlay()->ShowOverlay(m_ulOverlayHandle);
		bSuccess = overlayError == vr::VROverlayError_None;
		m_isVisible = true;
		
		//m_updateThread = new boost::thread(&WindowOverlay::setupThread, this);
		m_updateThread = new boost::thread(&WindowOverlay::setupThread, this);
	}

	return bSuccess;
}

void WindowOverlay::HideOverlay()
{
	if (m_ulOverlayHandle == vr::k_ulOverlayHandleInvalid)
		return;

	vr::VRCompositor();  // Required to call overlays...
	//vr::VROverlay()->HideOverlay(m_ulOverlayHandle);
	
	m_io->stop();

	//Stop Thread and cleanup
	m_updateThread->join();
	delete m_updateThread;
	m_updateThread = NULL;

	vr::VROverlay()->ClearOverlayTexture(m_ulOverlayHandle);
	vr::VROverlay()->DestroyOverlay(m_ulOverlayHandle);
	m_ulOverlayHandle = vr::k_ulOverlayHandleInvalid;

	m_isVisible = false;
}

void WindowOverlay::handleEvent(const vr::VREvent_t & event)
{
}
void WindowOverlay::setTracking(uint32_t device)
{
	vr::HmdVector2_t overlayCenter;
	vr::HmdMatrix34_t newOverlayTransform;
	m_TrackedDevice = device;

	overlayCenter.v[0] = 0.5f;
	overlayCenter.v[1] = 0.5f;
	
	switch (device)
	{
	case 0:
		vr::VROverlay()->GetTransformForOverlayCoordinates(m_ulOverlayHandle, vr::VRCompositor()->GetTrackingSpace(), overlayCenter, &newOverlayTransform);
		setOverlayMatrix(newOverlayTransform);
		break;
	case 1:
		m_xTrans = 0;
		m_yTrans = 0;
		m_zTrans = -9;

		m_xRotate = 0;
		m_yRotate = 0;
		m_zRotate = 0;

		m_scale = 100;
		break;
	case 2:
	case 3:
		m_xTrans = 0;
		m_yTrans = 2;
		m_zTrans = 0;

		m_xRotate = -80;
		m_yRotate = 0;
		m_zRotate = 0;

		m_scale = 30;
		break;
	}


	setOverlayTracking();
	updateTransform();
}

uint32_t WindowOverlay::getTracking() const
{
	return m_TrackedDevice;
}

void WindowOverlay::setRate(int rateSelect)
{
	/*
	* 0 - 1 FPS
	* 1 - 5 FPS
	* 2 - 10 FPS
	* 3 - 24 FPS
	* 4 - 30 FPS
	* 5 - 50 FPS
	* 6 - 60 FPS
	*/
	switch (rateSelect)
	{
	case 0:
		m_rate = 1000;
		break;
	case 1:
		m_rate = 1000 / 5;
		break;
	case 2:
		m_rate = 1000 / 10;
		break;
	case 3:
		m_rate = 1000 / 24;
		break;
	case 4:
		m_rate = 1000 / 30;
		break;
	case 5:
		m_rate = 1000 / 50;
		break;
	case 6:
		m_rate = 1000 / 60;
		break;
	}

}
int WindowOverlay::getRate() const
{
	switch (m_rate)
	{
	case 1000:
		return 0;
		break;
	case (1000 / 5):
		return 1;
		break;
	case (1000 / 10):
		return 2;
		break;
	case (1000 / 24):
		return 3;
		break;
	case (1000 / 30):
		return 4;
		break;
	case (1000 / 50):
		return 5;
		break;
	case (1000 / 60):
		return 6;
	}
}


void WindowOverlay::setTransparancy(int transparancy)
{
	if (transparancy < 0)
	{
		m_transparancy = 0.0f;
	}
	else if (transparancy > 100)
	{
		m_transparancy = 1.0f;
	}
	else
	{
		m_transparancy = ((float)transparancy) / 100.0f;
	}
	
	vr::VROverlay()->SetOverlayAlpha(m_ulOverlayHandle, m_transparancy);
}
int WindowOverlay::getTransparancy() const
{
	return (int)(m_transparancy*100.0f);
}

void WindowOverlay::setOverlayTracking()
{
	uint32_t controller1 = -1;
	uint32_t controller2 = -1;
	bool first = true;

	//Set overlay to the VR world
	if (m_TrackedDevice == 0)
	{
		vr::VROverlay()->SetOverlayTransformAbsolute(m_ulOverlayHandle, vr::VRCompositor()->GetTrackingSpace(), &m_overlayDistanceMtx);
		return;
	}


	//TODO: remove this and just use the controller value set by the main window
	//main window will enumerate the controllers to select from and pass the selection to the overlay
	//Still check to make sure that the device class is a controller and if it fails revert to spacial display and reset position.
	for (int i = 0; i < vr::k_unMaxTrackedDeviceCount; i++)
	{
		switch (m_vrSys->GetTrackedDeviceClass(i))
		{
		case vr::TrackedDeviceClass_Controller:
			if (controller1 == -1)
			{
				controller1 = i;
			}
			if (controller1 >= 0 && i != controller1)
			{
				controller2 = i;
			}
			if (controller2 >= 0)
			{
				break;
			}
		}
	}
	
	
	if (m_TrackedDevice == 2 && controller1 >= 0)
	{
		vr::VROverlay()->SetOverlayTransformTrackedDeviceRelative(m_ulOverlayHandle, controller1, &m_overlayDistanceMtx);
		return;
	}
	if (m_TrackedDevice == 3 && controller2 >= 0)
	{
		vr::VROverlay()->SetOverlayTransformTrackedDeviceRelative(m_ulOverlayHandle, controller2, &m_overlayDistanceMtx);
		return;
	}

	m_TrackedDevice = 1;
	vr::VROverlay()->SetOverlayTransformTrackedDeviceRelative(m_ulOverlayHandle, vr::k_unTrackedDeviceIndex_Hmd, &m_overlayDistanceMtx);
	
	
}

void WindowOverlay::asyncUpdate()
{
	
	updateTexture();
	m_timer->expires_from_now(boost::posix_time::milliseconds(m_rate));
	m_timer->async_wait(boost::bind(&WindowOverlay::asyncUpdate, this));
}

bool WindowOverlay::isVisible() const
{
	return m_isVisible;
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

	ID3D11Texture2D* oldTex = m_OverlayTexture->getTexture();
	m_OverlayTexture->setTextureFromWindow(m_targetHwnd, x, y);
	

	ID3D11ShaderResourceView* shaderResource;
	uint32_t pwidth;
	uint32_t pHeight;
	uint32_t pNativeFormat;
	vr::EGraphicsAPIConvention pAPI;
	vr::EColorSpace pColorSpace;

	if (oldTex != m_OverlayTexture->getTexture())
	{
		vr::VROverlay()->ClearOverlayTexture(m_ulOverlayHandle);
		vr::Texture_t m_texture = { (void *)m_OverlayTexture->getTexture(), vr::API_DirectX ,  vr::ColorSpace_Gamma };
		vr::VROverlay()->SetOverlayTexture(m_ulOverlayHandle, &m_texture);
	}

	//Get reference to OpenVR ID3D11Texture2D
	vr::VROverlay()->GetOverlayTexture(m_ulOverlayHandle, (void **)&shaderResource, m_OverlayTexture->getTexture(), &pwidth, &pHeight, &pNativeFormat, &pAPI, &pColorSpace);
	
	//Copy Texture data from OverlayTexture to the openVR Texture reference
	m_OverlayTexture->updateTexture(shaderResource);

	//Release OpenVR texture reference
	vr::VROverlay()->ReleaseNativeOverlayHandle(m_ulOverlayHandle, (void*)shaderResource);

	shaderResource = NULL;


	setOverlayTracking();
}

void WindowOverlay::setScale(const int scale)
{
	if (scale < 0)
	{
		m_scale = 0;
	}
	if (scale > 100)
	{
		m_scale = 100;
	}
	else
	{
		m_scale = scale;
	}
	
	updateTransform();
}
int WindowOverlay::getScale() const {
	return m_scale;
}

unsigned int WindowOverlay::getWidth() const
{
	return m_texWidth;
}
unsigned int WindowOverlay::getHeight() const
{
	return m_texHeight;
}

vr::VROverlayHandle_t WindowOverlay::getOverlayHandle() const
{
	return m_ulOverlayHandle;
}

vr::HmdMatrix34_t WindowOverlay::getOverlayMatrix() const
{
	return m_overlayDistanceMtx;
}

void WindowOverlay::setOverlayMatrix(const vr::HmdMatrix34_t & relativePosition)
{
	
	//m_overlayDistanceMtx = relativePosition;

	m_xTrans = relativePosition.m[0][3] * 25.0f;// *(m_scale / 100.0f);
	m_yTrans = relativePosition.m[1][3] * 25.0f;// *(m_scale / 100.0f);
	m_zTrans = relativePosition.m[2][3] *25.0f;// *25.0f*(m_scale / 100.0f);

	m_xRotate = std::atan2(relativePosition.m[2][1], relativePosition.m[2][2]) * (180.0f / 3.14159f);
	m_yRotate = std::atan2(-relativePosition.m[2][0], std::sqrt((relativePosition.m[2][1] * relativePosition.m[2][1]) +
																	(relativePosition.m[2][2] * relativePosition.m[2][2]))) * (180.0f / 3.14159f);
	m_zRotate = std::atan2(relativePosition.m[1][0], relativePosition.m[0][0]) * (180.0f / 3.14159f);

	updateTransform();
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

int WindowOverlay::getRotate(const int axis) const
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

int WindowOverlay::getTrans(const int axis) const
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
	vr::HmdMatrix44_t matrixResult;
	vr::HmdMatrix44_t result;
	memset(&scale, 0, sizeof(scale));
	memset(&Translation, 0, sizeof(Translation));
	memset(&xRotate, 0, sizeof(xRotate));
	memset(&yRotate, 0, sizeof(yRotate));
	memset(&zRotate, 0, sizeof(zRotate));


	//Setup Scale (Currently fixed)
	scale.m[0][0] = (float)m_scale / 100.0f; //X Scale
	scale.m[1][1] = (float)m_scale / 100.0f; //Y Scale
	scale.m[2][2] = (float)m_scale / 100.0f; //Z Scale
	scale.m[3][3] = 1.0f; //unused


	//Setup Translation
	Translation.m[0][0] = 1.0f;
	Translation.m[1][1] = 1.0f;
	Translation.m[2][2] = 1.0f;
	Translation.m[2][2] = 1.0f;
	Translation.m[0][3] = ((float)(m_xTrans) / 25.0f);// / scale.m[0][0];
	Translation.m[1][3] = ((float)(m_yTrans) / 25.0f);// / scale.m[0][0];
	Translation.m[2][3] = ((float)(m_zTrans) / 25.0f);// / scale.m[0][0];
	Translation.m[3][3] = 1.0f;

	//Setup xRotate matrix
	xRotate.m[0][0] = 1.0f;
	xRotate.m[1][1] = std::cos(((float)m_xRotate)*(3.14159f / 180.0f));
	xRotate.m[1][2] = std::sin(((float)m_xRotate)*(3.14159f / 180.0f)) * -1.0f;
	xRotate.m[2][1] = std::sin(((float)m_xRotate)*(3.14159f / 180.0f));
	xRotate.m[2][2] = std::cos(((float)m_xRotate)*(3.14159f / 180.0f));
	xRotate.m[3][3] = 1.0f;

	//Setup yRotate matrix
	yRotate.m[0][0] = std::cos(((float)m_yRotate)*(3.14159f / 180.0f));
	yRotate.m[0][2] = std::sin(((float)m_yRotate)*(3.14159f / 180.0f));
	yRotate.m[1][1] = 1.0f;
	yRotate.m[2][0] = std::sin(((float)m_yRotate)*(3.14159f / 180.0f)) * -1.0f;
	yRotate.m[2][2] = std::cos(((float)m_yRotate)*(3.14159f / 180.0f));
	yRotate.m[3][3] = 1.0f;

	//Setup zRotate matrix
	zRotate.m[0][0] = std::cos(((float)m_zRotate)*(3.14159f / 180.0f));
	zRotate.m[0][1] = std::sin(((float)m_zRotate)*(3.14159f / 180.0f)) * -1.0f;
	zRotate.m[1][0] = std::sin(((float)m_zRotate)*(3.14159f / 180.0f));
	zRotate.m[1][1] = std::cos(((float)m_zRotate)*(3.14159f / 180.0f));
	zRotate.m[2][2] = 1.0f;
	zRotate.m[3][3] = 1.0f;

	//Matrix product all of them
	//matrixResult = multMatrix(xRotate, yRotate);
	//matrixResult = multMatrix(matrixResult, zRotate);
	result = multMatrix(Translation, zRotate);
	result = multMatrix(result, xRotate);
	result = multMatrix(result, yRotate);
	result = multMatrix(result, scale);

	for (int i= 0; i < 3; i++)
		for (int j = 0; j < 4; j++)
		{
			m_overlayDistanceMtx.m[i][j] = result.m[i][j];
		}
}



void WindowOverlay::setName(const std::wstring& name)
{
	m_wndName = name;
}

void WindowOverlay::setExeName(const std::wstring& name)
{
	m_exeName = name;
}

std::wstring WindowOverlay::getName() const
{
	return std::wstring(m_wndName);
}

std::wstring WindowOverlay::getExeName() const
{
	return std::wstring(m_exeName);
}



