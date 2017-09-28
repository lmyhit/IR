#include "serial.h"


using namespace std;
using namespace boost::asio;




Serial::Serial(QString content)
	: content(content)
{

}






void
Serial::SerialOn(QString content)
{
	boost::asio::io_service iosev;
	serial_port sp(iosev, "COM3");
	// 设置参数
	//QString str9 = ui->comboBox_2->currentText();
	//bool ok;
	//int val = str9.toInt(&ok, 10);
	sp.set_option(serial_port::baud_rate(9600));
	sp.set_option(serial_port::flow_control(serial_port::flow_control::none));
	sp.set_option(serial_port::parity(serial_port::parity::none));
	sp.set_option(serial_port::stop_bits(serial_port::stop_bits::two));
	sp.set_option(serial_port::character_size(8));
	
	// 向串口读数据
	iosev.run();

	boost::asio::streambuf buf2;
	read_until(sp, buf2, 'SHIF');
	write(sp, buffer(content.toStdString(), 50));



}

void
Serial::OnStarted()
{
	this->SerialOn(content);
	emit finished();
}