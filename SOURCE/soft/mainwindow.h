#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTextEdit>
#include <QProgressBar>


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
    QAction *openAction;
    QString path;
    QTextEdit *pte;
    QProgressBar *pgb;
    //Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
