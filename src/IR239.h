#pragma once
#include "winsock2.h"
#include <boost/asio.hpp>
#include <boost/bind.hpp> 
#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QtWidgets/QMainWindow>
#include "ui_IR239.h"
#include "HalconCpp.h"   
#include "HDevThread.h" 
#include <iostream>
#include <string>
#include <QFileDialog>
#include <QDir>
#include <QThread>
#include <QDateTime>
#include <QTextStream>
#include <QMessageBox>
#include "serial.h"
namespace Ui {
	class IR239;
}
class IR239 : public QMainWindow
{
	Q_OBJECT

public:
	IR239(QWidget *parent = 0);
	double d;
	bool serial;
	QString content;
	boost::asio::io_service iosev;

	Serial *serialt;
	void serial_SerialOn(QString content);
private:
	Ui::IR239Class *ui;
	QImage *image;
private slots:
	void onPushButtonClicked();
	void onPushButtonClicked2();
	void onPushButtonClicked3();
	void onPushButtonClicked4();
	void onPushButtonClicked7();
	void onPushButtonClicked5();
	void onPushButtonClicked6();
	void onfinish();
};

using namespace HalconCpp;



void dev_open_window_fit_image(HObject ho_Image, HTuple hv_Row, HTuple hv_Column,
	HTuple hv_WidthLimit, HTuple hv_HeightLimit, HTuple *hv_WindowHandle);

void dev_update_off();

void p_cluster_normals(HTuple hv_Pose1, HTuple hv_Pose2, HTuple hv_CenterNormal1,
	HTuple hv_CenterNormal2, HTuple *hv_ClusterP1, HTuple *hv_ClusterP2, HTuple *hv_ClusterCN1,
	HTuple *hv_ClusterCN2);

void p_determine_ellipse_contours(HObject ho_Image, HObject *ho_EllipseContoursLarge,
	HObject *ho_EllipseContoursSmall, HTuple hv_visualization, HTuple *hv_NumberLarge,
	HTuple *hv_NumberSmall);

void getcircleinformation(HObject ho_Image, HObject *ho_EllipseContoursLarge, HTuple *hv_Row,
	HTuple *hv_Column, HTuple *hv_R);



void dev_open_window_fit_image(HObject ho_Image, HTuple hv_Row, HTuple hv_Column,
	HTuple hv_WidthLimit, HTuple hv_HeightLimit, HTuple *hv_WindowHandle);

void dev_update_off();

void p_cluster_normals(HTuple hv_Pose1, HTuple hv_Pose2, HTuple hv_CenterNormal1,
	HTuple hv_CenterNormal2, HTuple *hv_ClusterP1, HTuple *hv_ClusterP2, HTuple *hv_ClusterCN1,
	HTuple *hv_ClusterCN2);

void p_determine_ellipse_contours(HObject ho_Image, HObject *ho_EllipseContoursLarge,
	HObject *ho_EllipseContoursSmall, HTuple hv_visualization, HTuple *hv_NumberLarge,
	HTuple *hv_NumberSmall);

double action();

#endif // MAINWINDOW_H