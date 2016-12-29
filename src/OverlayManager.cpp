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
#include "OverlayManager.h"



OverlayManager::OverlayManager()
	:m_overlayVec()
	, m_updateThread(NULL)
	, m_timer(m_io, boost::posix_time::millisec(20))
	, m_controller1TouchPressed(false)
	, m_controller2TouchPressed(false)
	, m_controller1GripPressed(false)
	, m_controller2GripPressed(false)
{
	m_timer.async_wait(boost::bind(&OverlayManager::asyncUpdate, this));
	m_updateThread = new boost::thread(&OverlayManager::setupThread, this);
}

OverlayManager::OverlayManager(const std::vector<std::shared_ptr<Overlay>>& overlayList)
	:m_overlayVec(overlayList)
	, m_updateThread(NULL)
	, m_timer(m_io, boost::posix_time::millisec(20))
	, m_controller1TouchPressed(false)
	, m_controller2TouchPressed(false)
	, m_controller1GripPressed(false)
	, m_controller2GripPressed(false)
{
	m_timer.async_wait(boost::bind(&OverlayManager::asyncUpdate, this));
	m_updateThread = new boost::thread(&OverlayManager::setupThread, this);
}


OverlayManager::~OverlayManager()
{
}

void OverlayManager::addOverlay(Overlay* overlayObj)
{
	boost::lock_guard<boost::mutex> guard(mtx_);
	m_overlayVec.push_back(std::shared_ptr<Overlay>(overlayObj));
}

void OverlayManager::updateOverlays()
{
	for (std::vector<std::shared_ptr<Overlay>>::iterator it = m_overlayVec.begin(); it != m_overlayVec.end(); ++it)
	{
		(*it)->updateTexture();
	}
}

