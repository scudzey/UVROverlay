#pragma once

#include "Overlay.h"
#include <boost/thread.hpp>
#include <boost/optional.hpp>
#include <boost/utility/in_place_factory.hpp>
#include <boost/utility.hpp>

#include <Eigen/Dense>
#include <Eigen/Geometry>

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

enum e_device
{
	SPACIAL = 0,
	HMD,
	CONTROLLER1,
	CONTROLLER2
};

enum e_updateRate
{
	FPS_1 = 0,
	FPS_5,
	FPS_10,
	FPS_24,
	FPS_30,
	FPS_50,
	FPS_60
};

enum e_updateDelay
{
	DELAY_FPS_1 = 1000,
	DELAY_FPS_5 = (1000 / 5),
	DELAY_FPS_10 = (1000 / 10),
	DELAY_FPS_24 = (1000 / 24),
	DELAY_FPS_30 = (1000 / 30),
	DELAY_FPS_50 = (1000 / 50),
	DELAY_FPS_60 = (1000 / 60)
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

	unsigned int getWidth() const;
	unsigned int getHeight() const;

	vr::VROverlayHandle_t getOverlayHandle() const;
	vr::HmdMatrix34_t getOverlayMatrix() const;

	void setRate(int rateSelect);
	int getRate() const;

	void setTransparancy(int transparancy);
	int getTransparancy() const;

	

	std::wstring getName() const;
	std::wstring getExeName() const;
	void setName(const std::wstring& name);
	void setExeName(const std::wstring& name);
	void asyncUpdate();

	bool isVisible() const;

private:
	void setOverlayTracking();
	void updateTransform();

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

	boost::optional<boost::asio::io_service> m_io;
	boost::optional<boost::asio::deadline_timer> m_timer;
	boost::thread*				m_updateThread;
	boost::mutex				mtx_;

	unsigned int m_texWidth;
	unsigned int m_texHeight;

	bool m_isVisible;

	int m_rate;
	
	float m_transparancy;


};




