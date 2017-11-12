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
    setWindowTitle(tr("Main Window"));

    encryptAction = new QAction(QIcon(":/imgs/close.png"), tr("&Encrypt..."), this);
   // encryptAction->setShortcuts(QKeySequence::Open);
    encryptAction->setStatusTip(tr("Encrypt an existing file"));
    connect(encryptAction, &QAction::triggered, this, &MainWindow::encrypt);

    QMenu *file = menuBar()->addMenu(tr("&Encrypt"));
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

void MainWindow::encrypt()
{

    EncryptionAlgorithm enc;
    enc.setChangesize(6);
    bool isOK;
    QString text = QInputDialog::getText(this, "Input",
                                           "Please input filename",
                                           //  QLineEdit::Password,    //输入的是密码，不显示明文
                                           QLineEdit::Normal,          //输入框明文
                                           NULL,
                                           &isOK);
    if(isOK)
    {

        std::string str;
        str = text.toStdString();
        char *c = (char*)malloc(str.size()*sizeof(char));
        c = StringToChar(str);
        enc.setSourcefile(c);
    }
    bool isOK1;
    QString text1 = QInputDialog::getText(this, "Output",
                                           "Please input filename",
                                           //  QLineEdit::Password,    //输入的是密码，不显示明文
                                           QLineEdit::Normal,          //输入框明文
                                           NULL,
                                           &isOK1);
   if(isOK1)
   {
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
       QMessageBox::warning(this,"Waiting..","Waiting for receive",QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
       std::string str1;
       str1 = text1.toStdString();
       char *c1 = (char*)malloc(str1.size()*sizeof(char));
       c1 = StringToChar(str1);
       enc.setObjectfile(c1);
/*
       enc.setPassword("123");
       enc.setScurecode("123");
*/
       string msg = "encrypt";
       string s1,s2;
       if (tcps.req_one_two(msg,s1,s2) )
       {
           char *cs1,*cs2;
           cs1 = StringToChar(s1);
           cs2 = StringToChar(s2);
           enc.setPassword(cs1);
           enc.setScurecode(cs2);
           enc.encryptFile();
           QMessageBox::about(this,"Finished","File encrypted!");
           tcps.Closeserver();
       }
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
    QString text = QInputDialog::getText(this, "Input",
                                           "Please input filename",
                                           //  QLineEdit::Password,    //输入的是密码，不显示明文
                                           QLineEdit::Normal,          //输入框明文
                                           NULL,
                                           &isOK);
    if(isOK)
    {
        std::string str;
        str = text.toStdString();
        char *c = (char*)malloc(str.size()*sizeof(char));
        c = StringToChar(str);
        dec.setSourcefile(c);
    }
    bool isOK1;
    QString text1 = QInputDialog::getText(this, "Output",
                                           "Please input filename",
                                           //  QLineEdit::Password,    //输入的是密码，不显示明文
                                           QLineEdit::Normal,          //输入框明文
                                           NULL,
                                           &isOK1);
   if(isOK1)
   {
       TCPServer tcp;
       tcp.Initserver();
       QMessageBox::warning(this,"Waiting..","Waiting for receive",QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
       std::string str1;
       str1 = text1.toStdString();
       char *c1 = (char*)malloc(str1.size()*sizeof(char));
       c1 = StringToChar(str1);
      dec.setObjectfile(c1);
      string s="decrypt";
      string s1;
      if(tcp.req_one_one(s,s1))
      {
          char *p;
          p = StringToChar(s1);
          dec.setMessage(p);
          dec.decryptFile();
          QMessageBox::about(this,"Finished","File decrypted!");
          tcp.Closeserver();
      }

   }
}