void OverlayManager::delOverlay(int index)
{
	boost::lock_guard<boost::mutex> guard(mtx_);
	
	m_overlayVec.erase(m_overlayVec.begin() + index);

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


//TODO: This is ugly and hacky and needs to get refactored
void OverlayManager::asyncUpdate()
{
	boost::lock_guard<boost::mutex> guard(mtx_);

	vr::TrackedDeviceIndex_t controller1 = -1;
	vr::TrackedDeviceIndex_t controller2 = -1;

	vr::VRControllerState_t hmdState;
	vr::VRControllerState_t controller1State;
	vr::VRControllerState_t controller2State;

	vr::TrackedDevicePose_t hmdPose;
	vr::TrackedDevicePose_t controller1Pose;
	vr::TrackedDevicePose_t controller2Pose;

	vr::HmdMatrix34_t overlayTransform;
	vr::HmdVector2_t overlayCenter;

	vr::ETrackingUniverseOrigin origin;

	

	unsigned int width, height;

	

	//Find the controllers
	vr::IVRSystem* vrSys = vr::VRSystem();
	vr::IVRCompositor* vrComp = vr::VRCompositor();
	vr::IVROverlay* vrOvrly = vr::VROverlay();

	for (int i = 0; i < vr::k_unMaxTrackedDeviceCount; i++)
	{
		switch (vrSys->GetTrackedDeviceClass(i))
		{
		case vr::TrackedDeviceClass_Controller:
			if (controller1 == -1)
			{
				controller1 = i;
			}
			if (controller1 >= 0 && i !=controller1)
			{
				controller2 = i;
			}
			if (controller2 >= 0)
			{
				break;
			}
		}
	}


	int count = 0;
	for (std::vector<std::shared_ptr<Overlay>>::iterator it = m_overlayVec.begin(); it != m_overlayVec.end(); ++it)
	{
		if (vrSys && controller1 >= 0 && (*it)->isVisible())
		{
			//Set padding of the overlay based on scale
			float padding = 0.5f * ((float)(*it)->getScale() / 100.0f);
			float z_padding = 0.1f;

			//Get the controller pose information relative to tracking space
			vrSys->GetControllerStateWithPose(vrComp->GetTrackingSpace(), controller1, &controller1State, sizeof(controller1State), &controller1Pose);
			vrSys->GetControllerStateWithPose(vrComp->GetTrackingSpace(), controller2, &controller2State, sizeof(controller2State), &controller2Pose);
			vrSys->GetControllerStateWithPose(vrComp->GetTrackingSpace(), vr::k_unTrackedDeviceIndex_Hmd, &hmdState, sizeof(hmdState), &hmdPose);
			
			//Center of the overlay adjusted for scale
			overlayCenter.v[0] = 0.5f;// * ((float)(*it)->getScale() / 100.0f);
			overlayCenter.v[1] = 0.5f;// * ((float)(*it)->getScale() / 100.0f);

			//Get the overlay transform relative to tracking space
			vr::EVROverlayError err = vr::VROverlayError_None;
			if (err = vrOvrly->GetTransformForOverlayCoordinates((*it)->getOverlayHandle(), vrComp->GetTrackingSpace(), overlayCenter, &overlayTransform))
			{
				DBOUT("Error with overlay!!" << err << std::endl);
			}

			//Converts Controller world tracking transform matrix to a transform matrix relative to the overlay
			Eigen::Transform<float, 3, Eigen::Affine> controller1Transform;
			Eigen::Transform<float, 3, Eigen::Affine> controller2Transform;
			Eigen::Transform<float, 3, Eigen::Affine> overlayTrans;

			for (int i = 0; i < 3; ++i)
			{
				for (int j = 0; j < 4; ++j)
				{
					controller1Transform(i, j) = controller1Pose.mDeviceToAbsoluteTracking.m[i][j];
					controller2Transform(i, j) = controller2Pose.mDeviceToAbsoluteTracking.m[i][j];
					overlayTrans(i, j) = overlayTransform.m[i][j];
				}
			}
			Eigen::Matrix<float, 4, 4> overlayInverse = overlayTrans.matrix().inverse();
			Eigen::Matrix<float, 4, 4> controller1OverlayTransform = overlayInverse * controller1Transform.matrix();
			Eigen::Matrix<float, 4, 4> controller2OverlayTransform = overlayInverse * controller2Transform.matrix();

			//Boolean values for if the controller is within the bounds of the overlay based on the padding
			//z-padding is used for the depth across the face of the overlay
			bool controller1InOverlay = (controller1OverlayTransform(0, 3) < padding && controller1OverlayTransform(0, 3) > -padding) &&
										(controller1OverlayTransform(1, 3) < padding && controller1OverlayTransform(1, 3) > -padding) &&
										(controller1OverlayTransform(2, 3) < z_padding && controller1OverlayTransform(2, 3) > -z_padding);
			
			bool controller2InOverlay = (controller2OverlayTransform(0, 3) < padding && controller2OverlayTransform(0, 3) > -padding) &&
										(controller2OverlayTransform(1, 3) < padding && controller2OverlayTransform(1, 3) > -padding) &&
										(controller2OverlayTransform(2, 3) < z_padding && controller2OverlayTransform(2, 3) > -z_padding);;

			//If the controller is within the bounds the center of the overlay
			if (controller1InOverlay || controller2InOverlay)
			{
				//Buzz controller  -- Not working fix
				vr::VRSystem()->TriggerHapticPulse(controller1, 2, 2000);

				if (controller1InOverlay && (*it)->getTracking() != 2)
				{
					//If controller1 is not currently tracking and controller2 isn't tracking overlay
					if(controller1State.rAxis[1].x > 0.75f && (m_controller1Tracking == NULL || m_controller1Tracking == (*it).get()) && m_controller2Tracking != (*it).get())
					{
						
						TrackingUpdate(it, controller1State, controller1Pose, controller1InOverlay, vrSys, vrComp);
						m_controller1Tracking = (*it).get();
						emit textureUpdated(count);
						
					}

					//If trigger is released and was tracking, reset pointer to null;
					if (controller1State.rAxis[1].x < 0.75f && m_controller1Tracking != NULL)
					{
						
						m_controller1Tracking = NULL;
					}


					//If touchpad is pressed in overlay w/ debounce check
					if (((controller1State.ulButtonPressed & vr::ButtonMaskFromId(vr::k_EButton_SteamVR_Touchpad)) == vr::ButtonMaskFromId(vr::k_EButton_SteamVR_Touchpad)) &&
						!m_controller1TouchPressed)
					{
						m_controller1TouchPressed = true;
						//bottom left - decrease opacity
						if (controller1State.rAxis[0].x < 0 && controller1State.rAxis[0].y < 0)
							(*it)->setTransparancy((*it)->getTransparancy() - 5);
						//bottom right - increase opacity
						if (controller1State.rAxis[0].x > 0 && controller1State.rAxis[0].y < 0)
							(*it)->setTransparancy((*it)->getTransparancy() + 5);
						//top left - decrease scale
						if (controller1State.rAxis[0].x < 0 && controller1State.rAxis[0].y > 0)
							(*it)->setScale((*it)->getScale() - 5);
						//top right - increase scale
						if (controller1State.rAxis[0].x > 0 && controller1State.rAxis[0].y > 0)
							(*it)->setScale((*it)->getScale() + 5);

						emit textureUpdated(count);
					}
					else if (((controller1State.ulButtonPressed & vr::ButtonMaskFromId(vr::k_EButton_SteamVR_Touchpad)) != vr::ButtonMaskFromId(vr::k_EButton_SteamVR_Touchpad)) &&
						m_controller1TouchPressed)
					{
						m_controller1TouchPressed = false;
					}

					//If SideButton is pressed
					else if (((controller1State.ulButtonPressed & vr::ButtonMaskFromId(vr::k_EButton_Grip)) == vr::ButtonMaskFromId(vr::k_EButton_Grip)) &&
						!m_controller1GripPressed)
					{
						m_controller1GripPressed = true;
						(*it)->setTracking(((*it)->getTracking() + 1) % 4);
						emit textureUpdated(count);
					}

					else if (((controller1State.ulButtonPressed & vr::ButtonMaskFromId(vr::k_EButton_Grip)) != vr::ButtonMaskFromId(vr::k_EButton_Grip)) &&
						m_controller1GripPressed)
					{
						m_controller1GripPressed = false;
					}

					
				}
				if (controller2InOverlay && (*it)->getTracking() != 3)
				{
					//If controller2 is not currently tracking and controller1 isn't tracking overlay			
					if (controller2State.rAxis[1].x > 0.75f && (m_controller2Tracking == NULL || m_controller2Tracking == (*it).get()) && m_controller1Tracking != (*it).get())
					{
						
						TrackingUpdate(it, controller2State, controller2Pose, controller2InOverlay, vrSys, vrComp);
						m_controller2Tracking = (*it).get();
						emit textureUpdated(count);
					}
					if (controller2State.rAxis[1].x < 0.75f && m_controller2Tracking != NULL)
					{
						m_controller2Tracking = NULL;
					}

					//If touchpad is pressed in overlay w/ debounce check
					if (((controller2State.ulButtonPressed & vr::ButtonMaskFromId(vr::k_EButton_SteamVR_Touchpad)) == vr::ButtonMaskFromId(vr::k_EButton_SteamVR_Touchpad)) &&
						!m_controller2TouchPressed)
					{
						m_controller2TouchPressed = true;
						//bottom left - decrease opacity
						if (controller2State.rAxis[0].x < 0 && controller2State.rAxis[0].y < 0)
							(*it)->setTransparancy((*it)->getTransparancy() - 5);
						//bottom right - increase opacity
						if (controller2State.rAxis[0].x > 0 && controller2State.rAxis[0].y < 0)
							(*it)->setTransparancy((*it)->getTransparancy() + 5);
						//top left - decrease scale
						if (controller2State.rAxis[0].x < 0 && controller2State.rAxis[0].y > 0)
							(*it)->setScale((*it)->getScale() - 5);
						//top right - increase scale
						if (controller2State.rAxis[0].x > 0 && controller2State.rAxis[0].y > 0)
							(*it)->setScale((*it)->getScale() + 5);

						emit textureUpdated(count);
					}
					else if (((controller2State.ulButtonPressed & vr::ButtonMaskFromId(vr::k_EButton_SteamVR_Touchpad)) != vr::ButtonMaskFromId(vr::k_EButton_SteamVR_Touchpad)) &&
						m_controller2TouchPressed)
					{
						m_controller2TouchPressed = false;
					}
					//If SideButton is pressed
					else if (((controller2State.ulButtonPressed & vr::ButtonMaskFromId(vr::k_EButton_Grip)) == vr::ButtonMaskFromId(vr::k_EButton_Grip)) &&
						!m_controller2GripPressed)
					{
						m_controller2GripPressed = true;
						(*it)->setTracking(((*it)->getTracking() + 1) % 4);
					}
					else if (((controller2State.ulButtonPressed & vr::ButtonMaskFromId(vr::k_EButton_Grip)) != vr::ButtonMaskFromId(vr::k_EButton_Grip)) &&
						m_controller2GripPressed)
					{
						m_controller2GripPressed = false;
					}


				}

				
			} //end controller in overlay if check

		} //end VR check if
		count++;
	} //end iterator loop for overlays 



	m_timer.expires_from_now(boost::posix_time::milliseconds(5));
	m_timer.async_wait(boost::bind(&OverlayManager::asyncUpdate, this));
}

void OverlayManager::setupThread()
{
	m_io.run();
}


void OverlayManager::TrackingUpdate(std::vector<std::shared_ptr<Overlay>>::iterator it, vr::VRControllerState_t controllerState, vr::TrackedDevicePose_t controllerPose, bool controllerInOverlay, vr::IVRSystem* vrSys, vr::IVRCompositor* vrComp)
{
	vr::TrackedDeviceIndex_t controller1 = -1;
	vr::TrackedDeviceIndex_t controller2 = -1;

	vr::VRControllerState_t hmdState;
	vr::VRControllerState_t controller1State;
	vr::VRControllerState_t controller2State;

	vr::TrackedDevicePose_t hmdPose;
	vr::TrackedDevicePose_t controller1Pose;
	vr::TrackedDevicePose_t controller2Pose;

	

	for (int i = 0; i < vr::k_unMaxTrackedDeviceCount; i++)
	{
		switch (vrSys->GetTrackedDeviceClass(i))
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

	vrSys->GetControllerStateWithPose(vrComp->GetTrackingSpace(), controller1, &controller1State, sizeof(controller1State), &controller1Pose);
	vrSys->GetControllerStateWithPose(vrComp->GetTrackingSpace(), controller2, &controller2State, sizeof(controller2State), &controller2Pose);
	vrSys->GetControllerStateWithPose(vrComp->GetTrackingSpace(), vr::k_unTrackedDeviceIndex_Hmd,  &hmdState, sizeof(hmdState), &hmdPose);


	if (controllerInOverlay) //controller trigger squeezed, in overlay and not being tracked to controller1
	{
		if ((*it)->getTracking() == 0)
		{
			(*it)->setOverlayMatrix(controllerPose.mDeviceToAbsoluteTracking);
		}
		else
		{
			//Must be same sized for matrix inverse calculation
			Eigen::Transform<float, 3, Eigen::Affine> trackedSource;
			Eigen::Transform<float, 3, Eigen::Affine> invertedSource;
			Eigen::Transform<float, 3, Eigen::Affine> controllerTransform;
			//Eigen::Transform<float, 3, Eigen::Affine> newTransform;
			vr::HmdMatrix34_t newPosition;
			memset(&newPosition, 0, sizeof(vr::HmdMatrix34_t));

			//HMD Calculation


			//Populate boost matrices
			for (unsigned i = 0; i < 3; ++i)
			{
				for (unsigned j = 0; j < 4; ++j)
				{
					
						if ((*it)->getTracking() == 1)
						{
							trackedSource(i, j) = hmdPose.mDeviceToAbsoluteTracking.m[i][j];
						}
						if ((*it)->getTracking() == 2)
						{
							trackedSource(i, j) = controller1Pose.mDeviceToAbsoluteTracking.m[i][j];
						}
						if ((*it)->getTracking() == 3)
						{
							trackedSource(i, j) = controller2Pose.mDeviceToAbsoluteTracking.m[i][j];
						}
						controllerTransform(i, j) = controllerPose.mDeviceToAbsoluteTracking.m[i][j];
				
					
				} //End Column loop
			}  //End Row loop

			
			Eigen::Matrix<float, 4, 4> invMatrix = trackedSource.matrix().inverse();
			Eigen::Matrix<float, 4, 4> newTransform;
			newTransform = invMatrix * controllerTransform.matrix();



			//Copy values from the new matrix into the openVR matrix
			for (unsigned i = 0; i < 3; ++i)
			{
				for (unsigned j = 0; j < 4; ++j)
				{
					if (i < 3)
					{
						newPosition.m[i][j] = newTransform(i, j);
					}
				} // end column loop
			} //end row loop

			//Maintain previous rotation


			(*it)->setOverlayMatrix(newPosition);
		} // end else (tracking check for non-spacial tracking)

	} //end controller check
}

