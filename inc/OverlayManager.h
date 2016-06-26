#pragma once
#include <vector>
#include "Overlay.h"
class OverlayManager
{
public:
	OverlayManager();
	OverlayManager(const std::vector<std::shared_ptr<Overlay>>& overlayList);
	virtual ~OverlayManager();

	void addOverlay(Overlay* overlayObj);
	void updateOverlays();

	std::vector<std::shared_ptr<Overlay>> getOverlays() const;

	void sendEvent(const vr::VREvent_t & event);

	
private:
	std::vector<std::shared_ptr<Overlay>> m_overlayVec;
};



