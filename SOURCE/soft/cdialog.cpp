#include "cdialog.h"

char* StringToChar(std::string str)
{
    int length=str.size();
    char* p=(char*)malloc(sizeof(char)*length);
    int i;
    for (i=0;i<length;i++)
    {
        p[i]=str[i];
    }
    p[i]=str[i];
    return p;
};

CDialog::CDialog(QWidget *parent):QDialog(parent)
{
    qlw = new QListWidget(this);
    qlw->resize(350,300);
    qlw->show();
    connButton = new QPushButton(this);
    connButton->setGeometry(QRect(40,300,50,50));
    connButton->setStyleSheet("QPushButton{border-image: url(:/imgs/checkmark_1.png);}"
                              "QPushButton:pressed{border-image: url(:/imgs/checkmark_2.png);}");
    connect(connButton,&QPushButton::clicked,this,&CDialog::connconfirm);

    cancelButton = new QPushButton(this);
    cancelButton->setGeometry(QRect(260,300,50,50));
    cancelButton->setStyleSheet("QPushButton{border-image: url(:/imgs/cross_1.png);}"
                                "QPushButton:pressed{border-image: url(:/imgs/cross_2.png);}");
    connect(cancelButton,&QPushButton::clicked,this,&CDialog::close);

    refreshButton = new QPushButton(this);
    refreshButton->setGeometry(QRect(150,300,50,50));
    refreshButton->setStyleSheet("QPushButton{border-image: url(:/imgs/reload_1.png);}"
                                 "QPushButton:pressed{border-image: url(:/imgs/reload_2.png);}");
    connect(refreshButton,&QPushButton::clicked,this,&CDialog::refresh);
}

void CDialog::connconfirm()
{
    num = qlw->currentRow();
    ipAddress = ip.getIPbyRow(num);
    QDialog::accept();
}

void CDialog::refresh()
{
    ip.refresh();
    std::string *s = ip.getIPList(num);
    setIP(s,num);
    qlw = new QListWidget(this);
    for (int i = 0; i < CDialog::num; i++)
    {
        char *c=StringToChar(str[i]);
        qlw->addItem(new QListWidgetItem(QIcon(""), tr(c)));
    }
    qlw->resize(350,300);
    qlw->show();
}

void CDialog::setIP(std::string s[256],int n)
{
    num = n;
    for (int i = 0; i < n; i++)
    {
        str[i]=s[i];
    }
}

int CDialog::connIP()
{
    return ipAddress;
}

int CDialog::outnum()
{
    return num;
}

void CDialog::clk_ref()
{
    refreshButton->click();
}

CDialog::~CDialog()
{

}
