#include "dialog.h"
#include "ui_dialog.h"
#include <QSerialPort>
#include <QSerialPortInfo>
#include <string>
#include <QDebug>
#include <QMessageBox>


Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
    ui->temp_lcdNumber->display("-------");
    arduino = new QSerialPort(this);
    serialBuffer = "";
    parsed_data = "";
    temperature_value = 0.0;

    for (QSerialPortInfo info : QSerialPortInfo::availablePorts())
    {
        qDebug() << "Number of ports: " << QSerialPortInfo::availablePorts().length() << "\n";
            foreach(const QSerialPortInfo &serialPortInfo, QSerialPortInfo::availablePorts()){
                qDebug() << "Description: " << serialPortInfo.description() << "\n";
                qDebug() << "Has vendor id?: " << serialPortInfo.hasVendorIdentifier() << "\n";
                qDebug() << "Vendor ID: " << serialPortInfo.vendorIdentifier() << "\n";
                qDebug() << "Has product id?: " << serialPortInfo.hasProductIdentifier() << "\n";
                qDebug() << "Product ID: " << serialPortInfo.productIdentifier() << "\n";
       }
    }


     //  Identifica se a porta do arduino esta ativado.

    bool arduino_is_available = false;
    QString arduino_uno_port_name;

    //  Verifica se a porta serial esta disponivel
    foreach(const QSerialPortInfo &serialPortInfo, QSerialPortInfo::availablePorts()){

        //  verifica se a porta serial tem um identificador de produto e fornecedor
        if(serialPortInfo.hasProductIdentifier() && serialPortInfo.hasVendorIdentifier()){
            //  verifica de a ID do produto e do fornecedor correspondem a um arduino uno
            if((serialPortInfo.productIdentifier() == arduino_uno_product_id)
                    && (serialPortInfo.vendorIdentifier() == arduino_uno_vendor_id)){
                arduino_is_available = true; //    arduino esta disponivel nessa porta
                arduino_uno_port_name = serialPortInfo.portName();
            }
        }
    }

    /*
     *  abre e configura a porta arduino
     */
    if(arduino_is_available){
        qDebug() << "Found the arduino port...\n";
        arduino->setPortName(arduino_uno_port_name);
        arduino->open(QSerialPort::ReadOnly);
        arduino->setBaudRate(QSerialPort::Baud9600);
        arduino->setDataBits(QSerialPort::Data8);
        arduino->setFlowControl(QSerialPort::NoFlowControl);
        arduino->setParity(QSerialPort::NoParity);
        arduino->setStopBits(QSerialPort::OneStop);
        QObject::connect(arduino, SIGNAL(readyRead()), this, SLOT(readSerial()));
    }else{
        qDebug() << "Couldn't find the correct port for the arduino.\n";
        QMessageBox::information(this, "Serial Port Error", "Couldn't open serial port to arduino.");
    }
}

Dialog::~Dialog()
{
    if(arduino->isOpen()){
        arduino->close(); //    fecha a porta serial se ela estiver aberta.
    }
    delete ui;
}

void Dialog::readSerial()
{
    /* funcao nao garante que que o valor seja recebido de uma só vez
      a mensagem pode chegar em partes, então os dados precisam ser armazenados no serial e depois
      analizar o valor da temperatura*/

    QStringList buffer_split = serialBuffer.split(","); //  separador decimal

    if(buffer_split.length() < 3){
        // nenhum valor analisado, esta acumulando byts do serial no buffer.
        serialData = arduino->readAll();
        serialBuffer = serialBuffer + QString::fromStdString(serialData.toStdString());
        serialData.clear();
    }else{
        // segundo elemento é analizado para atualizar o valor no LCD
        serialBuffer = "";
        qDebug() << buffer_split << "\n";
        parsed_data = buffer_split[1];
        temperature_value = (9/5.0) * (parsed_data.toDouble()) + 32; // converte para fahrenheit
        qDebug() << "Temperature: " << temperature_value << "\n";
        parsed_data = QString::number(temperature_value, 'g', 4); // formato para temperatura para 4 digitos ou menos
        Dialog::updateTemperature(parsed_data);
    }

}

void Dialog::updateTemperature(QString sensor_reading)
{
    //  atualiza o valor do display
    ui->temp_lcdNumber->display(sensor_reading);
}
