/*
**  VROverlay -- OpenVR window overlay management tool
**		Copyright(C) 2016 Joshua New
**
**	This program is free software : you can redistribute it and / or modify
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
#pragma once

#include "Overlay.h"
#include <vector>
#include <openvr.h>
#include <boost/thread.hpp>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/lu.hpp>
#include <boost/numeric/ublas/io.hpp>

#include <Eigen/Dense>
#include <Eigen/Geometry>
#include <Eigen/LU>

#include <QObject>

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

class OverlayManager : public QObject
{
	Q_OBJECT

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

signals:
	void textureUpdated(int index);

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

	bool m_controller1TouchPressed;
	bool m_controller2TouchPressed;

	bool m_controller1GripPressed;
	bool m_controller2GripPressed;
};



