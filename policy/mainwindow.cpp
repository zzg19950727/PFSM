#include "mainwindow.h"
#include "ui_mainwindow.h"
#define PATH    "C:/list.txt"
#define LOG     "C:/change.txt"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    rowcount=0;

    search_all = false;

    init_UI();

    readFileList();

    log_file = new QFile(LOG);

    if( log_file->open(QIODevice::WriteOnly | QIODevice::Text) )
        out = new QTextStream(log_file);
}

MainWindow::~MainWindow()
{
    saveFileList();
    log_file->close();
    delete log_file;
    delete out;
    delete ui;
}

void MainWindow::init_UI()
{
    this->setWindowTitle("策略配置");

    ui->comboBox_filetype->addItem(QIcon(":icon/unlock_file.ico"),"未锁定文件");
    ui->comboBox_filetype->addItem(QIcon(":icon/lock_file.ico"),"已锁定文件");
    ui->comboBox_filetype->addItem(QIcon(":icon/dir_lock.ico"),"已锁定文件夹");
    ui->comboBox_filetype->addItem(QIcon(":icon/lock_ext.ico"),"已锁定文件后缀");
    connect(ui->comboBox_filetype, SIGNAL(currentIndexChanged(QString)),
            this, SLOT(settable()) );

    ui->comboBox_filter->addItem("*.*");
    ui->comboBox_filter->addItem(".txt");
    ui->comboBox_filter->addItem(".doc");
    ui->comboBox_filter->addItem(".xls");
    ui->comboBox_filter->addItem(".ppt");
    ui->comboBox_filter->addItem(".png");
    ui->comboBox_filter->addItem(".jpg");
    connect(ui->comboBox_filter, SIGNAL(currentTextChanged(QString)),
            this, SLOT(settable()) );
}

void MainWindow::readFileList()
{
    QFile file(PATH);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug()<<"open file failed";
    }
    else
    {
        QTextStream in(&file);
        QString sss;
        int flag=0;
        while(!in.atEnd())
        {
           sss=in.readLine();
           if(sss=="<EXT>")
           {
               flag=2;
               continue;
           }
           else if(sss=="<FILE>")
           {
               flag=1;
               continue;
           }
           else if(sss == "<DIR>")
           {
               flag=0;
               continue;
           }

           sss.replace("\\","/");


           if(flag == 2)
               ExtList<<sss;
           else if(flag == 1)
               FileList<<sss;
           else
               DirList<<sss;
        }
        file.close();
    }

}

void MainWindow::saveFileList()
{
    QFile file(PATH);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        qDebug()<<"open file failed";
    }
    else
    {
        QTextStream out(&file);
        out<<"<DIR>"<<endl;
        for(int i=0;i<DirList.size();i++)
        {
            if(DirList[i].endsWith('/'))
                DirList[i].remove(DirList[i].length()-1,1);
            DirList[i].replace("/","\\");
            out<<DirList[i]<<endl;
        }
        out<<"<FILE>"<<endl;
        for(int i=0;i<FileList.size();i++)
        {
            FileList[i].replace("/","\\");
            out<<FileList[i]<<endl;
        }
        out<<"<EXT>";
        for(int i=0;i<ExtList.size();i++)
        {
            ExtList[i].replace("/","\\");
            out<<endl<<ExtList[i];
        }
        file.close();
    }

}

void MainWindow::filtFolder()
{
    if(ui->comboBox_filetype->currentIndex() == 2)
    {

    }

    QString path = ui->pathlineEdit->text();
    QDir dir( path );
    if(!dir.exists() || path.isEmpty() )
    {
        return;
    }
    QFileInfoList list = dir.entryInfoList();

    int file_count = list.count();
    if(file_count <= 0)
    {
        return;
    }

    QString filters;

    if( ui->comboBox_filter->currentText() != "*.*" )
        filters = ui->comboBox_filter->currentText();

    QDirIterator *iter;
    if(search_all)
       iter =  new QDirIterator(dir,QDirIterator::Subdirectories);
    else
       iter =  new QDirIterator(dir,QDirIterator::NoIteratorFlags);

    unlockinfolist.clear();
    lockinfolist.clear();
    waitlist.clear();

    while (iter->hasNext())
    {
        iter->next();
        QFileInfo info=iter->fileInfo();
        if (info.isFile() && info.fileName().contains(filters))
        {
            int state = FileState(info);
            int index = ui->comboBox_filetype->currentIndex();
            if(state==index)
                unlockinfolist<<info;
        }
    }
    delete iter;
}

