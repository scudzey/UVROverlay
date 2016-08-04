#include "OverlayManager.h"



OverlayManager::OverlayManager()
	:m_overlayVec()
	, m_updateThread(NULL)
	, m_timer(m_io, boost::posix_time::millisec(20))
{
	m_timer.async_wait(boost::bind(&OverlayManager::asyncUpdate, this));
	m_updateThread = new boost::thread(&OverlayManager::setupThread, this);
}

OverlayManager::OverlayManager(const std::vector<std::shared_ptr<Overlay>>& overlayList)
	:m_overlayVec(overlayList)
	, m_updateThread(NULL)
	, m_timer(m_io, boost::posix_time::millisec(20))
{
	m_timer.async_wait(boost::bind(&OverlayManager::asyncUpdate, this));
	m_updateThread = new boost::thread(&OverlayManager::setupThread, this);
}


OverlayManager::~OverlayManager()
{
}

void OverlayManager::addOverlay(Overlay* overlayObj)
{
	m_overlayVec.push_back(std::shared_ptr<Overlay>(overlayObj));
}

void OverlayManager::updateOverlays()
{
	for (std::vector<std::shared_ptr<Overlay>>::iterator it = m_overlayVec.begin(); it != m_overlayVec.end(); ++it)
	{
		(*it)->updateTexture();
	}
}

const std::vector<std::shared_ptr<Overlay>>& OverlayManager::getOverlays() const
{
	return m_overlayVec;
}

void OverlayManager::sendEvent(const vr::VREvent_t& event)
{
	

	for (std::vector<std::shared_ptr<Overlay>>::iterator it = m_overlayVec.begin(); it != m_overlayVec.end(); ++it)
	{
		(*it)->handleEvent(event);
		
	}
}

void OverlayManager::asyncUpdate()
{
	vr::TrackedDeviceIndex_t controller1 = -1;
	vr::TrackedDeviceIndex_t controller2 = -1;

	vr::VRControllerState_t controller1State;
	vr::VRControllerState_t controller2State;

	vr::TrackedDevicePose_t controller1Pose;
	vr::TrackedDevicePose_t controller2Pose;

	vr::HmdMatrix34_t overlayTransform;
	vr::HmdVector2_t overlayCenter;

	vr::ETrackingUniverseOrigin origin;

	unsigned int width, height;

	float padding = 0.5f;

	//Find the controllers
	for (int i = 0; i < vr::k_unMaxTrackedDeviceCount; i++)
	{
		switch (vr::VRSystem()->GetTrackedDeviceClass(i))
		{
		case vr::TrackedDeviceClass_Controller:
			if (controller1 == -1)
			{
				controller1 = i;
			}
			if (controller1 >= 0)
			{
				controller2 = i;
			}
			if (controller2 >= 0)
			{
				break;
			}
		}
	}

	

	for (std::vector<std::shared_ptr<Overlay>>::iterator it = m_overlayVec.begin(); it != m_overlayVec.end(); ++it)
	{
		if (vr::VRSystem() && controller1 >= 0 && (*it)->isVisible())
		{
			//Get the controller pose information relative to tracking space
			vr::VRSystem()->GetControllerStateWithPose(vr::VRCompositor()->GetTrackingSpace(), controller1, &controller1State, &controller1Pose);
			//overlayCenter.v[0] = (*it)->getWidth() / 2;
			//overlayCenter.v[1] = (*it)->getHeight() / 2;
			vr::VROverlay()->GetOverlayTextureSize((*it)->getOverlayHandle(), &width, &height);
			
			overlayCenter.v[0] = 0.5f * ((float)(*it)->getScale() / 100.0f);
			overlayCenter.v[1] = 0.5f * ((float)(*it)->getScale() / 100.0f);

			//Get the overlay transform relative to tracking space
			vr::EVROverlayError err = vr::VROverlayError_None;
			if (err = vr::VROverlay()->GetTransformForOverlayCoordinates((*it)->getOverlayHandle(), vr::VRCompositor()->GetTrackingSpace(), overlayCenter, &overlayTransform))
			{
				DBOUT("Error with overlay!!" << err << std::endl);
			}
			//vr::VROverlay()->GetOverlayTransformAbsolute((*it)->getOverlayHandle(), &origin, &overlayTransform);
			DBOUT("Controller Pose: " << controller1Pose.mDeviceToAbsoluteTracking.m[0][3] << " " << controller1Pose.mDeviceToAbsoluteTracking.m[1][3] << " " << controller1Pose.mDeviceToAbsoluteTracking.m[2][3] << std::endl);
			DBOUT("Overlay Pose: " << overlayTransform.m[0][3] << " " << overlayTransform.m[1][3] << " " << overlayTransform.m[2][3] << std::endl);

			//If the controller is within the bounds the center of the overlay
			if ((controller1Pose.mDeviceToAbsoluteTracking.m[0][3] > (overlayTransform.m[0][3] - padding) && controller1Pose.mDeviceToAbsoluteTracking.m[0][3] < (overlayTransform.m[0][3] + padding)) &&
				(controller1Pose.mDeviceToAbsoluteTracking.m[1][3] > (overlayTransform.m[1][3] - padding) && controller1Pose.mDeviceToAbsoluteTracking.m[1][3] < (overlayTransform.m[1][3] + padding)) &&
				(controller1Pose.mDeviceToAbsoluteTracking.m[2][3] > (overlayTransform.m[2][3] - padding) && controller1Pose.mDeviceToAbsoluteTracking.m[2][3] < (overlayTransform.m[2][3] + padding))
				)
			{
				//Buzz controller
				DBOUT( "In area: " << controller1 << std::endl );
				vr::VRSystem()->TriggerHapticPulse(controller1, 0, 20);
				if (controller1State.rAxis[1].x > 0.75f)
				{
					DBOUT("MOVING!!" << std::endl);
					(*it)->setOverlayMatrix(controller1Pose.mDeviceToAbsoluteTracking);
				}
				
			}
		}
	}


	m_timer.expires_from_now(boost::posix_time::milliseconds(20));
	m_timer.async_wait(boost::bind(&OverlayManager::asyncUpdate, this));
}

void OverlayManager::setupThread()
{
	m_io.run();
}


