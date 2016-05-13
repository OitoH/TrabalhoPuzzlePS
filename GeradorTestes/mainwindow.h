#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableWidgetItem>
#include <unordered_map>
#include <utility>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_sizeSpinBox_valueChanged(int arg1);

    void on_puzzleTable_itemChanged(QTableWidgetItem *item);

    void on_puzzleTable_itemDoubleClicked(QTableWidgetItem *item);

    void on_saveButton_released();

    void on_randomButton_released();

private:
    Ui::MainWindow *ui;
    int toChange;
    std::unordered_map<int, std::pair<int, int>> position;
    void initializeTable(bool random = false);
    bool isValid(int piece);
};

#endif // MAINWINDOW_H