void MainWindow::write_log(QString type, QString path, QString filename)
{
    if(!log_file->isOpen())
        return;
    if(path.endsWith('/'))
        path.remove(path.length()-1,1);
    path.replace("/","\\");
    *out<<type<<"\n"<<path<<"\n"<<filename<<"\n";
}

int MainWindow::FileState(QFileInfo strPath)
{
    /*0--明文  1--密文  2--待加密*/
    if(QDir::match(FileList,strPath.absoluteFilePath()))
        return 1;

    for(int i=0;i<DirList.size();i++)
    {
        if(strPath.path().indexOf(DirList[i])!=-1)
            return 1;
    }
    return 0;
}

void MainWindow::settable()
{
    ui->tableView->clearSpans();
    int index = ui->comboBox_filetype->currentIndex();
    switch(index)
    {
    case 0:
        show_file();
        break;
    case 1:
        show_file();
        break;
    case 2:
        show_dir();
        break;
    case 3:
        show_ext();
        break;
    }
}

void MainWindow::GetCheck(QStringList& list)
{
    list.clear();
    for(int i=0;i<rowcount;i++)
    {
      if(ui->tableView->model()->index(i,4).data(Qt::CheckStateRole) == Qt::Checked )
      {
          list<<unlockinfolist[i].absoluteFilePath();
      }
    }
}

void MainWindow::on_GoTopushButton_clicked()
{
    QString path = ui->pathlineEdit->text();
    QDir dir( path );
    if(!dir.exists() || path.isEmpty() )
    {
        return;
    }
    settable();
}

void MainWindow::on_DirSelectpushButton_clicked()
{
    QString dir = QFileDialog::getExistingDirectory(this);
    if(!dir.isEmpty())
    {
        ui->pathlineEdit->setText(dir);
        settable();
    }
}

void MainWindow::show_file()
{
    filtFolder();

    int count=0;

    //准备数据模型
    QStandardItemModel *model = new QStandardItemModel();
    model->setHorizontalHeaderItem(0, new QStandardItem(QObject::tr("文件名")));
    model->setHorizontalHeaderItem(1, new QStandardItem(QObject::tr("最后访问时间")));
    model->setHorizontalHeaderItem(2, new QStandardItem(QObject::tr("创建时间")));
    model->setHorizontalHeaderItem(3, new QStandardItem(QObject::tr("大小")));
    model->setHorizontalHeaderItem(4, new QStandardItem(QObject::tr("选择")));

    //利用setModel()方法将数据模型与QTableView绑定
    ui->tableView->resizeColumnsToContents();

    ui->tableView->setModel(model);

    //设置选中时为整行选中
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);

    //设置表格的单元为只读属性，即不能编辑
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    for(int i=0;i<unlockinfolist.size();i++)
    {
        model->setItem(count,0,new QStandardItem(unlockinfolist[i].fileName()));
        model->setItem(count,1,new QStandardItem(unlockinfolist[i].lastModified().toString("yyyy-MM-dd")));
        model->setItem(count,2,new QStandardItem(unlockinfolist[i].created().toString("yyyy-MM-dd")));
        model->setItem(count,3,new QStandardItem(QString::number(unlockinfolist[i].size())));
        QStandardItem* item0 = new QStandardItem();

        item0->setCheckable(true);
        model->setItem(count,4,item0);
        count++;
    }
    rowcount=count;
}

void MainWindow::show_dir()
{
    int count=0;

    //准备数据模型
    QStandardItemModel *model = new QStandardItemModel();
    model->setHorizontalHeaderItem(0, new QStandardItem(QObject::tr("文件夹")));
    model->setHorizontalHeaderItem(1, new QStandardItem(QObject::tr("选择")));

    //利用setModel()方法将数据模型与QTableView绑定
    ui->tableView->resizeColumnsToContents();

    ui->tableView->setModel(model);

    //设置选中时为整行选中
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);

    //设置表格的单元为只读属性，即不能编辑
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    for(int i=0;i<DirList.size();i++)
    {
        model->setItem(count,0,new QStandardItem(DirList[i]));
        QStandardItem* item0 = new QStandardItem();

        item0->setCheckable(true);
        model->setItem(count,1,item0);
        count++;
    }
    rowcount=count;
}

