#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <Qfiledialog>
#include <QMessageBox>
#include <QString>
#include <bits/stdc++.h>
#include "Vole_Machine.h"

Machine machine;


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}


string DecToHex(int num) { // Convert decimal integer to hexadecimal string.
    const string hexChars = "0123456789ABCDEF"; // Hexadecimal characters.
    string result;
    do {
        result = hexChars[num % 16] + result; // Append hex character.
        num /= 16;
    } while (num > 0);

    return result;
}

void MainWindow::on_pushButton_clicked() {
    machine.reseteProgram();
    ui->label_6->setText(QString::fromStdString(""));
    ui->label_7->setText(QString::fromStdString("IR: " + machine.getIR()));
    filePath = QFileDialog::getOpenFileName(this,tr("select file"), "C://", "Text file (*.txt)").toStdString();
    size_t pos = filePath.find_last_of("/\\");
    string filename = filePath.substr(pos + 1);
    ui->label_4->setText(QString::fromStdString(filename));
}


void MainWindow::on_pushButton_2_clicked() {

    machine.run();
    for (int i = 0; i < 16; ++i) {
        QListWidgetItem *item = ui->listWidget->item(i);
        QString qval = QString::fromStdString("Register " + DecToHex(i) + " : "+ machine.getReg(i));
        item->setText(qval);
    }
    for (int row = 0; row < 16; ++row) {
        for (int col = 0; col < 16; ++col) {
            QString qval = QString::fromStdString(machine.getCell(row*16 +col));
            ui -> tableWidget->setItem(row,col, new QTableWidgetItem(qval));
        }
    }
    ui ->label_5->setText(QString::fromStdString("PC: " + to_string(machine.getCounter())));
    if(machine.get_is_halted()){
        ui->label_6->setText(QString::fromStdString("Halted"));
    }
    ui->label_7->setText(QString::fromStdString("IR: " + machine.getIR()));
}


void MainWindow::on_pushButton_3_clicked() {
    start = ui->lineEdit->text().toStdString();
    machine.loadProgramFile(filePath, start);
    for (int row = 0; row < 16; ++row) {
        for (int col = 0; col < 16; ++col) {
            QString qval = QString::fromStdString(machine.getCell(row*16 +col));
            ui -> tableWidget->setItem(row,col, new QTableWidgetItem(qval));
        }
    }
    ui ->label_5->setText(QString::fromStdString("PC: " + to_string(machine.getCounter())));
    QMessageBox::information(this, tr("Status"), "File Stored Sucssesfully");
    for (int i = 0; i < 16; ++i) {
        QListWidgetItem *item = ui->listWidget->item(i);
        QString qval = QString::fromStdString("Register " + DecToHex(i) + " : " + machine.getReg(i));
        item->setText(qval);
    }
}


void MainWindow::on_pushButton_4_clicked() {
    while(true){
        if(!machine.get_is_halted()){
            machine.run();
            for (int i = 0; i < 16; ++i) {
                QListWidgetItem *item = ui->listWidget->item(i);
                QString qval = QString::fromStdString("Register " + DecToHex(i) + " : "+ machine.getReg(i));
                item->setText(qval);
            }
            for (int row = 0; row < 16; ++row) {
                for (int col = 0; col < 16; ++col) {
                    QString qval = QString::fromStdString(machine.getCell(row*16 +col));
                    ui -> tableWidget->setItem(row,col, new QTableWidgetItem(qval));
                }
            }
            ui ->label_5->setText(QString::fromStdString("PC: " + to_string(machine.getCounter())));
            if(machine.get_is_halted()){
                ui->label_6->setText(QString::fromStdString("Halted"));
            }
        } else {
            break;
        }
    }
}

