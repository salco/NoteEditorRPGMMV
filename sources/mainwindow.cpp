#include "includes/mainwindow.h"
#include "ui_mainwindow.h"

#include <cstring>
#include <sys/types.h>
#include <sys/stat.h>

#include <QDebug>

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
    "System",
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
        for(auto element : databaseCategorie)
        {
            std::string concatTestPath = fullPath;
            concatTestPath.append("\\").append(element).append(".json");

            if( stat (concatTestPath.c_str(), &info) == 0 )
            {
                ui->comboBoxDataType->addItem(element);
            }
        }

        for(auto element : databaseDynamicCategorie)
        {
            std::string concatTestPath = fullPath;
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
