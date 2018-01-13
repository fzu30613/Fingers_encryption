#ifndef CDIALOG_H
#define CDIALOG_H
#include <QDialog>
#include <QPushButton>
#include <QListWidget>
#include <QDebug>
#include <string.h>
#include <string>
#include "netmanager.h"

class CDialog : public QDialog
{
    Q_OBJECT
public:
    void setIP(std::string s[],int n);
    int connIP();
    int outnum();
    void clk_ref();
    IPManager ip;
    CDialog(QWidget *parent = 0);
    ~CDialog();
private:
    unsigned long ipAddress;
    int num;
    std::string str[256];
    QListWidget *qlw;
    QPushButton *connButton;
    QPushButton *cancelButton;
    QPushButton *refreshButton;
    void connconfirm();
    void refresh();

};


#endif // CDIALOG_H
