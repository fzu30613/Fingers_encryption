#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QCheckBox>
#include <QButtonGroup>
#include <QProgressBar>
#include "encryption.h"
#include "netmanager.h"
#include "cdialog.h"

/*namespace Ui {
class MainWindow;
}*/


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
   /* explicit*/ MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    void encrypt();
    void decrypt();
   // QAction *encryptAction;
   // QAction *decryptAction;
    void open();
    void setconnect();
    void setdisconnect();
    void choseFile();
    void choseDire();
    void deleteFile();
    void secure();
    int file;
    int dire;
    int dele;
    QString p[9999];
    int encryption(QString path,int num);
    int decryption(QString path,int num);
    tcpM::TCPManager tcp;
    CDialog *cdlg;
    QAction *connectAction;
    QAction *openAction;
    QAction *disconnectAction;
    QString path;
    QCheckBox *qcb[2];
    QCheckBox *deletefile;
    QProgressBar *pgb;
    //Ui::MainWindow *ui;
};



#endif // MAINWINDOW_H
