#include "includes/mainwindow.h"
#include "ui_mainwindow.h"

#include <cstring>
#include <sys/types.h>
#include <sys/stat.h>

#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);


//        fileSystemModel.setOption(QFileSystemModel::DontUseCustomDirectoryIcons);
//        fileSystemModel.setOption(QFileSystemModel::DontWatchForChanges);

    fileSystemModel.setNameFilters({"*.json"});
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setProjectPath(std::string text)
{




    text.append("\\data");
    auto result = extractData(text);

    if(result)
    {
        ui->textEdit->setPlainText(text.c_str());
        fileSystemModel.setRootPath((ui->textEdit->toPlainText()));
        ui->treeView->setModel(&fileSystemModel);
        ui->treeView->setRootIndex(fileSystemModel.index((ui->textEdit->toPlainText())));
    }

}

bool MainWindow::extractData(std::string fullPath)
{
    struct stat info;

    if (stat(fullPath.c_str(), &info) == 0 && S_ISDIR(info.st_mode)) {
        qDebug() << "found";

        return true;
    }
    return {};
}
