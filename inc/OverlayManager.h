#pragma once

#include "Overlay.h"
#include <vector>
#include <openvr.h>
#include <boost/thread.hpp>


#define DBOUT( s )            \
{                             \
   std::wostringstream os_;    \
   os_ << s;                   \
   OutputDebugStringW( os_.str().c_str() );  \
}

class OverlayManager
{
public:
	OverlayManager();
	OverlayManager(const std::vector<std::shared_ptr<Overlay>>& overlayList);
	virtual ~OverlayManager();

	void addOverlay(Overlay* overlayObj);
	void updateOverlays();
	void delOverlay(int index);

	const std::vector<std::shared_ptr<Overlay>>& getOverlays() const;

	void sendEvent(const vr::VREvent_t & event);

	void asyncUpdate();
	void setupThread();
	
private:
	std::vector<std::shared_ptr<Overlay>> m_overlayVec;

	boost::asio::io_service m_io;
	boost::asio::deadline_timer m_timer;
	boost::thread*				m_updateThread;
	boost::mutex				mtx_;
};



