#include "StaticOverlay.h"
#include "OverlayManager.h"
#include <openvr.h>

int main(int argc, char *argv[])
{
	//TODO: add VR hooking to it's own class
	vr::HmdError m_eLastHmdError;
	vr::IVRSystem *pVRSystem = vr::VR_Init(&m_eLastHmdError, vr::VRApplication_Overlay);

	if (m_eLastHmdError != vr::VRInitError_None)
	{
		
		return 1;
	}

	
	StaticOverlay* overlay = new StaticOverlay("C:\\openvr\\Texture.png");
	OverlayManager* mgr = new OverlayManager();
	mgr->addOverlay(overlay);

	vr::HmdMatrix34_t overlayDistanceMtx;
	memset(&overlayDistanceMtx, 0, sizeof(overlayDistanceMtx));
	overlayDistanceMtx.m[0][0] = overlayDistanceMtx.m[1][1] = overlayDistanceMtx.m[2][2] = 0.8f;
	overlayDistanceMtx.m[2][0] = 1.0f;
	overlayDistanceMtx.m[0][3] = 0.8f;
	overlayDistanceMtx.m[2][3] = -2.0f;

	overlay->setOverlayMatrix(overlayDistanceMtx);

	overlay->ShowOverlay();


	//TODO: Come up with a better looping strat
	while (1);

    return 0;
}
