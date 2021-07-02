#pragma once

#include <QMainWindow>
#include "includes/codeeditor.h"
#include "includes/rpgNoteHighlighter.h"

#include <QPainter>
#include <QTextBlock>
#include <QFileSystemModel>
#include <QLabel>

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
    void save();

private slots:
    void on_comboBoxDataType_currentIndexChanged(int index);

    void on_comboBoxContext_currentIndexChanged(int index);

    void on_pushButtonSave_released();

private:
    Ui::MainWindow *ui;
    QFileSystemModel fileSystemModel;
    rpgNoteHighlighter *highlighter;

    static const char* const databaseCategorie[] ;
    static const char* const databaseDynamicCategorie[] ;
    static const char tabSpaceNumber = 4;

    std::string projectPath;
    std::string fileFullPath;
    QFile file;
    QAction* actionTest;
    QLabel* labelWatermark;


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
    bool backupJsonFile(const std::string &fullPath);

    void replaceAll(std::string& str, const std::string& from, const std::string& to);

};

