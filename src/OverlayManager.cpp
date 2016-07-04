#include "OverlayManager.h"



OverlayManager::OverlayManager()
	:m_overlayVec()
{
}

OverlayManager::OverlayManager(const std::vector<std::shared_ptr<Overlay>>& overlayList)
	:m_overlayVec(overlayList)
{
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
