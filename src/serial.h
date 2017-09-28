#pragma once

#include "winsock2.h"
#include <boost/asio.hpp>
#include <boost/bind.hpp> 
#include <QObject>
class Serial : public QObject
{
	Q_OBJECT

public:
	Serial(QString content);

	QString content;
//	void SerialOn(QString content);
signals:
	void finished();

public slots:
    void SerialOn(QString content);
	void OnStarted();
};
