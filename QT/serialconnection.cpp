#include "serialconnection.h"
#include <QComboBox>
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QSerialPortInfo>
#include <QSpinBox>
#include <QMessageBox>
#include <QTimer>
#include<QDebug>

serialconnection::serialconnection(QWidget *parent) :
    QDialog(parent),
    serialPortLabel(new QLabel(tr("Serial port:"))),
    serialPortComboBox(new QComboBox),
    runButton(new QPushButton(tr("Connect"))),
    refreshButton(new QPushButton())
{
    const auto infos = QSerialPortInfo::availablePorts();
    for (const QSerialPortInfo &info : infos)
        serialPortComboBox->addItem(info.portName() + " " +info.description());

    auto mainLayout = new QGridLayout;
    mainLayout->addWidget(serialPortLabel, 0, 0);
    mainLayout->addWidget(serialPortComboBox, 0, 1);
    refreshButton->setIcon(QIcon(":/images/refresh.png"));
    mainLayout->addWidget(refreshButton,0,2);
    mainLayout->addWidget(runButton, 1, 1);
    setLayout(mainLayout);

    setWindowTitle(tr("GTU"));
    setWindowIcon(QIcon(":/images/logo.png"));

    runButton->setFocus();

    connect(runButton, &QPushButton::clicked, this, &serialconnection::serialconnect);
    connect(refreshButton, &QPushButton::clicked, this, &serialconnection::refresh_clicked);
}

void serialconnection::serialconnect()
{
    QMessageBox messageBox;
    QString currentPortName = serialPortComboBox->currentText();

    if (currentPortName.isEmpty()) {
        messageBox.critical(0,"Error","No port name specified !");
        messageBox.setFixedSize(500,200);
        return;
    }
    QString com_port = serialPortComboBox->currentText().split(" ")[0];
    serial = new QSerialPort();
    serial->setPortName(com_port);
    serial->setBaudRate(QSerialPort::Baud9600);

    if (!serial->open(QIODevice::ReadWrite)) {
        messageBox.critical(0,"Error",(tr("Can't open %1, error code %2")
                              .arg(com_port).arg(serial->error())));
        messageBox.setFixedSize(500,200);
        return;
    }
    // timer call checkComPortStatus function in every 1 sec.
    // Then function will check com port status (still connected or not)
    timer = new QTimer(this);
    QObject::connect(timer, SIGNAL(timeout()), this, SLOT(checkComPortStatus()));
    timer->start(1000); //time specified in ms
    hide();
    if(editor == nullptr){
        editor = new textEditor(serial);
        editor->show();
    }
    else{
        editor->updateSerial(serial);
        editor->setEnabled(true);
    }
}

void serialconnection::setControlsEnabled(bool enable)
{
    runButton->setEnabled(enable);
    serialPortComboBox->setEnabled(enable);
}
void serialconnection::refresh_clicked(){

    serialPortComboBox->clear();
    const auto infos = QSerialPortInfo::availablePorts();
    for (const QSerialPortInfo &info : infos)
        if(info.isValid())
            serialPortComboBox->addItem(info.portName() + " " +info.description());
}
void serialconnection::checkComPortStatus()
{
    QMessageBox messageBox;
    QSerialPortInfo *info = new QSerialPortInfo;
    QList<QSerialPortInfo> list = info->availablePorts();
    bool port_still_available = false;

    for (QSerialPortInfo &port : list) {
        if (port.portName() == serial->portName() && port.isValid() == true)
            port_still_available = true;
    }
    delete info;
    if(port_still_available == false){
         refresh_clicked();
         show();
         editor->setEnabled(false);
         timer->stop();
         messageBox.critical(0,"Error","Connection lost!!!");
         messageBox.setFixedSize(500,200);
    }
}
