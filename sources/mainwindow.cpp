#include "includes/mainwindow.h"
#include "ui_mainwindow.h"

#include <cstring>
#include <sys/types.h>
#include <sys/stat.h>
#include <algorithm>

#include <QDebug>

#include <QJsonParseError>
#include <QJsonObject>
#include <QJsonArray>

const char* const MainWindow::databaseCategorie[] = {
    "Actors",
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
#include <QFontMetricsF>
#include <QKeySequence>
#include <QAction>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    projectPath.clear();
    fileFullPath.clear();

    fileSystemModel.setNameFilters({"*.json"});

    ui->plainTextEdit->setTabStopDistance(
        QFontMetricsF(ui->plainTextEdit->font()).horizontalAdvance(' ') * tabSpaceNumber);

    highlighter = new rpgNoteHighlighter(ui->plainTextEdit->document());
    actionTest = new QAction(this);
    actionTest->setShortcut(QKeySequence::Save);

    connect(actionTest, &QAction::triggered,
                     this, &MainWindow::save);
    ui->plainTextEdit->addAction(actionTest);

    labelWatermark = new QLabel(this);

    labelWatermark->setText("Made by Salco | V:"  IDE_VERSION);
    ui->statusbar->addPermanentWidget(labelWatermark);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete actionTest;
    delete highlighter;
    delete labelWatermark;

}

void MainWindow::save()
{
    qDebug() << "Saving";
    QJsonDocument originalFile;

    auto result = extractContextFromJsonFile(fileFullPath, originalFile);

    if(result)
    {
#if defined(TEST_OUT_OF_FILE)
        std::string fileTestPath = projectPath;
        fileTestPath
            .append("\\")
            .append("testFile")
            .append(".json");
#else
        std::string& fileTestPath = fileFullPath;
#endif

        auto arrayJson = originalFile.array();

        if(not originalFile.array().at( ui->comboBoxContext->currentIndex() ).isNull())
        {
            auto originalValue = arrayJson.at(ui->comboBoxContext->currentIndex()).toObject();

            std::string formatedData = ui->plainTextEdit->toPlainText().toStdString();
            replaceAll(formatedData,"\t", std::string(tabSpaceNumber, ' '));

            originalValue["note"] = formatedData.c_str();
            arrayJson.replace(ui->comboBoxContext->currentIndex(), originalValue);
            originalFile.setArray( arrayJson);

            saveJsonData(fileTestPath,originalFile);
        }
    }
}

void MainWindow::setProjectPath(std::string text)
{
    text.append("\\data");
    auto result = extractDataCategories(text);

    if(result)
    {
        ui->textEdit->setPlainText(text.c_str());
        fileSystemModel.setRootPath((ui->textEdit->toPlainText()));
        ui->treeView->setModel(&fileSystemModel);
        ui->treeView->setRootIndex(fileSystemModel.index((ui->textEdit->toPlainText())));
    }

}

bool MainWindow::extractDataCategories(std::string fullPath)
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
        }
        return true;
    }
    return {};
}

void MainWindow::on_comboBoxDataType_currentIndexChanged(int index)
{
    QJsonDocument flowerJson;

    fileFullPath = projectPath;
    fileFullPath
        .append("\\")
        .append(ui->comboBoxDataType->currentText().toStdString())
        .append(".json");

    auto result = extractContextFromJsonFile(fileFullPath, flowerJson);

    if(result)
    {
        updateContextComboBox(ui->comboBoxDataType->currentText().toStdString(), flowerJson);
    }
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

void MainWindow::on_comboBoxContext_currentIndexChanged(int index)
{
    constexpr auto elementTypeToDisplay = "note";
    QJsonDocument flowerJson;

    if(index >= 0)
    {
        auto result = extractContextFromJsonFile(fileFullPath, flowerJson);

        if(result)
        {
            auto contextMap = flowerJson.toVariant().toList()[index].toMap();
            auto searchForData = contextMap.find(elementTypeToDisplay);

            ui->plainTextEdit->clear();

            if(searchForData != contextMap.end())
            {
                ui->plainTextEdit->setPlainText(searchForData->toString());
            }
        }
    }
}

bool MainWindow::extractContextFromJsonFile(const std::string &fullPath, QJsonDocument& data)
{
    file.setFileName(fileFullPath.c_str());
    file.open(QIODevice::ReadOnly | QIODevice::Text);

    QJsonParseError jsonError;
    QJsonDocument flowerJson = QJsonDocument::fromJson(file.readAll(),&jsonError);
    file.close();

    if (jsonError.error == QJsonParseError::NoError)
    {
        data = flowerJson;
        return true;
    }
    return false;
}

void MainWindow::on_pushButtonSave_released()
{
    save();
}

bool MainWindow::saveJsonData(const std::string &fullPath, QJsonDocument& data)
{
    bool result = false;
    auto arrayJson = data.array();

    //small backup
    backupJsonFile(fullPath);
    file.setFileName(fullPath.c_str());

    if (file.open(QFile::WriteOnly|QFile::Text))
    {
        file.write("[\n");

        for(int index = 0; index < arrayJson.count(); index++)
        {
            auto jsonDoc = QJsonDocument(data[index].toObject());

            if(arrayJson.at(index).isNull())
            {
                file.write("null");
            }
            else
            {
                file.write(
                    jsonDoc.toJson(QJsonDocument::JsonFormat::Compact)
                );
            }
            if(index+1 < arrayJson.count())
            {
                file.write(",");
            }
            file.write("\n");
        }
        file.write("]\n");

        file.close();
        result = true;
    }
    return result;
}

void MainWindow::replaceAll(std::string& str, const std::string& from, const std::string& to) {
    if(from.empty())
        return;
    size_t start_pos = 0;
    while((start_pos = str.find(from, start_pos)) != std::string::npos) {
        str.replace(start_pos, from.length(), to);
        start_pos += to.length(); // In case 'to' contains 'from', like replacing 'x' with 'yx'
    }
}

bool MainWindow::backupJsonFile(const std::string &fullPath)
{
    bool result = false;
    std::string backupFileName = fullPath.c_str();
    backupFileName.append(".backup");
    file.setFileName(backupFileName.c_str());
    file.remove();
    result = file.copy(fullPath.c_str(), backupFileName.c_str());

    return result;
}
