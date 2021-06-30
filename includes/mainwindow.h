#pragma once

#include <QMainWindow>
#include "includes/codeeditor.h"

#include <QPainter>
#include <QTextBlock>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
};

