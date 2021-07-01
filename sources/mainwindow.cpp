#include "includes/mainwindow.h"
#include "ui_mainwindow.h"

#include <cstring>
#include <sys/types.h>
#include <sys/stat.h>
#include <algorithm>

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
    constexpr uint8_t maxFileNameLenght = 50;

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
            char buffer [maxFileNameLenght];
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
//    qDebug() << ui->comboBoxDataType->currentText();

    std::string fileFullPath = projectPath;
    fileFullPath
        .append("\\")
        .append(ui->comboBoxDataType->currentText().toStdString())
        .append(".json");

    file.setFileName(fileFullPath.c_str());
    file.open(QIODevice::ReadOnly | QIODevice::Text);

    QJsonParseError jsonError;
    QJsonDocument flowerJson = QJsonDocument::fromJson(file.readAll(),&jsonError);
    file.close();

    if (jsonError.error != QJsonParseError::NoError)
    {
        qDebug() << "ERROR:" << jsonError.errorString();
    }
    else
    {
        updateContextComboBox(ui->comboBoxDataType->currentText().toStdString(), flowerJson);
    }



#if defined(TODISPLACE)

    for(int index = 0; index < list.count(); index++)
    {
        QMap<QString, QVariant> map = list[index].toMap();
        qDebug() << "test: "<< map["name"].toString();
    }

        qDebug() << "test:"  << flowerJson.toVariant().toList()[9].toMap()["name"].toString();
    ui->plainTextEdit->setPlainText(flowerJson.toVariant().toList()[9].toMap()["note"].toString());



    //TODO set le CurrentFileEdited comme variable de class
    //TODO ajouter le id ("%.4d %s", id, name) dans le 2eme radio group

    //TODO faire en sorte que une fois on selectione le bon context on ouvre le truc dans l'ide
    //TODO voir pour le bon  JsonFormat a ledit et apres
#endif

}


void MainWindow::updateContextComboBox(const std::string &categorie, QJsonDocument &contextData)
{
    constexpr uint8_t maximumCaracterLenght = 100;
    constexpr auto elementTypeToDisplay = "name";
    auto resultSearch =
            std::find (std::begin(databaseCategorie), std::end(databaseCategorie), categorie);

    if(resultSearch != std::end(databaseCategorie))
    {
        qDebug() << "categorie found:" << *resultSearch;
        ui->comboBoxContext->setEnabled(false);
        ui->comboBoxContext->clear();

        QList<QVariant> listFromContextFile = contextData.toVariant().toList();

        char buffer [maximumCaracterLenght];
        for(int index = 0; index < listFromContextFile.count(); index++)
        {
            sprintf (buffer, "%.4d %s",
                     index,
                     listFromContextFile[index]
                        .toMap()[elementTypeToDisplay].toString().toStdString().c_str()
                     );
            ui->comboBoxContext->addItem(buffer);
        }

        ui->comboBoxContext->setEnabled(true);
    }
}
