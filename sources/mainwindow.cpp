#include "includes/mainwindow.h"
#include "ui_mainwindow.h"

#include <cstring>
#include <sys/types.h>
#include <sys/stat.h>

#include <QDebug>
#include <QJsonParseError>

const char* const MainWindow::databaseCategorie[] = {
    "Actors",
    "Animations",
    "Armors",
    "Classes",
    "CommonEvents",
    "Enemies",
    "Items",
    "Skills",
    "States",
    "Tilesets",
    "Troops",
    "Weapons"
};

const char* const MainWindow::databaseDynamicCategorie[] = {
    "Map"
};

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    projectPath={};


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

    if (stat(fullPath.c_str(), &info) == 0 && S_ISDIR(info.st_mode))
    {
        projectPath = fullPath;

        for(auto element : databaseCategorie)
        {
            std::string concatTestPath = projectPath;
            concatTestPath.append("\\").append(element).append(".json");

            if( stat (concatTestPath.c_str(), &info) == 0 )
            {
                ui->comboBoxDataType->addItem(element);
            }
        }

        for(auto element : databaseDynamicCategorie)
        {
            std::string concatTestPath = projectPath;
            char buffer [50];
            int n;
            bool isElementDetected = false;

            concatTestPath.append("\\").append(element).append("001").append(".json");

            if( stat (concatTestPath.c_str(), &info) == 0 )
            {
                ui->comboBoxDataType->addItem(element);
            }

#if defined(PUT_THAT_ELSEWHERE)
            do
            {
                n=sprintf (buffer, "%.3d",index);

                concatTestPath.append("\\").append(element).append(".json");

                isElementDetected = (stat (concatTestPath.c_str(), &info) == 0);
                if( isElementDetected )
                {
                    ui->comboBoxDataType->addItem(element);
                }

            }while(isElementDetected);
#endif
        }
        return true;
    }
    return {};
}

void MainWindow::on_comboBoxDataType_currentIndexChanged(int index)
{
    qDebug() << ui->comboBoxDataType->currentText();

    std::string fileFullPath = projectPath;
    fileFullPath
        .append("\\")
        .append(ui->comboBoxDataType->currentText().toStdString())
        .append(".json");

    file.setFileName(fileFullPath.c_str());
    file.open(QIODevice::ReadOnly | QIODevice::Text);

    QJsonParseError jsonError;
    QJsonDocument flowerJson = QJsonDocument::fromJson(file.readAll(),&jsonError);
    if (jsonError.error != QJsonParseError::NoError){
    qDebug() << "ERROR:" << jsonError.errorString();
    }
    QList<QVariant> list = flowerJson.toVariant().toList();

    for(int index = 0; index < list.count(); index++)
    {
        QMap<QString, QVariant> map = list[index].toMap();
        qDebug() << "test: "<< map["name"].toString();
    }

    file.close();
}

