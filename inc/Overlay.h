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
#pragma once
#ifndef OVERLAY_H
#define OVERLAY_H
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/lexical_cast.hpp>
#include <windows.h>
#include <openvr.h>
//#include <windows.h>
class Overlay
{
public:

	virtual ~Overlay() {}

	virtual bool ShowOverlay() = 0;
	virtual void HideOverlay() = 0;
	virtual void handleEvent(const vr::VREvent_t& event) = 0;
	virtual void updateTexture() = 0;
	virtual void setOverlayMatrix(const vr::HmdMatrix34_t& relativePosition) = 0;
	virtual vr::HmdMatrix34_t getOverlayMatrix() const = 0;
	virtual boost::uuids::uuid getOverlayUuid() const = 0;
	virtual void setRotate(const int axis, const int value)=0;
	virtual void setTrans(const int axis, const int value)=0;

	virtual vr::VROverlayHandle_t getOverlayHandle() const = 0;

	virtual void toggleMoveLock() = 0;
	virtual bool getMoveLock() = 0;

	virtual void setScale(const int scale) = 0;
	virtual int getScale() const =0;

	virtual void setTracking(uint32_t device)=0;
	virtual uint32_t getTracking() const = 0 ;

	virtual void setRate(int rateSelect) = 0;
	virtual int getRate() const = 0;

	virtual void setTransparancy(int transparancy) = 0;
	virtual int getTransparancy() const = 0;


	virtual void asyncUpdate() = 0;

	virtual int getRotate(const int axis) const = 0 ;
	virtual int getTrans(const int axis) const = 0 ;

	virtual std::wstring getName() const =0;
	virtual void setName(const std::wstring& name)=0 ;

	virtual unsigned int getWidth() const = 0;
	virtual unsigned int getHeight() const = 0;

	virtual bool isVisible() const = 0;
};


#endif //Define OVERLAY_H