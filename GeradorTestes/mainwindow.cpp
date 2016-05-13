#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <string>
#include <chrono>
#include <algorithm>
#include <random>
#include <QFile>
#include <QTextStream>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->sizeSpinBox->setValue(4);
}

void MainWindow::initializeTable(bool random)
{
    int element,
        rowCount = ui->puzzleTable->rowCount(),
        puzzleSize = rowCount * rowCount;
    ui->puzzleTable->blockSignals(true);
    if(random)
    {
        std::vector<int> random_pieces(puzzleSize);

        for (int i = puzzleSize - 1; i > -1; --i)
            random_pieces[i] = i;

        std::shuffle(random_pieces.begin()
                , random_pieces.end()
                , std::default_random_engine(std::chrono::system_clock::now().time_since_epoch().count())
                );

        for(int row = ui->puzzleTable->rowCount() - 1; row > -1; --row) {
            for(int column = ui->puzzleTable->columnCount() - 1; column > -1 ; --column) {
                element = random_pieces[row * rowCount + column];
                ui->puzzleTable->setItem(row, column, new QTableWidgetItem(QString::number(element)));
                position[element] = std::pair<int ,int>(row, column);
            }
        }
    }
    else {
        for(int row = ui->puzzleTable->rowCount() - 1; row > -1; --row) {
            for(int column = ui->puzzleTable->columnCount() - 1; column > -1 ; --column) {
                element = row * ui->puzzleTable->rowCount() + column;
                ui->puzzleTable->setItem(row, column, new QTableWidgetItem(QString::number(element)));
                position[element] = std::pair<int ,int>(row, column);
            }
        }
    }
    ui->puzzleTable->blockSignals(false);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_sizeSpinBox_valueChanged(int arg1)
{
    ui->puzzleTable->setRowCount(arg1);
    ui->puzzleTable->setColumnCount(arg1);
    initializeTable();
}

bool MainWindow::isValid(int piece)
{
    int rowCount = ui->puzzleTable->rowCount();
    return piece > -1 && piece < rowCount * rowCount;
}

void MainWindow::on_puzzleTable_itemChanged(QTableWidgetItem *item)
{
    bool convertible = false;
    int newItem = item->text().toInt(&convertible);

    if (convertible && isValid(newItem))
    {
        std::pair<int, int>& newItemOldPosition = position[newItem];
        position[toChange] = newItemOldPosition;
        ui->puzzleTable->item(newItemOldPosition.first, newItemOldPosition.second)->setText(QString::number(toChange));
        position[newItem] = std::pair<int, int>(item->row(), item->column());
    }
    else
    {
        item->setText(QString::number(toChange));
    }
}

void MainWindow::on_puzzleTable_itemDoubleClicked(QTableWidgetItem *item)
{
    toChange = item->text().toInt();
}

void MainWindow::on_saveButton_released()
{
    QFile file(ui->filenameLineEdit->text());
    file.open(QIODevice::WriteOnly);
    QTextStream fileStream(&file);
    int rowCount = ui->puzzleTable->rowCount();
    fileStream << "n\n" << rowCount << "\n";
    for(int row = 0; row < rowCount; ++row) {
        for(int column = 0; column < rowCount; ++column) {
            fileStream << " " << ui->puzzleTable->item(row, column)->text() << " ";
        }
    }
    fileStream << "\n";
}

void MainWindow::on_randomButton_released()
{
    initializeTable(true);
}
