#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QAction>
#include <QMenuBar>
#include <QMessageBox>
#include <QStatusBar>
#include <QToolBar>
#include <QInputDialog>
#include <QFileDialog>
#include <QFile>
#include <QCheckBox>



char* stringToChar(std::string str)
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

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)//,
{
    setWindowTitle(tr("ENCRYPTION"));

    dele = 0; file = 1; dire = 0;

    cdlg = new CDialog(this);

    QPushButton *dec = new QPushButton(this);
    dec->setStyleSheet("QPushButton{border-image: url(:/G:/课件/软工实践/文件加密/icon/1121302.png);}"
                       "QPushButton:pressed{border-image: url(:/G:/课件/软工实践/文件加密/icon/3.png);}");
    dec->setGeometry(QRect(50,220,100,100));
    dec->setToolTip("解密");
    connect(dec,&QPushButton::clicked,this,&MainWindow::decrypt);

    QPushButton *enc = new QPushButton(this);
    enc->setStyleSheet("QPushButton{border-image: url(:/G:/课件/软工实践/文件加密/icon/1121301.png);}"
                       "QPushButton:pressed{border-image: url(:/G:/课件/软工实践/文件加密/icon/4.png);}");
    enc->setGeometry(QRect(200,220,100,100));
    enc->setToolTip("加密");
    connect(enc,&QPushButton::clicked,this,&MainWindow::encrypt);

    QPushButton *secure = new QPushButton(this);
    secure->setStyleSheet("QPushButton{border-image: url(:/imgs/secure.png);}"
                       "QPushButton:pressed{border-image: url(:/imgs/secure_1.png);}");
    secure->setGeometry(QRect(150,280,50,50));
    secure->setToolTip("救援码解密");
    connect(secure,&QPushButton::clicked,this,&MainWindow::secure);

    openAction = new QAction(QIcon(":/imgs/file.png"),tr("打开.."),this);
    openAction->setShortcuts(QKeySequence::Open);
    openAction->setStatusTip(tr("打开文件（夹）"));
    connect(openAction,&QAction::triggered,this,&MainWindow::open);
    QMenu *menu = menuBar()->addMenu(tr("菜单"));
    menu->addAction(openAction);

    QButtonGroup * checkgroup;
    int x = 40;QString str_check[]={"选中文件夹","选中文件"};
    checkgroup = new QButtonGroup(this);
    for (int i = 0; i < 2; i++)
    {
        qcb[i] = new QCheckBox(str_check[i],this);
        qcb[i]->setGeometry(QRect(x+50,50,100,20));
        checkgroup->addButton(qcb[i]);       
        x+=100;
    }
    qcb[1]->setChecked(true);
    connect(qcb[1],&QCheckBox::clicked,this,&MainWindow::choseFile);
    connect(qcb[0],&QCheckBox::clicked,this,&MainWindow::choseDire);

    //deletefile = new QCheckBox(tr("删除原文件"),this);
    //deletefile->setGeometry(QRect(140,70,100,20));
    //connect(deletefile,&QCheckBox::clicked,this,&MainWindow::deleteFile);

    connectAction = new QAction(QIcon(":/imgs/connect.png"),tr("建立连接.."),this);
    connectAction->setStatusTip(tr("与手机建立连接"));
    connectAction->setShortcuts(QKeySequence::New);
    connect(connectAction,&QAction::triggered,this,&MainWindow::setconnect);
    menu->addAction(connectAction);

    disconnectAction = new QAction(QIcon(":/imgs/disconnect.png"),tr("断开连接.."),this);
    disconnectAction->setStatusTip("与手机断开连接");
    disconnectAction->setShortcuts(QKeySequence::Save);
    connect(disconnectAction,&QAction::triggered,this,&MainWindow::setdisconnect);
    menu->addAction(disconnectAction);
    disconnectAction->setDisabled(true);

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
          fileDialog->setWindowTitle(tr("打开文件"));
          fileDialog->setDirectory(".");
          if(file == 1 && dire == 0)
          {
              fileDialog->setFileMode(QFileDialog::AnyFile);
          }
          else
          {
              fileDialog->setFileMode(QFileDialog::Directory);
          }
          if(fileDialog->exec() == QDialog::Accepted) {
                  path = fileDialog->selectedFiles()[0];
                  QMessageBox::information(NULL, tr("Path"), tr("你选择了") + path);
          }
          else {
                  QMessageBox::information(NULL, tr("Path"), tr("你没有选择文件"));
          }
}

