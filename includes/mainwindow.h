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

private:
    Ui::MainWindow *ui;
    QFileSystemModel fileSystemModel;

    static const char* const databaseCategorie[] ;
    static const char* const databaseDynamicCategorie[] ;

    std::string projectPath;
    QFile file;

    bool extractData(std::string fullPath);
    void updateContextComboBox(const std::string &categorie, QJsonDocument &contextData);
};

