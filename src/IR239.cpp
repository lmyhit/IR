
//要置于顶部
#include "IR239.h"

using namespace std;
using namespace boost::asio;


IR239::IR239(QWidget *parent)
	: QMainWindow(parent),
	ui(new Ui::IR239Class)
{
	ui->setupUi(this);

	this->image = new QImage();
	connect(ui->pushButton, SIGNAL(clicked()), this, SLOT(onPushButtonClicked()));
	connect(ui->pushButton_2, SIGNAL(clicked()), this, SLOT(onPushButtonClicked2()));
	connect(ui->pushButton_7, SIGNAL(clicked()), this, SLOT(onPushButtonClicked7()));
	connect(ui->pushButton_3, SIGNAL(clicked()), this, SLOT(onPushButtonClicked3()));
	connect(ui->pushButton_4, SIGNAL(clicked()), this, SLOT(onPushButtonClicked4()));
	connect(ui->pushButton_5, SIGNAL(clicked()), this, SLOT(onPushButtonClicked5()));
	connect(ui->pushButton_6, SIGNAL(clicked()), this, SLOT(onPushButtonClicked6()));

}

void 
IR239::serial_SerialOn(QString content)
{
	serialt = new Serial(content);
	QThread * th = new QThread();
	serialt->moveToThread(th);

	connect(th, SIGNAL(started()), serialt, SLOT(OnStarted()));
	connect(serialt, SIGNAL(finished()), this, SLOT(onfinish()));
	th->start();
}


void
IR239::onPushButtonClicked()
{
	d = action();
	ui->textBrowser_4->insertPlainText(QString("sucessful image processing!"));
	QDateTime time = QDateTime::currentDateTime();//获取系统现在的时间
	QString str = time.toString("yyyy-MM-dd hh:mm:ss\n"); //设置显示格式
	ui->textBrowser_4->insertPlainText(QString(str));

	QGraphicsScene *scene2 = new QGraphicsScene;
	char *D2 = "C://Users/Administrator/Desktop/1.tif";
	scene2->addPixmap(QPixmap(D2));
	//ui->graphicsView_2->resize(361, 241);
	ui->graphicsView_2->setScene(scene2);
	ui->graphicsView_2->fitInView(scene2->sceneRect(), Qt::KeepAspectRatio);

	QGraphicsScene *scene3 = new QGraphicsScene;
	char *D3 = "C://Users/Administrator/Desktop/2.tif";
	scene3->addPixmap(QPixmap(D3)); 
	//ui->graphicsView_3->resize(361, 241);
	ui->graphicsView_3->setScene(scene3);
	ui->graphicsView_3->fitInView(scene3->sceneRect(), Qt::KeepAspectRatio);
}

void
IR239::onPushButtonClicked2()
{
	ui->textBrowser->setPlainText("");
	ui->textBrowser->insertPlainText(QString::number(d, 10, 4));
	QGraphicsScene *scene = new QGraphicsScene;
	char *D = "C://Users/Administrator/Desktop/3.tif";
	scene->addPixmap(QPixmap(D));
	ui->graphicsView->setScene(scene);
	ui->graphicsView->fitInView(scene->sceneRect(), Qt::KeepAspectRatio);
	ui->graphicsView->show();
}

void
IR239::onPushButtonClicked7()
{
	content.clear();
	QString str1 = "SHIFT ,";
	QString str3 = ",,,,\r\n";
	content.append(str1);
	content.append(QString::number(d, 10, 4));
	content.append(str3);
	ui->textBrowser_2->setPlainText("");
	ui->textBrowser_2->insertPlainText(content);
}

void
IR239::onPushButtonClicked3()
{
	serial = 1;
	ui->textBrowser_3->insertPlainText(QString("Serial port open!"));
	QDateTime time = QDateTime::currentDateTime();//获取系统现在的时间
	QString str = time.toString("yyyy-MM-dd hh:mm:ss\n"); //设置显示格式
	ui->textBrowser_3->insertPlainText(QString(str));

	
}



void
IR239::onPushButtonClicked5()
{
	if (serial)
	{
		
		serial_port sp(iosev, "COM3");
		// 设置参数
		QString str9 = ui->comboBox_2->currentText();
		bool ok;
		int val = str9.toInt(&ok, 10);
		sp.set_option(serial_port::baud_rate(val));
		sp.set_option(serial_port::flow_control(serial_port::flow_control::none));
		sp.set_option(serial_port::parity(serial_port::parity::none));
		sp.set_option(serial_port::stop_bits(serial_port::stop_bits::two));
		sp.set_option(serial_port::character_size(8));
		write(sp, buffer(content.toStdString(), 50));
		// 向串口读数据
		iosev.run();

		/**boost::asio::streambuf buf2;
		read_until(sp, buf2, ' ');
		write(sp, buf2);**/


		QString str5 = "Already sent!";
		ui->textBrowser_3->insertPlainText(str5);
		QDateTime time = QDateTime::currentDateTime();//获取系统现在的时间
		QString str = time.toString("yyyy-MM-dd hh:mm:ss\n"); //设置显示格式
		ui->textBrowser_3->insertPlainText(QString(str));
	}
	
}

void
IR239::onPushButtonClicked4()
{
	serial = 0;
	ui->textBrowser_3->insertPlainText(QString("Serial port close!"));
	QDateTime time = QDateTime::currentDateTime();//获取系统现在的时间
	QString str = time.toString("yyyy-MM-dd hh:mm:ss\n"); //设置显示格式
	ui->textBrowser_3->insertPlainText(QString(str));

	
}

void
IR239::onPushButtonClicked6()
{
	if (serial)
	{
		serial_SerialOn(content);
		QString str6 = "Waiting for request signal!";
		ui->textBrowser_3->insertPlainText(str6);
		QDateTime time = QDateTime::currentDateTime();//获取系统现在的时间
		QString str7 = time.toString("yyyy-MM-dd hh:mm:ss\n"); //设置显示格式
		ui->textBrowser_3->insertPlainText(QString(str7));
	}
}

void
IR239::onfinish()
{
	QString str5 = "Already sent back!";
	ui->textBrowser_3->insertPlainText(str5);
	QDateTime time = QDateTime::currentDateTime();//获取系统现在的时间
	QString str = time.toString("yyyy-MM-dd hh:mm:ss\n"); //设置显示格式
	ui->textBrowser_3->insertPlainText(QString(str));
}
