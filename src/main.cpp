
#include "IR239.h"
#include <QtWidgets/QApplication> 
#include <QPalette>  
#include <QWidget>  
#include <QMessageBox>  
#include <stdio.h>  
#include <QDebug>
#include "stdlib.h"


int main(int argc, char *argv[])
{

	QApplication a(argc, argv);
	IR239 w;
	// Default settings used in HDevelop (can be omitted) 
	w.show();
	return a.exec();
}
