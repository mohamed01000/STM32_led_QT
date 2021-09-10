#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "qmessagebox.h"
#include "QtSerialPort/QSerialPort"

#define nomport "COM7"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    portserie = new QSerialPort(this) ;
    portserie->setPortName(nomport);
    portserie->setBaudRate(QSerialPort::Baud9600);
    portserie->setDataBits(QSerialPort::Data8);
    portserie->setParity(QSerialPort::NoParity);
    portserie->setStopBits(QSerialPort::OneStop);
    portserie->setFlowControl(QSerialPort::NoFlowControl);
     if (portserie->open(QIODevice::ReadWrite))
     {
     QMessageBox::information(this, "Com ouverte","connexion OK sur "+QString(nomport));
     portserie->write("Communications etablies\n");
     }
     else
     {
     // probleme d'ouverture du port serie
     QMessageBox::critical(this, "Erreur sur port "+QString(nomport), portserie->errorString());
     exit(1);
     }
}



MainWindow::~MainWindow()
{

     if (portserie->isOpen()) portserie->close();
    delete ui;
}


void MainWindow::on_rbLED_clicked()
{

     QString chargeUtile;
     if (ui->rbLED->isChecked()==true) chargeUtile="LD1\n\r";
     else chargeUtile="LD0\n\r";
     portserie->write(chargeUtile.toLocal8Bit());
     ui->lblInfos->setText(chargeUtile);

     if(portserie->open(QIODevice::ReadWrite))
      {
      QMessageBox::information(this, "Com ouverte","connexion OK sur "+QString(nomport));
      // si un caractere est arrive on l'ecrit
      connect(portserie, SIGNAL(readyRead()), this, SLOT(litData()));
      portserie->write("Communications etablies\n");
      ui->lblInfos->setText("Connecte sur "+QString(nomport));
      }
}


