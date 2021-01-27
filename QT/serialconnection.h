#ifndef SERIALCONNECTION_H
#define SERIALCONNECTION_H

#include <QDialog>
#include <QSerialPort>
#include "textEditor.h"

QT_BEGIN_NAMESPACE

class QLabel;
class QLineEdit;
class QSpinBox;
class QPushButton;
class QComboBox;

QT_END_NAMESPACE

class serialconnection : public QDialog
{
    Q_OBJECT
public:
    explicit serialconnection(QWidget *parent = nullptr);

private slots:
    void serialconnect();
    void refresh_clicked();
    void checkComPortStatus();
//    void processError(const QString &s);
//    void processTimeout(const QString &s);

private:
    void setControlsEnabled(bool enable);


private:
    int transactionCount = 0;
    QLabel *serialPortLabel = nullptr;
    QComboBox *serialPortComboBox = nullptr;
    QPushButton *runButton = nullptr;
    QPushButton *refreshButton = nullptr;
    QSerialPort *serial = nullptr;
    textEditor *editor = nullptr;
    QTimer *timer;
};

#endif // SERIALCONNECTION_H
