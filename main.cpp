#include <string>
#include <cstdlib>
#include <QtCore>
#include <QImage>
#include <QBuffer>
#include <QByteArray>
#include <QApplication>

#include "imperialClock.h"


int main(int argc, char* argv[])
{
	Q_INIT_RESOURCE(resources);

	QApplication app(argc, argv);

	imperialClock clock;
	
	app.exec();
}