#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include <QFileDialog>
#include <QFileSystemModel>
#include <QStandardItem>
#include <QStringList>
#include <QDateTime>
#include <QTextStream>
#include <QFile>
#include <QDir>
#include <QDebug>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);

    int FileState(QFileInfo strPath);

    void GetCheck(QStringList &list);

    ~MainWindow();

    void init_UI();

    void readFileList();

    void saveFileList();

    void filtFolder();

    void write_log(QString type, QString path, QString filename);

private slots:
    void on_GoTopushButton_clicked();

    void on_DirSelectpushButton_clicked();

    void show_file();

    void show_dir();

    void show_ext();

    void settable();

    void on_actionUnlockDir_triggered();

    void on_actionLockDir_triggered();

    void on_actionunlockFile_triggered();

    void on_actionLockFile_triggered();

    void on_actionLockExt_triggered();

    void on_actionUnlockExt_triggered();

    void select_all();

    void on_checkBox_search_all_stateChanged(int);

private:
    Ui::MainWindow *ui;

    int rowcount;

    QStringList FileList,DirList,ExtList;

    QFileInfoList unlockinfolist,lockinfolist,waitlist;

    QFile *log_file;

    QTextStream *out;

    bool search_all;
};

#endif // MAINWINDOW_H
