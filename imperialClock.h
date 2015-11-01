//--------------------------------------------------------------------------------------------------
// 
/// @PROJECT	imperialClock
///	@AUTHORS	Nic Holthaus
/// @DATE		2014/12/25
//
//--------------------------------------------------------------------------------------------------
//
// The MIT License (MIT)
//
// Permission is hereby granted, free of charge, to any person obtaining a copy of this software 
// and associated documentation files (the "Software"), to deal in the Software without 
// restriction, including without limitation the rights to use, copy, modify, merge, publish, 
// distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the 
// Software is furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in all copies or 
// substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING 
// BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND 
// NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, 
// DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING 
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
//
//--------------------------------------------------------------------------------------------------
// 
// Copyright (c) 2014 Nic Holthaus
// 
//--------------------------------------------------------------------------------------------------

#ifndef imperialClock_h__
#define imperialClock_h__

//--------------------
//	INCLUDES
//--------------------

// Qt
#include <QDialog>
#include <cstdlib>
#include <QTimer>
#include <QPainter>
#include <QDateTime>
#include <QCheckBox>
#include <QMenu>
#include <QApplication>
#include <QAction>
#include <QSystemTrayIcon>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QMessageBox>
#include <QCloseEvent>
#include <QLineEdit>
#include <QSettings>

// std
#include <chrono>
#include <sstream>
#include <ctime>
#include <iostream>
#include <functional>
#include <iomanip>

// local
#include "LogitechLCDLib.h"

//--------------------------------------------------------------------------------
///	@class		imperialClock
//--------------------------------------------------------------------------------
/// @brief		main object for the imperialClock project
/// @details
//--------------------------------------------------------------------------------
class imperialClock : public QDialog
{
	Q_OBJECT

public:

	explicit imperialClock();
	virtual ~imperialClock();

	void setVisible(bool visible);
	void closeEvent(QCloseEvent *event) override;

protected:

	QGroupBox* optionsGroupBox;
	QCheckBox* smoothCheckBox;
	QCheckBox* dateCheckBox;
	QCheckBox* timeCheckBox;
	QCheckBox* twentyFourHourTimeCheckBox;

	QLineEdit* personalizedMessageEdit;

	QAction* minimizeAction;
	QAction* restoreAction;
	QAction* quitAction;

	QSystemTrayIcon* trayIcon;
	QMenu* trayIconMenu;

private:

	wchar_t* to_wchar_t(const char* string);
	void initializeLcd();
	void startClock();
	void drawClock();
	void setupClockHands();
	void createGuiElements();
	void loadSettings();
	void saveSettings();

private:

	QImage* logo;
	QTimer* timer;

	QPoint secondHand[4];
	QPoint minuteHand[4];
	QPoint hourHand[4];

	bool dateEnabled = true;
	bool timeEnabled = true;

	bool prevButtonUp;
	bool prevButtonDown;
};

#endif // imperialClock_h__