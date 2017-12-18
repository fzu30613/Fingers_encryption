#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "encryption.h"
#include <string.h>
#include <QAction>
#include <QMenuBar>
#include <QMessageBox>
#include <QStatusBar>
#include <QToolBar>
#include <QInputDialog>
#include <QPushButton>
#include <QFileDialog>
#include <QDebug>
#include "server.h"

char* StringToChar(string str)
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
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)//,
{
    setWindowTitle(tr("ENCRYPTION_BETA"));
    QPushButton *dec = new QPushButton(this);
    dec->setStyleSheet("QPushButton{border-image: url(:/G:/课件/软工实践/文件加密/icon/1121302.png);}"
                       "QPushButton:pressed{border-image: url(:/G:/课件/软工实践/文件加密/icon/3.png);}");
    dec->setGeometry(QRect(50,220,100,100));
    connect(dec,&QPushButton::clicked,this,&MainWindow::decrypt);
    QPushButton *enc = new QPushButton(this);
    enc->setStyleSheet("QPushButton{border-image: url(:/G:/课件/软工实践/文件加密/icon/1121301.png);}"
                       "QPushButton:pressed{border-image: url(:/G:/课件/软工实践/文件加密/icon/4.png);}");
    enc->setGeometry(QRect(200,220,100,100));
    connect(enc,&QPushButton::clicked,this,&MainWindow::encrypt);

    openAction = new QAction(QIcon(":/imgs/file.png"),tr("Open.."),this);
    openAction->setShortcuts(QKeySequence::Open);
    openAction->setStatusTip(tr("Open an exist file"));
    connect(openAction,&QAction::triggered,this,&MainWindow::open);
    QMenu *menu = menuBar()->addMenu(tr("&Menu"));
    menu->addAction(openAction);

    pte = new QTextEdit(this);
    pte->setGeometry(QRect(25,80,300,25));
    pte->append(path);

    pgb = new QProgressBar(this);
    pgb->setGeometry(QRect(60,150,250,50));
    pgb->setValue(0);
    pgb->hide();

    /*encryptAction = new QAction(QIcon(":/imgs/close.png"), tr("&Encrypt..."), this);
    encryptAction->setShortcuts(QKeySequence::Open);
    encryptAction->setStatusTip(tr("Encrypt an existing file"));
    connect(encryptAction, &QAction::triggered, this, &MainWindow::encrypt);

    QMenu *file = menuBar()->addMenu(tr("&Open"));
    file->addAction(encryptAction);

    QToolBar *toolBar = addToolBar(tr("&Encrypt"));
    toolBar->addAction(encryptAction);

     decryptAction = new QAction(QIcon(":/imgs/open.png"), tr("&Decrypt..."), this);
     decryptAction->setStatusTip(tr("Decrypt an existing file"));
     connect(decryptAction, &QAction::triggered, this, &MainWindow::decrypt);
     QMenu *file2 = menuBar()->addMenu(tr("&Decrypt"));
     file2->addAction(decryptAction);
     QToolBar *toolBar2 = addToolBar(tr("&Decrypt"));
     toolBar2->addAction(decryptAction);
*/
    statusBar() ;
}
   /* ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}
*/
MainWindow::~MainWindow()
{
   // delete ui;
}

void MainWindow::open()
{
    QFileDialog *fileDialog = new QFileDialog(this);
          fileDialog->setWindowTitle(tr("Open File"));
          fileDialog->setDirectory(".");
          //fileDialog->setFilter(tr("Image Files(*.jpg *.png)"));
          if(fileDialog->exec() == QDialog::Accepted) {
                  path = fileDialog->selectedFiles()[0];
                  QMessageBox::information(NULL, tr("Path"), tr("You selected ") + path);
                  pte->append(path);
          } else {
                  QMessageBox::information(NULL, tr("Path"), tr("You didn't select any files."));
          }
}


