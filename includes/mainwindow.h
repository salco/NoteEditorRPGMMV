#pragma once

#include <QMainWindow>
#include "includes/codeeditor.h"

#include <QPainter>
#include <QTextBlock>
#include <QFileSystemModel>

#include <string>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void setProjectPath(std::string text);

private slots:
    void on_comboBoxDataType_currentIndexChanged(int index);

    void on_comboBoxContext_currentIndexChanged(int index);

    void on_pushButtonSave_released();

private:
    Ui::MainWindow *ui;
    QFileSystemModel fileSystemModel;

    static const char* const databaseCategorie[] ;
    static const char* const databaseDynamicCategorie[] ;

    std::string projectPath;
    std::string fileFullPath;
    QFile file;

    bool extractDataCategories(std::string fullPath);
    void updateContextComboBox(const std::string &categorie, QJsonDocument &contextData);

    /**
     * @brief extract info from JsonFile in QT object
     * @param [in] fullPath the full path to extract the json data
     * @param [out] data will set info if reading succeed
     * @return true on succeed
     */
    bool extractContextFromJsonFile(const std::string &fullPath, QJsonDocument& data);

    bool saveJsonData(const std::string &fullPath, QJsonDocument& data);

};

