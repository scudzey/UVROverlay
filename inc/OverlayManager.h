#pragma once

#include "Overlay.h"
#include <vector>
#include <openvr.h>
#include <boost/thread.hpp>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/lu.hpp>
#include <boost/numeric/ublas/io.hpp>

#define DBOUT( s )            \
{                             \
   std::wostringstream os_;    \
   os_ << s;                   \
   OutputDebugStringW( os_.str().c_str() );  \
}






/* Matrix inversion routine.
Uses lu_factorize and lu_substitute in uBLAS to invert a matrix */
template<class T>
bool InvertMatrix(const boost::numeric::ublas::matrix<T>& input, boost::numeric::ublas::matrix<T>& inverse)
{
	typedef boost::numeric::ublas::permutation_matrix<std::size_t> pmatrix;

	// create a working copy of the input
	boost::numeric::ublas::matrix<T> A(input);

	// create a permutation matrix for the LU-factorization
	pmatrix pm(A.size1());

	// perform LU-factorization
	int res = boost::numeric::ublas::lu_factorize(A, pm);
	if (res != 0)
		return false;

	// create identity matrix of "inverse"
	inverse.assign(boost::numeric::ublas::identity_matrix<T>(A.size1()));

	// backsubstitute to get the inverse
	boost::numeric::ublas::lu_substitute(A, pm, inverse);

	return true;
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
	//Update overlay tracking
	void TrackingUpdate(std::vector<std::shared_ptr<Overlay>>::iterator it, vr::VRControllerState_t controllerState, vr::TrackedDevicePose_t controllerPose, bool controllerInOverlay, vr::IVRSystem* vrSys, vr::IVRCompositor* vrComp);

private:
	std::vector<std::shared_ptr<Overlay>> m_overlayVec;

	boost::asio::io_service m_io;
	boost::asio::deadline_timer m_timer;
	boost::thread*				m_updateThread;
	boost::mutex				mtx_;

	Overlay* m_controller1Tracking = NULL;
	Overlay* m_controller2Tracking = NULL;
};