void MainWindow::setconnect()
{
    cdlg->ip.refresh();
    int num;
    std::string *str;
    str = cdlg->ip.getIPList(num);
    cdlg->setIP(str,num);
    cdlg->setFixedSize(350,350);
    cdlg->update();
    cdlg->clk_ref();
    if (cdlg->exec() == QDialog::Accepted)
    {
       // qDebug()<<cdlg->connIP();
        tcp.TCPconnect(cdlg->connIP());
        if (tcp.isConnect())
        {
            QMessageBox::information(NULL,tr("提示"),tr("连接建立！"));
            cdlg->close();
            connectAction->setDisabled(true);
            disconnectAction->setEnabled(true);
        }
        else
        {
            QMessageBox::warning(NULL,tr("警告"),tr("连接建立失败！"));
            cdlg->close();
        }

    }
    else
    {
        QMessageBox::information(NULL,tr("提示"),tr("未选择连接设备!"));
    }
}

void MainWindow::setdisconnect()
{
    tcp.TCPdisconnect();
    if (tcp.isConnect())
    {
        QMessageBox::warning(NULL,tr("警告"),tr("断开连接失败！"));
    }
    else
    {
        QMessageBox::information(NULL,tr("提示"),tr("连接断开！"));
        connectAction->setEnabled(true);
        disconnectAction->setDisabled(true);
    }
}

void MainWindow::choseFile()
{
    file = 1;
    dire = 0;
}

void MainWindow::choseDire()
{
    file = 0;
    dire = 1;
}

void MainWindow::deleteFile()
{
    dele = (dele+1)%2;
}

int MainWindow::encryption(QString path,int num)
{
    QDir dir(path);
    dir.setFilter(QDir::Dirs | QDir::Hidden);
    QFileInfoList list =dir.entryInfoList();
    for (int i = 2; i < list.size(); i++)
    {
        QFileInfo fileInfo = list.at(i);
        num = encryption(fileInfo.absoluteFilePath(),num);
    }
    //qDebug() << path << ":";
    dir.setFilter(QDir::Files | QDir::Hidden);
    list = dir.entryInfoList();
    for (int i = 0; i < list.size(); i++)
    {
        QFileInfo fileInfo = list.at(i);
        p[num++]=fileInfo.filePath();
    }
    return num;
};

int MainWindow::decryption(QString path,int num)
{
    QDir dir(path);
    dir.setFilter(QDir::Dirs | QDir::Hidden);
    QFileInfoList list =dir.entryInfoList();
    for (int i = 2; i < list.size(); i++)
    {
        QFileInfo fileInfo = list.at(i);
        num = decryption(fileInfo.absoluteFilePath(),num);
    }
   // qDebug() << path << ":";
    dir.setFilter(QDir::Files | QDir::Hidden);
    list = dir.entryInfoList();
    for (int i = 0; i < list.size(); i++)
    {
        QFileInfo fileInfo = list.at(i);
        p[num++]=fileInfo.filePath();
    }
    return num;
};

