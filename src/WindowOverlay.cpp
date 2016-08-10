/*
**		VROverlay -- OpenVR window overlay management tool
**		Copyright(C) 2016 Joshua New
**
**		This program is free software : you can redistribute it and / or modify
**		it under the terms of the GNU General Public License as published by
**		the Free Software Foundation, either version 3 of the License, or
**		(at your option) any later version.
**
**		This program is distributed in the hope that it will be useful,
**		but WITHOUT ANY WARRANTY; without even the implied warranty of
**		MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the
**		GNU General Public License for more details.
**
**		You should have received a copy of the GNU General Public License
**		along with this program.If not, see <http://www.gnu.org/licenses/>.
*/
#include "WindowOverlay.h"
#include <math.h>



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
		if (m_OverlayTexture->getTexture())
			m_OverlayTexture->getTexture()->Release();
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
	case SPACIAL:
		vr::VROverlay()->GetTransformForOverlayCoordinates(m_ulOverlayHandle, vr::VRCompositor()->GetTrackingSpace(), overlayCenter, &newOverlayTransform);
		setOverlayMatrix(newOverlayTransform);
		break;
	case HMD:
		m_xTrans = 0;
		m_yTrans = 0;
		m_zTrans = -9;

		m_xRotate = 0;
		m_yRotate = 0;
		m_zRotate = 0;

		m_scale = 100;
		break;
	case CONTROLLER1:
	case CONTROLLER2:
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
	case FPS_1:
		m_rate = DELAY_FPS_1;
		break;
	case FPS_5:
		m_rate = DELAY_FPS_5;
		break;
	case FPS_10:
		m_rate = DELAY_FPS_10;
		break;
	case FPS_24:
		m_rate = DELAY_FPS_24;
		break;
	case FPS_30:
		m_rate = DELAY_FPS_30;
		break;
	case FPS_50:
		m_rate = DELAY_FPS_50;
		break;
	case FPS_60:
		m_rate = DELAY_FPS_60;
		break;
	}

}
int WindowOverlay::getRate() const
{
	switch (m_rate)
	{
	case DELAY_FPS_1:
		return FPS_1;
		break;
	case DELAY_FPS_5:
		return FPS_5;
		break;
	case DELAY_FPS_10:
		return FPS_10;
		break;
	case DELAY_FPS_24:
		return FPS_24;
		break;
	case DELAY_FPS_30:
		return FPS_30;
		break;
	case DELAY_FPS_50:
		return FPS_50;
		break;
	case DELAY_FPS_60:
		return FPS_60;
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
	if (m_TrackedDevice == SPACIAL)
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
	
	
	if (m_TrackedDevice == CONTROLLER1 && controller1 >= 0)
	{
		vr::VROverlay()->SetOverlayTransformTrackedDeviceRelative(m_ulOverlayHandle, controller1, &m_overlayDistanceMtx);
		return;
	}
	if (m_TrackedDevice == CONTROLLER2 && controller2 >= 0)
	{
		vr::VROverlay()->SetOverlayTransformTrackedDeviceRelative(m_ulOverlayHandle, controller2, &m_overlayDistanceMtx);
		return;
	}

	m_TrackedDevice = HMD;
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
	if (!m_OverlayTexture->setTextureFromWindow(m_targetHwnd, x, y))
		return;
	

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
	
	//Get Translations
	m_xTrans = relativePosition.m[0][3] * 25.0f;// *(m_scale / 100.0f);
	m_yTrans = relativePosition.m[1][3] * 25.0f;// *(m_scale / 100.0f);
	m_zTrans = relativePosition.m[2][3] *25.0f;// *25.0f*(m_scale / 100.0f);

	//Copy VR Matrix into Eigen matrix and calcuate euler angles of transform
	Eigen::Matrix3f transformAngles;
	for (int i = 0; i < 3; ++i)
	{
		for (int j = 0; j < 3; ++j)
		{
			transformAngles(i, j) = relativePosition.m[i][j];
		}
	}
	Eigen::Vector3f angles = transformAngles.eulerAngles(0, 1, 2);
	
	//Store angles
	m_xRotate = angles[0] * (180.0f / M_PI);	
	m_yRotate = angles[1] * (180.0f / M_PI);	
	m_zRotate = angles[2] * (180.0f / M_PI);	

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
	//Setup translation
	Eigen::Translation3f translation((m_xTrans) / 25.0f, (m_yTrans) / 25.0f, (m_zTrans) / 25.0f);

	//Setup Rotation
	Eigen::AngleAxisf rotationX((float)m_xRotate*(M_PI / 180.0f), Eigen::Vector3f::UnitX());
	Eigen::AngleAxisf rotationY((float)m_yRotate*(M_PI / 180.0f), Eigen::Vector3f::UnitY());
	Eigen::AngleAxisf rotationZ((float)m_zRotate*(M_PI / 180.0f), Eigen::Vector3f::UnitZ());

	//Setup Scale
	Eigen::Transform<float, 3, Eigen::Affine> transform = translation * (rotationX * rotationY * rotationZ) * Eigen::Scaling((float)m_scale / 100.0f);

	//Get Final Transform
	Eigen::Matrix<float, 4, 4> finalTransform = transform.matrix();

	//Copy Eigen Matrix to VR Matrix
	for (int i= 0; i < 3; i++)
		for (int j = 0; j < 4; j++)
		{
			m_overlayDistanceMtx.m[i][j] = finalTransform(i, j);
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



