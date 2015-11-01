#include "imperialClock.h"

//------------------------------------------------------------------------------
//	FUNCTION: loadSettings [ ]
//------------------------------------------------------------------------------
void imperialClock::loadSettings()
{
	QSettings settings("Logitech", "Logitech Gaming Software", this);
	settings.beginGroup("LCDImperialClock");
	{
		smoothCheckBox->setChecked(settings.value("smoothMovement", true).toBool());
		dateCheckBox->setChecked(settings.value("dateEnabled", true).toBool());
		timeCheckBox->setChecked(settings.value("timeEnabled", true).toBool());
		twentyFourHourTimeCheckBox->setChecked(settings.value("twentyFourHourTime", false).toBool());
		personalizedMessageEdit->setText(settings.value("personalizedMessage", "").toString());
	}
	settings.endGroup();
}

//------------------------------------------------------------------------------
//	FUNCTION: saveSettings [ private ]
//------------------------------------------------------------------------------
void imperialClock::saveSettings()
{
	QSettings settings("Logitech", "Logitech Gaming Software", this);
	settings.beginGroup("LCDImperialClock");
	{
		settings.setValue("smoothMovement", smoothCheckBox->isChecked());
		settings.setValue("dateEnabled", dateCheckBox->isChecked());
		settings.setValue("timeEnabled", timeCheckBox->isChecked());
		settings.setValue("twentyFourHourTime", twentyFourHourTimeCheckBox->isChecked());
		settings.setValue("personalizedMessage", personalizedMessageEdit->text());
	}
	settings.endGroup();

	qApp->quit();
}

//------------------------------------------------------------------------------
//	FUNCTION: createGuiElements [ ]
//------------------------------------------------------------------------------
void imperialClock::createGuiElements()
{
	this->setLayout(new QHBoxLayout);

	optionsGroupBox = new QGroupBox("Options");

	this->layout()->addWidget(optionsGroupBox);

	smoothCheckBox = new QCheckBox("Smooth Movement (uses more CPU)");
	dateCheckBox = new QCheckBox("Show Date");
	timeCheckBox = new QCheckBox("Show Digital Time");
	twentyFourHourTimeCheckBox = new QCheckBox("24 Hour Time");

	connect(smoothCheckBox, &QCheckBox::toggled, [&](bool checked){ checked ? timer->setInterval(50) : timer->setInterval(1000); });
	connect(dateCheckBox, &QCheckBox::toggled, [&](bool checked){checked ? dateEnabled = true : dateEnabled = false; });
	connect(timeCheckBox, &QCheckBox::toggled, [&](bool checked){checked ? timeEnabled = true : timeEnabled = false; });

	smoothCheckBox->setToolTip("Smoothes the motion of the second hand sweep at the expense of some additional CPU processing");
	dateCheckBox->setToolTip("This can also be toggled by holding down the 'UP' button on the Logitech G19s D-pad.");
	timeCheckBox->setToolTip("This can also be toggled by holding down the 'DOWN' button on the Logitech G19s D-pad.");
	twentyFourHourTimeCheckBox->setToolTip("Toggles between a 12 and 24 hour clock when Digital time is enabled.");

	smoothCheckBox->setChecked(true);
	dateCheckBox->setChecked(true);
	timeCheckBox->setChecked(true);
	twentyFourHourTimeCheckBox->setChecked(false);

	personalizedMessageEdit = new QLineEdit(this);
	personalizedMessageEdit->setPlaceholderText("Your Personalized Message Here...");
	personalizedMessageEdit->setMaxLength(29);

	QVBoxLayout* layout = new QVBoxLayout;

	layout->addWidget(smoothCheckBox);
	layout->addWidget(dateCheckBox);
	layout->addWidget(timeCheckBox);
	layout->addWidget(twentyFourHourTimeCheckBox);
	layout->addWidget(personalizedMessageEdit);

	optionsGroupBox->setLayout(layout);

	// menu
	trayIconMenu = new QMenu(this);

	minimizeAction = new QAction("Minimize", trayIconMenu);
	restoreAction = new QAction("Restore", trayIconMenu);
	quitAction = new QAction("Exit", trayIconMenu);

	trayIconMenu->addAction(minimizeAction);
	trayIconMenu->addAction(restoreAction);
	trayIconMenu->addSeparator();
	trayIconMenu->addAction(quitAction);

	connect(minimizeAction, &QAction::triggered, this, std::bind(&QDialog::setVisible, this, false));
	connect(restoreAction, &QAction::triggered, this, std::bind(&QDialog::setVisible, this, true));
	connect(quitAction, &QAction::triggered, this, &imperialClock::saveSettings);

	// system tray
	QIcon icon(":/icon");
	trayIcon = new QSystemTrayIcon(icon, this);
	setWindowIcon(icon);

	connect(trayIcon, &QSystemTrayIcon::activated, [&](QSystemTrayIcon::ActivationReason reason)
	{
		if (reason == QSystemTrayIcon::Trigger)
		{
			this->setVisible(true);
		}
	});

	trayIcon->setToolTip("Logitech G19s Imperial Clock Applet");
	trayIcon->setContextMenu(trayIconMenu);
	trayIcon->show();
}