void MainWindow::encrypt()
{

   /* EncryptionAlgorithm enc;
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

     /*  TCPServer tcps;
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

       std::string msg = "encrypt";
       std::string s1,s2;
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
   }*/

     /* if(isOK) {
              QMessageBox::information(NULL, "Information",
                                                  "Your comment is: <b>" + text + "</b>",
                                                  QMessageBox::Yes | QMessageBox::No,
                                                  QMessageBox::Yes);
       }*/


    if (path.isEmpty())
    {
        QMessageBox::information(NULL,tr("提示"),tr("未选择文件！"));
        return;
    }
    if (tcp.isConnect()){
   std::string psw1,sec1;
   tcp.requestEncrypt(psw1,sec1);
  // qDebug() <<QString::fromStdString(psw1);
   if (psw1.empty() || sec1.empty())
   {
        QMessageBox::warning(NULL,tr("警告"),tr("信息传输失败"));
       return ;
   }
   char * psw, *sec;
   psw= (char*)malloc(sizeof(char)*psw1.length());
   sec= (char*)malloc(sizeof(char)*sec1.length());
   psw = stringToChar(psw1);
   sec = stringToChar(sec1);
   if (dire == 1 && file ==0)
   {
       int n=0;
       int num = encryption(path,0);
       for (int i = 0; i < num; i++)
       {
             EncryptionAlgorithm enc;
             enc.setChangesize(16);
             enc.setPassword(psw);
             enc.setScurecode(sec);
             char *c = (char*)malloc(p[i].toStdString().length()*sizeof(char));
             c = stringToChar(p[i].toStdString());
             char *c1 = (char*)malloc((p[i].toStdString().length()+5)*sizeof(char));
             c1 = stringToChar(p[i].toStdString());
             c1[0+p[i].toStdString().length()]='-';
             c1[1+p[i].toStdString().length()]='e';
             c1[2+p[i].toStdString().length()]='n';
             c1[3+p[i].toStdString().length()]='c';
             c1[4+p[i].toStdString().length()]='\0';
             enc.setSourcefile(c);
             enc.setObjectfile(c1);
             if (enc.encryptFile() == 0)n++;
        }
       if (n!=num)
       {
           QMessageBox::information(NULL,tr("提示"),tr("加密")+QString::number(num-n)+tr("个文件失败！"));
       }
        QMessageBox::information(NULL,tr("提示"),tr("成功加密")+QString::number(n)+tr("个文件！"));
   }
   else
   {
       EncryptionAlgorithm enc;
       enc.setChangesize(16);
       enc.setPassword(psw);
       enc.setScurecode(sec);
       char *c = (char*)malloc(path.toStdString().length()*sizeof(char));
       c = stringToChar(path.toStdString());
       char *c1 = (char*)malloc((path.toStdString().length()+6)*sizeof(char));
       c1 = stringToChar(path.toStdString());
       c1[0+path.toStdString().length()]='-';
       c1[1+path.toStdString().length()]='e';
       c1[2+path.toStdString().length()]='n';
       c1[3+path.toStdString().length()]='c';
       c1[4+path.toStdString().length()]='\0';
      // qDebug() << c;
       //qDebug() << c1;
       enc.setSourcefile(c);
       enc.setObjectfile(c1);
       enc.encryptFile();
       QMessageBox::information(NULL,tr("提示"),tr("成功加密1个文件！"));
   }
    }
    else
    {
        QMessageBox::warning(NULL,tr("警告"),tr("连接断开!"));
    }
}

void MainWindow::decrypt()
{
   /* EncryptionAlgorithm dec;
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

       /*TCPServer tcp;

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
   pgb->hide();*/


    if (path.isEmpty())
    {
        QMessageBox::information(NULL,tr("提示"),tr("未选择文件！"));
        return;
    }
    if (tcp.isConnect()){
    std::string mesg;
    tcp.requestDecrypt(mesg);
    if (mesg.empty() || mesg == "$")
    {
        QMessageBox::warning(NULL,tr("警告"),tr("信息传输失败"));
       return ;
    }
    char * msg;
    msg=(char*)malloc(sizeof(char)*mesg.length());
    msg=stringToChar(mesg);
    if (file == 0 && dire == 1)
    {
        int n = 0;
        int num = decryption(path,0);
        for (int i = 0; i < num; i++)
        {
        EncryptionAlgorithm enc;
        enc.setChangesize(16);
        enc.setMessage(msg);
        char *c = (char*)malloc(p[i].toStdString().length()*sizeof(char));
        c = stringToChar(p[i].toStdString());
        char *c1 = (char*)malloc((p[i].toStdString().length())*sizeof(char));
        for (int j =0; j<p[i].toStdString().length()-4;j++)
            {
            c1[j]=c[j];
            }
        c1[p[i].toStdString().length()-4]='\0';
        enc.setSourcefile(c);
        enc.setObjectfile(c1);
        if(enc.decryptFile(1)==0)n++;
        }
         QMessageBox::information(NULL,tr("提示"),tr("成功解密")+QString::number(n)+tr("个文件！"));
    }
    else
    {
        EncryptionAlgorithm enc;
        enc.setChangesize(16);
        enc.setMessage(msg);
      //  qDebug() << QString::fromStdString(mesg);
        char *c = (char*)malloc(path.toStdString().length()*sizeof(char));
        c = stringToChar(path.toStdString());
        char *c1 = (char*)malloc((path.toStdString().length())*sizeof(char));
        for (int j =0; j<path.toStdString().length()-4;j++)
        {
            c1[j]=c[j];
        }
        c1[path.toStdString().length()-4]='\0';
        enc.setSourcefile(c);
        enc.setObjectfile(c1);
        int res = enc.decryptFile(1);
        if (res ==0)
        QMessageBox::information(NULL,tr("提示"),tr("成功解密1个文件！"));
        else
        {
            if (res == -1)
            QMessageBox::information(NULL,tr("提示"),tr("解密失败:打开文件失败"));
            else if (res == -3)
            {
                QMessageBox::information(NULL,tr("提示"),tr("解密失败:验证失败"));
            }
            else if (res == -2)
            {
                QMessageBox::information(NULL,tr("提示"),tr("解密失败:救援码失败"));
            }
        }
    }
    }
    else
    {
        QMessageBox::warning(NULL,tr("警告"),tr("连接断开！"));
    }
}