void MainWindow::encrypt()
{

    EncryptionAlgorithm enc;
    enc.setChangesize(6);

    bool isOK;
    if (!path.isNull())isOK = true;
    if(isOK)
    {
        pgb->show();
        pgb->setValue(0);
        std::string str;
        str = path.toStdString();
        char *c = (char*)malloc(str.size()*sizeof(char));
        c = StringToChar(str);
        enc.setSourcefile(c);

       /*QMessageBox *msgBox;
          msgBox = new QMessageBox("Waiting...",       ///--这里是设置消息框标题
              "Waiting for finger confirmed...",                       ///--这里是设置消息框显示的内容
              QMessageBox::Warning,                          ///--这里是在消息框显示的图标
              QMessageBox::Ok | QMessageBox::Default,     ///---这里是显示消息框上的按钮情况
              QMessageBox::Cancel | QMessageBox::Escape,  ///---这里与 键盘上的 escape 键结合。当用户按下该键，消息框将执行cancel按钮事件
              0);
       msgBox->show();*/

       TCPServer tcps;
       tcps.Initserver();
       pgb->setValue(10);
       QMessageBox::warning(this,"Waiting..","Waiting for receive",QMessageBox::Yes, QMessageBox::Yes);

       std::string str1;
       str1 = path.toStdString();
       char *c1 = (char*)malloc((str1.size()+5)*sizeof(char));
       c1 = StringToChar(str1);
       c1[0+str1.size()]='-';
       c1[1+str1.size()]='e';
       c1[2+str1.size()]='n';
       c1[3+str1.size()]='c';
       c1[4+str1.size()]='\0';
       enc.setObjectfile(c1);
/*
       enc.setPassword("123");
       enc.setScurecode("123");
*/
       string msg = "encrypt";
       string s1,s2;
       if (tcps.req_one_two(msg,s1,s2) )
       {
           pgb->setValue(30);
           char *cs1,*cs2;
           cs1 = StringToChar(s1);
           cs2 = StringToChar(s2);
           enc.setPassword(cs1);
           enc.setScurecode(cs2);
           enc.encryptFile();
           pgb->setValue(100);
           QMessageBox::about(this,"Finished","File encrypted!");
           tcps.Closeserver();
       }
       pgb->hide();
   }

     /* if(isOK) {
              QMessageBox::information(NULL, "Information",
                                                  "Your comment is: <b>" + text + "</b>",
                                                  QMessageBox::Yes | QMessageBox::No,
                                                  QMessageBox::Yes);
       }*/
}

void MainWindow::decrypt()
{
    EncryptionAlgorithm dec;
    dec.setChangesize(6);
    bool isOK;
    if (!path.isNull())isOK = true;
    if(isOK)
    {
        pgb->show();
        pgb->setValue(0);
        std::string str;
        str = path.toStdString();
        char *c = (char*)malloc(str.size()*sizeof(char));
        c = StringToChar(str);
        dec.setSourcefile(c);

       TCPServer tcp;

       pgb->setValue(10);
       QMessageBox::warning(this,"Waiting..","Waiting for receive",QMessageBox::Yes , QMessageBox::Yes);
       tcp.Initserver();
       std::string str1;
       str1 = path.toStdString();
       char *c1 = (char*)malloc(str1.size()*sizeof(char));
       c1 = StringToChar(str1);
       char *c2 = (char*)malloc((str1.size()-3)*sizeof(char));
       for (int i =0; i<str1.size()-4;i++)
       {
           c2[i]=c1[i];
       }
       c2[str1.size()]='\0';
      dec.setObjectfile(c2);
      string s="decrypt";
      string s1;
      if(tcp.req_one_one(s,s1))
      {
          pgb->setValue(30);
          char *p;
          p = StringToChar(s1);
          dec.setMessage(p);
          dec.decryptFile();
          pgb->setValue(100);
          QMessageBox::about(this,"Finished","File decrypted!");
          tcp.Closeserver();
      }
   }
   pgb->hide();
}