void MainWindow::show_ext()
{
    int count=0;

    //准备数据模型
    QStandardItemModel *model = new QStandardItemModel();
    model->setHorizontalHeaderItem(0, new QStandardItem(QObject::tr("文件后缀")));
    model->setHorizontalHeaderItem(1, new QStandardItem(QObject::tr("选择")));

    //利用setModel()方法将数据模型与QTableView绑定
    ui->tableView->resizeColumnsToContents();

    ui->tableView->setModel(model);

    //设置选中时为整行选中
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);

    //设置表格的单元为只读属性，即不能编辑
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    for(int i=0;i<ExtList.size();i++)
    {
        model->setItem(count,0,new QStandardItem(ExtList[i]));
        QStandardItem* item0 = new QStandardItem();

        item0->setCheckable(true);
        model->setItem(count,1,item0);
        count++;
    }
    rowcount=count;
}

void MainWindow::on_actionUnlockDir_triggered()
{
    if(ui->comboBox_filetype->currentIndex() != 2)
        return;
    QStringList List;
    for(int i=0;i<rowcount;i++)
    {
      if(ui->tableView->model()->index(i,1).data(Qt::CheckStateRole) == Qt::Checked )
      {
          List<<ui->tableView->model()->index(i,0).data().toString();
      }
    }

    for(int i=0; i<List.size(); i++)
    {
           DirList.removeAll(List[i]);
           write_log("0",List[i],"DIR");
    }

    DirList.removeDuplicates();

    QMessageBox::information(this, "通知", "解锁成功！");

    settable();
    saveFileList();
}

void MainWindow::on_actionLockDir_triggered()
{
    QString path = ui->pathlineEdit->text();
    QDir dir( path );
    if(!dir.exists() || path.isEmpty() )
    {
        return;
    }
    DirList<<ui->pathlineEdit->text();
    write_log("1",ui->pathlineEdit->text(),"DIR");
    DirList.removeDuplicates();

    settable();
    saveFileList();
}

void MainWindow::on_actionunlockFile_triggered()
{
    if(ui->comboBox_filetype->currentIndex() != 1)
        return;
    QStringList List;
    for(int i=0;i<rowcount;i++)
    {
      if(ui->tableView->model()->index(i,4).data(Qt::CheckStateRole) == Qt::Checked )
      {
          List<<unlockinfolist[i].absoluteFilePath();
          write_log("0",unlockinfolist[i].absolutePath(),unlockinfolist[i].fileName());
      }
    }


    for(int i=0; i<List.size(); i++)
    {
           FileList.removeAll(List[i]);

    }

    FileList.removeDuplicates();

    QMessageBox::information(this, "通知", "解锁成功！");

    settable();
    saveFileList();
}

void MainWindow::on_actionLockFile_triggered()
{
    if(ui->comboBox_filetype->currentIndex() != 0)
        return;
    QStringList List;
    for(int i=0;i<rowcount;i++)
    {
      if(ui->tableView->model()->index(i,4).data(Qt::CheckStateRole) == Qt::Checked )
      {
          List<<unlockinfolist[i].absoluteFilePath();
          write_log("1",unlockinfolist[i].absolutePath(),unlockinfolist[i].fileName());
      }
    }


    for(int i=0; i<List.size(); i++)
    {
           FileList<<List[i];
    }

    FileList.removeDuplicates();

    QMessageBox::information(this, "通知", "锁定成功！");

    settable();
    saveFileList();
}

void MainWindow::on_actionLockExt_triggered()
{
    QString ext = ui->comboBox_filter->currentText();
    if( ext == "*.*" || ext.isEmpty() )
        return;
    if(ext[0] != '.' || ext.length()<2)
    {
        QMessageBox::information(this, "通知", "未知的文件类型！");
        return;
    }

    ExtList<<ext;
    write_log("1",ext,"EXT");

    QMessageBox::information(this, "通知", "锁定成功！");

    settable();
    saveFileList();
}

void MainWindow::on_actionUnlockExt_triggered()
{
    if(ui->comboBox_filetype->currentIndex() != 3)
        return;
    QStringList List;

    for(int i=0;i<rowcount;i++)
    {
      if(ui->tableView->model()->index(i,1).data(Qt::CheckStateRole) == Qt::Checked )
      {
          List<<ui->tableView->model()->index(i,0).data().toString();

      }
    }

    for(int i=0; i<List.size(); i++)
    {
           ExtList.removeAll(List[i]);
           write_log("0",List[i],"EXT");
    }

    ExtList.removeDuplicates();

    QMessageBox::information(this, "通知", "解锁成功！");

    settable();
    saveFileList();
}

void MainWindow::select_all()
{
    ui->checkBox_select_all->checkState();
    if(ui->comboBox_filetype->currentIndex()<2)
    {
        ;
    }
    else
    {
        ;
    }
}


void MainWindow::on_checkBox_search_all_stateChanged(int)
{
    if(ui->checkBox_search_all->checkState() == Qt::Checked)
        search_all = true;
    else
        search_all = false;
}