//------------------------------------------------------------------------------
//	FUNCTION: setupClockHands [ ]
//------------------------------------------------------------------------------
void imperialClock::setupClockHands()
{
	secondHand[0] = QPoint(0, 10);
	secondHand[1] = QPoint(1, 0);
	secondHand[2] = QPoint(0, -95);
	secondHand[3] = QPoint(-1, 0);

	minuteHand[0] = QPoint(0, 10);
	minuteHand[1] = QPoint(2, 0);
	minuteHand[2] = QPoint(0, -95);
	minuteHand[3] = QPoint(-2, 0);

	hourHand[0] = QPoint(0, 10);
	hourHand[1] = QPoint(4, 0);
	hourHand[2] = QPoint(0, -50);
	hourHand[3] = QPoint(-4, 0);
}

//------------------------------------------------------------------------------
//	FUNCTION: drawClock [ ]
//------------------------------------------------------------------------------
void imperialClock::drawClock()
{
	bool buttonUp = LogiLcdIsButtonPressed(LOGI_LCD_COLOR_BUTTON_UP);
	bool buttonDown = LogiLcdIsButtonPressed(LOGI_LCD_COLOR_BUTTON_DOWN);

	if (buttonUp && !prevButtonUp)
	{
		dateCheckBox->toggle();
	}
	if (buttonDown && !prevButtonDown)
	{
		timeCheckBox->toggle();
	}

	prevButtonDown = buttonDown;
	prevButtonUp = buttonUp;

	using namespace std;
	using namespace std::chrono;

	QDateTime now = QDateTime::currentDateTime();

	int hour = now.time().hour();
	if (!twentyFourHourTimeCheckBox->isChecked())
	{
		hour = hour % 12;
	}
	int minute = now.time().minute();
	int second = now.time().second();
	int msec = now.time().msec();

	QImage clock(*logo);

	QPainter p(&clock);
	p.setRenderHints(QPainter::HighQualityAntialiasing | QPainter::SmoothPixmapTransform);
	p.translate(clock.width() / 2, clock.height() / 2);

	p.setPen(Qt::NoPen);
	p.setBrush(Qt::white);

	// draw hours
	p.save();
	p.setBrush(QColor("#BBBBBB"));
	p.rotate(30.0 * ((hour + minute / 60.0)));
	p.drawConvexPolygon(hourHand, 4);
	p.restore();

	// draw minutes
	p.save();
	p.setBrush(QColor("#999999"));
	p.rotate(6.0 * ((minute + second / 60.0)));
	p.drawConvexPolygon(minuteHand, 4);
	p.restore();

	// draw seconds
	p.save();
	p.setBrush(Qt::white);
	p.rotate(6.0 * (second + msec / 1000.0));
	p.drawConvexPolygon(secondHand, 4);
	p.restore();

	p.setBrush(Qt::black);
	p.drawEllipse(QPoint(0, 0), 3, 3);
	p.end();

	clock.convertToFormat(QImage::Format_RGB32);

	LogiLcdColorSetBackground(clock.bits());

	if (dateEnabled)
	{
		std::stringstream date;
		date << ' ' << now.date().toString().toStdString();
		LogiLcdColorSetTitle(to_wchar_t(date.str().c_str()), 255, 255, 255);
	}
	else
	{
		LogiLcdColorSetTitle(to_wchar_t(""), 255, 255, 255);
	}

	if (!personalizedMessageEdit->text().isEmpty())
	{
		std::stringstream date;
		date << personalizedMessageEdit->text().toStdString();
		LogiLcdColorSetText(6, to_wchar_t(date.str().c_str()), 255, 255, 255);
	}
	else
	{
		LogiLcdColorSetText(6, to_wchar_t(""), 255, 255, 255);
	}

	if (timeEnabled)
	{
		std::stringstream time;
		time << "           " << hour << ':' << std::setfill('0') << std::setw(2) << minute << ':' << std::setw(2) << second;
		LogiLcdColorSetText(7, to_wchar_t(time.str().c_str()), 255, 255, 255);
	}
	else
	{
		LogiLcdColorSetText(7, to_wchar_t(""), 255, 255, 255);
	}

	LogiLcdUpdate();
}

