#pragma once

#include "Overlay.h"
#include <boost/thread.hpp>
#include <D3D11_1.h>
#include <D3DX11.h>
#include <DXGI.h>
#include <string>

#include "OverlayTexture.h"

enum e_axis
{
	X_AXIS = 0,
	Y_AXIS,
	Z_AXIS
};

class WindowOverlay : public virtual Overlay
{
public:
	WindowOverlay(OverlayTexture* d3dTexture, vr::IVRSystem *vrsys);
	virtual ~WindowOverlay();
	void setupThread();
	virtual bool ShowOverlay();
	virtual void HideOverlay();
	virtual void handleEvent(const vr::VREvent_t& event);
	virtual void updateTexture();
	virtual void setOverlayMatrix(const vr::HmdMatrix34_t& relativePosition);
	virtual boost::uuids::uuid getOverlayUuid() const;
	virtual void setHwnd(HWND);

	void setRotate(const int axis, const int value);
	void setTrans(const int axis, const int value);

	int getRotate(const int axis) const;
	int getTrans(const int axis) const;

	void setTracking(uint32_t device);
	uint32_t getTracking() const;

	void setScale(const int scale);
	int getScale() const;

	

	std::wstring getName() const;
	std::wstring getExeName() const;
	void setName(const std::wstring& name);
	void setExeName(const std::wstring& name);
	void asyncUpdate();

private:
	void setOverlayTracking();
	void updateTransform();
	vr::HmdMatrix44_t multMatrix(vr::HmdMatrix44_t m1, vr::HmdMatrix44_t m2);

private:
	HWND m_targetHwnd;
	boost::uuids::uuid m_overlayUID;
	OverlayTexture* m_OverlayTexture;
	vr::VROverlayHandle_t m_ulOverlayHandle;
	vr::HmdMatrix34_t m_overlayDistanceMtx;
	vr::Texture_t m_texture;

	vr::IVRSystem *m_vrSys;

	int m_scale;

	

	uint32_t m_TrackedDevice;

	//Position Values
	int m_xRotate;
	int m_yRotate;
	int m_zRotate;

	int			 m_xTrans;
	int			 m_yTrans;
	int			 m_zTrans;

	std::wstring  m_wndName;
	std::wstring  m_exeName;

	boost::asio::io_service m_io;
	boost::asio::deadline_timer m_timer;
	boost::thread*				m_updateThread;
	boost::mutex				mtx_;




};


