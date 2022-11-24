#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QString>
#include <string.h>
#include <conio.h>
#include <vector>

using namespace std;

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

void MainWindow::on_bm_clicked()
{
    ui->result->setText(" ");

    unsigned int ASCII[256];
    unsigned int last;
    int step = 0;

    QString line = ui->textline->text();
    QString key = ui->keyline->text();

    if (key.size() == 0 or line.size() == 0)
    {
        QMessageBox messageBox;
        messageBox.critical(0,"Error","Invalid input !");
        messageBox.setFixedSize(500,200);
        return;
    }

    for (int i = 0; i < 256; i++)
    {
        ASCII[i] = key.size();
    }
    for (int i = key.size() - 2; i >= 0; i--)
    {
        if ( ASCII[(int)(key[i].toLatin1())] == key.size() )
            ASCII[(int)(key[i].toLatin1())] = key.size() - i - 1;
    }

    last = key.size() - 1;
    step++;

    while (last < line.size())
    {
        int k;
        for (k = key.size() - 1; k >= 0; k--)
        {
            if (line[last - key.size() + k + 1] == key[k]) continue;
            else    if (k == key.size() - 1)
            {
                last += ASCII[(int)(line[last].toLatin1())];
                break;
            }
            else {
                last += ASCII[(int)(line[last - key.size() + k + 1].toLatin1())];
                break;
            }
        }

        if (k == -1)
        {
            ui->result->setText("The search word was found in " + QString::number(step) + " steps. BM.");
            return;
        }
        if (last < line.size()) step++;

    }

    ui->result->setText("There is no search word in the string. BM.");

}


void MainWindow::on_kmp_clicked()
{
    ui->result->setText(" ");

    QString line = ui->textline->text();
    QString key = ui->keyline->text();

    if (key.size() == 0 or line.size() == 0)
    {
        QMessageBox messageBox;
        messageBox.critical(0,"Error","Invalid input !");
        messageBox.setFixedSize(500,200);
        return;
    }

    int n1 = key.size();
    vector<int> pf(n1);
    pf[0] = 0;

    for (int i = 1; i < n1; i++)
    {
        int j = pf[i - 1];
        while ((j > 0) && (key[j] != key[i]))
        {
            j = pf[j - 1];
        }
        if (key[i] == key[j]) j++;

        pf[i] = j;
    }

    int n2 = line.size();
    int i = 0;
    int j = 0;
    while (i < n2)
        {
            if (line[i] == key[j])
            {
                i++;
                j++;
                if (j == n1)
                {
                    ui->result->setText("The search word was found. KMP.");
                    break;
                }
            }
            else if (line[i] != key[j])
            {
                if (j == 0)
                    i++;
                else
                    j = pf[j - 1];
            }
        }
        if ((i == n2) && (j < n1))
            ui->result->setText("There is no search word in the string. KMP.");
}




void MainWindow::on_rabin_clicked()
{
    ui->result->setText(" ");

    int mod = 77;
    int constant = 9;

    QString line = ui->textline->text();
    QString key = ui->keyline->text();

    int delta;
    int value = 0;
    int func_value = 0;

    int word_hash;
    bool comparison_result = 0;
    int step = 0;

    if (key.size() == 0 or line.size() == 0)
    {
        QMessageBox messageBox;
        messageBox.critical(0,"Error","Invalid input !");
        messageBox.setFixedSize(500,200);
        return;
    }


    for (int i = key.size() - 1; i >= 0; i--)
    {
        value += ((int)pow(constant, i) * (int)line[key.size() - 1 - i].toLatin1()) % mod;
    }

    value = value % mod;

    delta = (int)pow(constant, key.size() - 1) % mod;

    for (int i = key.size() - 1; i >= 0; i--)
    {
        func_value += ((int)pow(constant, i) * (int)key[key.size() - 1 - i].toLatin1()) % mod;
    }

    word_hash = func_value % mod;


    for (int i = 0; i < line.size() - key.size(); i++)
    {
        step++;

        if (value == word_hash)
        {
            ui->result->setText(ui->result->text() +
                                "The hash function values matched. An element-by-element comparison occurred.");
            comparison_result = 0;
            for (int k = 0; k < key.size(); k++)
            {
                if ( key[k] != line[k + i])
                {
                    comparison_result = 1;
                    break;
                }
            }

            if (comparison_result == 0)
            {
                ui->result->setText(ui->result->text() + "The search word was found in "
                                    + QString::number(step) + " steps.");
                return;
            }
        }

        value = ((((value - delta * (int)(line[i].toLatin1())) * constant)
                  + (int)line[i + key.size()].toLatin1()) % mod + mod) % mod;
    }

    ui->result->setText("There is no search word in the string.");

}



