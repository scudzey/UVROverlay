#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent, OverlayManager* mgr, vr::IVRSystem* vrSystem) :
	QMainWindow(parent),
	ui(new Ui::MainWindow),
	m_mgr(mgr),
	m_env(NULL),
	m_vrSystem(vrSystem)
{
    ui->setupUi(this);
	
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::show()
{
	QMainWindow::show();
	int32_t adapterIndex = 0;
	m_vrSystem->GetDXGIOutputInfo(&adapterIndex);
	HWND hWnd = (HWND)effectiveWinId();
	m_env = new RenderEnvironment(adapterIndex, hWnd);
}

void MainWindow::on_remOverlayButton_clicked()
{
    //Remove Overlay from manager and delete overlay
}


//Add overlay button clicked
void MainWindow::on_addOverlayButton_clicked()
{
	WindowSelect* selection = new WindowSelect();
	QObject::connect(selection, SIGNAL(WindowSelected(WindowDescriptor&)), this, SLOT(on_AddOverlay(WindowDescriptor&)));
	selection->exec();

}


//Overlay Selection changed
void MainWindow::on_overlayList_itemSelectionChanged()
{
    //Fillout Overlay settings groupbox
	int selectedIndex = ui->overlayList->currentRow();
	std::shared_ptr<Overlay> selectedOverlay = m_mgr->getOverlays()[selectedIndex];
	ui->titleBox->setText(QString::fromWCharArray(selectedOverlay->getName().c_str()));

	ui->xTranslationText->setText(QString::number(selectedOverlay->getTrans(X_AXIS)));
	ui->yTranslationText->setText(QString::number(selectedOverlay->getTrans(Y_AXIS)));
	ui->zTranslationText->setText(QString::number(selectedOverlay->getTrans(Z_AXIS)));

	ui->xRotationSlider->setValue(selectedOverlay->getRotate(X_AXIS));
	ui->xRotateText->setText(QString::number(selectedOverlay->getRotate(X_AXIS)));
	
	ui->yRotationSlider->setValue(selectedOverlay->getRotate(Y_AXIS));
	ui->yRotateText->setText(QString::number(selectedOverlay->getRotate(Y_AXIS)));
	
	ui->zRotationSlizer->setValue(selectedOverlay->getRotate(Z_AXIS));
	ui->zRotateText->setText(QString::number(selectedOverlay->getRotate(Z_AXIS)));

	ui->scaleSlider->setValue(selectedOverlay->getScale());
	ui->scaleText->setText(QString::number(selectedOverlay->getScale()));


}

void MainWindow::on_keybindBox_cursorPositionChanged(int arg1, int arg2)
{
    //Trigger keybind dialog
}

void MainWindow::on_xRotationSlider_sliderMoved(int position)
{
	int selectedIndex = ui->overlayList->currentRow();
	if (selectedIndex >= 0)
	{
		m_mgr->getOverlays()[selectedIndex]->setRotate(X_AXIS, ui->xRotationSlider->value());
	}
	ui->xRotateText->setText(QString::number(m_mgr->getOverlays()[selectedIndex]->getRotate(X_AXIS)));
}

void MainWindow::on_yRotationSlider_sliderMoved(int position)
{
	int selectedIndex = ui->overlayList->currentRow();
	if (selectedIndex >= 0)
	{
		m_mgr->getOverlays()[selectedIndex]->setRotate(Y_AXIS, ui->yRotationSlider->value());
	}
	ui->yRotateText->setText(QString::number(m_mgr->getOverlays()[selectedIndex]->getRotate(Y_AXIS)));
}

void MainWindow::on_zRotationSlizer_sliderMoved(int position)
{
	int selectedIndex = ui->overlayList->currentRow();
	if (selectedIndex >= 0)
	{
		m_mgr->getOverlays()[selectedIndex]->setRotate(Z_AXIS, ui->zRotationSlizer->value());
	}
	ui->zRotateText->setText(QString::number(m_mgr->getOverlays()[selectedIndex]->getRotate(Z_AXIS)));
}

void MainWindow::on_scaleSlider_sliderMoved(int position)
{
	int selectedIndex = ui->overlayList->currentRow();
	if (selectedIndex >= 0)
	{
		m_mgr->getOverlays()[selectedIndex]->setScale(ui->scaleSlider->value());
	}
	ui->scaleText->setText(QString::number(m_mgr->getOverlays()[selectedIndex]->getScale()));
}

void MainWindow::on_trackingSelect_currentIndexChanged(int index)
{
	int selectedIndex = ui->overlayList->currentRow();
	if (selectedIndex >= 0)
	{
		m_mgr->getOverlays()[selectedIndex]->setTracking(ui->trackingSelect->currentIndex());
	}
}

void MainWindow::on_xRotateText_editingFinished()
{
	int selectedIndex = ui->overlayList->currentRow();
	bool ok;
	int value;
	
	value = ui->xRotateText->text().toInt(&ok);
	


	if (selectedIndex >= 0 && ok)
	{
		m_mgr->getOverlays()[selectedIndex]->setRotate(X_AXIS, value);
		ui->xRotationSlider->setValue(value);
	}
	else
	{
		ui->xRotateText->setText(QString::number(m_mgr->getOverlays()[selectedIndex]->getRotate(X_AXIS)));
	}
	
}

void MainWindow::on_yRotateText_editingFinished()
{
	int selectedIndex = ui->overlayList->currentRow();
	bool ok;
	int value;

	value = ui->yRotateText->text().toInt(&ok);



	if (selectedIndex >= 0 && ok)
	{
		m_mgr->getOverlays()[selectedIndex]->setRotate(Y_AXIS, value);
		ui->yRotationSlider->setValue(value);
	}
	else
	{
		ui->yRotateText->setText(QString::number(m_mgr->getOverlays()[selectedIndex]->getRotate(Y_AXIS)));
	}
}

void MainWindow::on_zRotateText_editingFinished()
{
	int selectedIndex = ui->overlayList->currentRow();
	bool ok;
	int value;

	value = ui->zRotateText->text().toInt(&ok);



	if (selectedIndex >= 0 && ok)
	{
		m_mgr->getOverlays()[selectedIndex]->setRotate(Z_AXIS, value);
		ui->zRotationSlizer->setValue(value);
	}
	else
	{
		ui->zRotateText->setText(QString::number(m_mgr->getOverlays()[selectedIndex]->getRotate(Z_AXIS)));
	}
}

void MainWindow::on_xTranslationText_editingFinished()
{
	int selectedIndex = ui->overlayList->currentRow();
	bool ok;
	int value;

	value = ui->xTranslationText->text().toInt(&ok);



	if (selectedIndex >= 0 && ok)
	{
		m_mgr->getOverlays()[selectedIndex]->setTrans(X_AXIS, value);
	}
	else
	{
		ui->xTranslationText->setText(QString::number(m_mgr->getOverlays()[selectedIndex]->getTrans(X_AXIS)));
	}
}

void MainWindow::on_yTranslationText_editingFinished()
{
	int selectedIndex = ui->overlayList->currentRow();
	bool ok;
	int value;

	value = ui->yTranslationText->text().toInt(&ok);



	if (selectedIndex >= 0 && ok)
	{
		m_mgr->getOverlays()[selectedIndex]->setTrans(Y_AXIS, value);
	}
	else
	{
		ui->yTranslationText->setText(QString::number(m_mgr->getOverlays()[selectedIndex]->getTrans(Y_AXIS)));
	}
}

void MainWindow::on_zTranslationText_editingFinished()
{
	int selectedIndex = ui->overlayList->currentRow();
	bool ok;
	int value;

	value = ui->zTranslationText->text().toInt(&ok);



	if (selectedIndex >= 0 && ok)
	{
		m_mgr->getOverlays()[selectedIndex]->setTrans(Z_AXIS, value);
	}
	else
	{
		ui->zTranslationText->setText(QString::number(m_mgr->getOverlays()[selectedIndex]->getTrans(Z_AXIS)));
	}
}

void MainWindow::on_scaleText_editingFinished()
{
	int selectedIndex = ui->overlayList->currentRow();
	bool ok;
	int value;

	value = ui->scaleText->text().toInt(&ok);



	if (selectedIndex >= 0 && ok)
	{
		m_mgr->getOverlays()[selectedIndex]->setScale(value);
		ui->scaleSlider->setValue(value);
	}
	else
	{
		ui->scaleText->setText(QString::number(m_mgr->getOverlays()[selectedIndex]->getScale()));
	}
}

void MainWindow::on_AddOverlay(WindowDescriptor& data)
{
	WindowOverlay* overlay = new WindowOverlay(new OverlayTexture(m_env), m_vrSystem);
	m_mgr->addOverlay(overlay);
	qDebug() << "adding overlay";
	vr::HmdMatrix34_t overlayDistanceMtx2;
	memset(&overlayDistanceMtx2, 0, sizeof(overlayDistanceMtx2));
	overlay->setHwnd(data.getHwnd());
	overlay->setName(data.getTitle());
	overlay->setExeName(data.getExe());
	overlay->setOverlayMatrix(overlayDistanceMtx2);
	overlay->ShowOverlay();


	ui->overlayList->clear();
	const std::vector<std::shared_ptr<Overlay>> vec = m_mgr->getOverlays();
	for (std::vector<std::shared_ptr<Overlay>>::const_iterator it = vec.begin(); it != vec.end(); ++it)
	{
		ui->overlayList->addItem(QString::fromWCharArray((*it)->getName().c_str(), (*it)->getName().length()));
	}
}

void MainWindow::on_zTranslateBackBig_clicked()
{
	int selectedIndex = ui->overlayList->currentRow();
	if (selectedIndex >= 0)
	{
		float transValue = m_mgr->getOverlays()[selectedIndex]->getTrans(Z_AXIS);
		m_mgr->getOverlays()[selectedIndex]->setTrans(Z_AXIS, transValue - 1.0f);
	}
	ui->zTranslationText->setText(QString::number(m_mgr->getOverlays()[selectedIndex]->getTrans(Z_AXIS)));
}














bool MainWindow::nativeEvent(const QByteArray& eventType, void* message, long* result)
{
	MSG *msg = static_cast<MSG *>(message);
	if (msg->message == WM_INPUT && false)
	{
		UINT dwSize;

		GetRawInputData((HRAWINPUT)msg->lParam, RID_INPUT, NULL, &dwSize,
			sizeof(RAWINPUTHEADER));
		LPBYTE lpb = new BYTE[dwSize];
		if (lpb == NULL)
		{
			return 0;
		}

		if (GetRawInputData((HRAWINPUT)msg->lParam, RID_INPUT, lpb, &dwSize,
			sizeof(RAWINPUTHEADER)) != dwSize)
			OutputDebugString(TEXT("GetRawInputData does not return correct size !\n"));

		RAWINPUT* raw = (RAWINPUT*)lpb;

		if (raw->header.dwType == RIM_TYPEKEYBOARD)
		{
			MessageBox(NULL, L"Test", L"Test", MB_OK);
		}
		if (raw->header.dwType == RIM_TYPEHID)
		{
			UINT bufferSize;
			GetRawInputDeviceInfo(raw->header.hDevice, RIDI_PREPARSEDDATA, NULL, &bufferSize);
			LPBYTE deviceInfo = new BYTE[bufferSize];

			//Add check for allocation


			//Get the HID preparsed data
			GetRawInputDeviceInfo(raw->header.hDevice, RIDI_PREPARSEDDATA, deviceInfo, &bufferSize);
			PHIDP_PREPARSED_DATA deviceInfoData = (PHIDP_PREPARSED_DATA)deviceInfo;
			HIDP_CAPS caps;
			HidP_GetCaps(deviceInfoData, &caps);

			LPBYTE ButtonCapsBuf = new BYTE[sizeof(HIDP_BUTTON_CAPS) * caps.NumberInputButtonCaps];
			//Add check for allocation

			//Get the button capabilities from HID
			PHIDP_BUTTON_CAPS ButtonCaps = (PHIDP_BUTTON_CAPS)ButtonCapsBuf;
			USHORT capsLength = caps.NumberInputButtonCaps;
			HidP_GetButtonCaps(HidP_Input, ButtonCaps, &capsLength, deviceInfoData);


			LPBYTE ValCapsBuf = new BYTE[sizeof(HIDP_BUTTON_CAPS) * caps.NumberInputValueCaps];

			//Find the current button usage data from HID device
			USAGE usage[128];
			ULONG usageLength = ButtonCaps->Range.UsageMax - ButtonCaps->Range.UsageMin + 1;
			HidP_GetUsages(HidP_Input, ButtonCaps->UsagePage, 0, usage, &usageLength, deviceInfoData, (PCHAR)raw->data.hid.bRawData, raw->data.hid.dwSizeHid);


			GetRawInputDeviceInfo(raw->header.hDevice, RIDI_DEVICENAME, NULL, &bufferSize);
			LPBYTE deviceNameInfoBuf = new BYTE[sizeof(wchar_t) * bufferSize];

			//Add check for allocation


			//Get the HID Name
			GetRawInputDeviceInfo(raw->header.hDevice, RIDI_DEVICENAME, deviceNameInfoBuf, &bufferSize);

			wchar_t DeviceName[127];
			HANDLE HIDHandle = CreateFile((LPCWSTR)deviceNameInfoBuf, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, NULL, NULL);
			if (HIDHandle)
			{
				HidD_GetProductString(HIDHandle, DeviceName, sizeof(wchar_t) * 126);
				CloseHandle(HIDHandle);
				for (int i = 0; i < usageLength; i++)
				{
					MessageBox(NULL, boost::lexical_cast<std::wstring>(usage[i] - ButtonCaps->Range.UsageMin).c_str(), std::wstring(DeviceName).c_str(), MB_OK);
				}
			}


			//Clean up byte arrays;
			delete[] deviceNameInfoBuf;
			delete[] ValCapsBuf;
			delete[] ButtonCapsBuf;
			delete[] deviceInfo;
		}

		//Clean up byte arrays;
		delete[] lpb;
		return true;
	}

	return false;
}