void MainWindow::secure()
{
    if (path.isEmpty())
    {
        QMessageBox::information(NULL,tr("提示"),tr("未选择文件！"));
    }
    else
    {
        QMessageBox::information(NULL,tr("提示"),tr("已选择文件：")+path);
        bool ok;
        QString text = QInputDialog::getText(this, tr("救援代码输入"),
                                         tr("救援代码:"), QLineEdit::Normal,
                                         NULL,&ok);
        if (ok && !text.isEmpty())
        {
            if (file == 0 && dire == 1)
            {
                int res,n=0;
                int num = decryption(path,0);
                for (int i = 0; i < num; i++)
                {
                EncryptionAlgorithm enc;
                enc.setChangesize(16);
                char *m = (char*)malloc(text.toStdString().size()*sizeof(char));
                m = stringToChar(text.toStdString());
                enc.setMessage(m);
                char *c = (char*)malloc(p[i].toStdString().length()*sizeof(char));
                c = stringToChar(p[i].toStdString());
                char *c1 = (char*)malloc((p[i].toStdString().length())*sizeof(char));
                for (int j =0; j<p[i].toStdString().length()-4;j++)
                    {
                    c1[j]=c[j];
                    }
                c1[p[i].toStdString().length()-4]='\0';
                enc.setSourcefile(c);
                enc.setObjectfile(c1);
                res = enc.decryptFile(2);
                if (res == 0)
                {
                    n++;
                }
                }
                if (res==0)
                {
                    path.clear();
                    if (path.isEmpty())
                    QMessageBox::information(NULL,tr("提示"),tr("解密")+QString::number(n)+tr("个文件成功！"));
                    return;
                }
            }
            else
            {
            EncryptionAlgorithm enc;
            enc.setChangesize(16);
            char *sp1 = (char*)malloc(path.toStdString().size()*sizeof(char));
            sp1 = stringToChar(path.toStdString());
            enc.setSourcefile(sp1);
            char *sp2 = (char*)malloc((path.toStdString().size()-3)*sizeof(char));
            for (int i = 0; i < path.toStdString().size()-4; i++)
            {
                sp2[i]=sp1[i];
            }
            sp2[ path.toStdString().size()-4]='\0';
            enc.setObjectfile(sp2);
            char *c = (char*)malloc(text.toStdString().size()*sizeof(char));
            c = stringToChar(text.toStdString());
            enc.setMessage(c);
            int res = enc.decryptFile(2);
            if (res==0)
            {
                path.clear();
                if (path.isEmpty())
                QMessageBox::information(NULL,tr("提示"),tr("解密成功！"));
                return;
            }
            else if (res==-2)
            {
                 QMessageBox::warning(NULL,tr("警告"),tr("救援代码错误!"));
                 return;
            }
            else if (res==-1)
            {
                 QMessageBox::warning(NULL,tr("警告"),tr("打开文件失败!"));
                 return;
            }
            }
        }
    }
}