//------------------------------------------------------------------------------
//	FUNCTION: startClock [ ]
//------------------------------------------------------------------------------
void imperialClock::startClock()
{
	connect(timer, &QTimer::timeout, this, &imperialClock::drawClock);
	timer->start(50);
}

//------------------------------------------------------------------------------
//	FUNCTION: initializeLcd [ ]
//------------------------------------------------------------------------------
void imperialClock::initializeLcd()
{
	LogiLcdInit(to_wchar_t("Imperial Clock"), LOGI_LCD_TYPE_COLOR);

	logo->convertToFormat(QImage::Format_RGB32);

	LogiLcdColorSetBackground(logo->bits());
	LogiLcdUpdate();
}

//------------------------------------------------------------------------------
//	FUNCTION: closeEvent [ ]
//------------------------------------------------------------------------------
void imperialClock::closeEvent(QCloseEvent *event)
{
	if (trayIcon->isVisible()) {
		trayIcon->showMessage(tr("Logitech G19s Imperial Clock"),
			tr("This program will continue to run in the "
			"background. To terminate the program, "
			"choose 'Quit' in the context menu "
			"of the system tray icon."));
		hide();
		event->ignore();
	}
}

//------------------------------------------------------------------------------
//	FUNCTION: setVisible [ ]
//------------------------------------------------------------------------------
void imperialClock::setVisible(bool visible)
{
	minimizeAction->setEnabled(visible);
	restoreAction->setEnabled(!visible);
	QDialog::setVisible(visible);
}

//------------------------------------------------------------------------------
//	FUNCTION: ~imperialClock [ ]
//------------------------------------------------------------------------------
imperialClock::~imperialClock()
{
	LogiLcdShutdown();
}

//------------------------------------------------------------------------------
//	FUNCTION: imperialClock [ ]
//------------------------------------------------------------------------------
imperialClock::imperialClock() : QDialog(),
logo(new QImage(":/logo")),
timer(new QTimer)
{
	setupClockHands();
	initializeLcd();
	createGuiElements();
	loadSettings();
	startClock();
}

//------------------------------------------------------------------------------
//	FUNCTION: to_wchar_t [ private ]
//------------------------------------------------------------------------------
wchar_t* imperialClock::to_wchar_t(const char* string)
{
	size_t newsize = strlen(string) + 1;
	wchar_t * wText = new wchar_t[newsize];
	size_t convertedChars = 0;
	mbstowcs_s(&convertedChars, wText, newsize, string, _TRUNCATE);
	return wText;
}