#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <random>
#include <chrono>
#include <time.h>
#include <functional>

using namespace std;

const QList<int> S1 {3,15,45};
const QList<int> S2 {5,30,75};
const QList<int> S3 {7,50,150};
const QList<int> S4 {9,60,250};
const QList<int> S5 {12,75,350};
const QList<int> S6 {15,90,500};
const QList<int> S7 {20,120,750};
const QList<int> S8 {30,150,1000};
const QList<int> S9 {50,200,2000};
const QList<QString> sym {"S1", "S2", "S3", "S4", "S5", "S6", "S7", "S8", "S9"};
const double pWin = 0.2;
const double cashBack = 0.9;
const double bet = 1;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->tableWidget->horizontalHeader()->hide();
    ui->tableWidget->verticalHeader()->hide();

    //qsrand(midnight.secsTo(QTime::currentTime()));

    lootMatrix.append(S1);
    lootMatrix.append(S2);
    lootMatrix.append(S3);
    lootMatrix.append(S4);
    lootMatrix.append(S5);
    lootMatrix.append(S6);
    lootMatrix.append(S7);
    lootMatrix.append(S8);
    lootMatrix.append(S9);

    genCoefMatrix();
    genProbMatrix();

    setFixedHeight(sizeHint().height());
    setFixedWidth(sizeHint().width());

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_startButton_clicked()
{
    m_count += 1;
    cashTake += bet;
    double r = lootRand();
    if ( r < 0.2) {
        genWinMatrix();
        m_win += 1;
    }
    else {
        genLoseMatrix();
    }
}

void MainWindow::genWinMatrix()
{
    Symbol winSymbol;
    winSymbol = getSymbol();
    QString smb;
    int countSmb;

    switch (winSymbol.symb) {
    case 0:
        smb = "S1";
        break;
    case 1:
        smb = "S2";
        break;
    case 2:
        smb = "S3";
        break;
    case 3:
        smb = "S4";
        break;
    case 4:
        smb = "S5";
        break;
    case 5:
        smb = "S6";
        break;
    case 6:
        smb = "S7";
        break;
    case 7:
        smb = "S8";
        break;
    case 8:
        smb = "S9";
        break;
    default:
        break;
    }

    switch (winSymbol.comb) {
    case 0:
        countSmb = 3;
        break;
    case 1:
        countSmb = 4;
        break;
    case 2:
        countSmb = 5;
        break;
    default:
        break;
    }

    //qDebug() << smb << countSmb;

    drawPic(smb, countSmb);

}

void MainWindow::genLoseMatrix()
{
    drawPic("none", -1);
}

void MainWindow::genCoefMatrix()
{
    for (int i = 0; i < lootMatrix.size(); i++) {
        QList<double> symbol;
        for (int j = 0; j < lootMatrix[i].size(); j++) {
            symbol.append((bet*cashBack) / lootMatrix[i][j]);
        }
        coefMatrix.append(symbol);
    }


}

void MainWindow::genProbMatrix()
{
    double sum = 0;

    for (int i = 0; i < coefMatrix.size(); i++) {
        for (int j = 0; j < coefMatrix[i].size(); j++) {
            sum += coefMatrix[i][j];
        }
    }

    for (int i = 0; i < coefMatrix.size(); i++) {
        QList<double> prob;
        for (int j = 0; j < coefMatrix[i].size(); j++) {
            prob.append(coefMatrix[i][j]/sum);
        }
        probMatrix.append(prob);
    }
    qDebug() << probMatrix;
}

void MainWindow::getMaxPossible(Symbol &s)
{
    int pay = 0;
    for (int i = 0; i < lootMatrix.size(); i++) {
        for (int j = 0; j < lootMatrix[i].size(); j++) {
            if ((lootMatrix[i][j] < (cashHave - cashTake*0.1)) &&
                    (lootMatrix[i][j] > pay)) {
                pay = lootMatrix[i][j];
                s.symb = i;
                s.comb = j;
            }
        }
    }
}

double MainWindow::lootRand()
{
    return (double)rand()/(RAND_MAX);
}

Symbol MainWindow::getSymbol()
{
  //  double rnd = lootRand();
    double sumProb = 0;

 //   mt19937::result_type seed = time(0);
    auto seed = chrono::high_resolution_clock::now().time_since_epoch().count();
    auto real_rand = std::bind(uniform_real_distribution<double>(0,1),
                               mt19937(seed));

   // qDebug() << rnd;
    //qDebug() << probMatrix;
    Symbol s;
    s.symb = 0;
    s.comb = 0;

    for (int i = 0; i < probMatrix.size(); i++) {
        for (int j = 0; j < probMatrix[i].size(); j++) {
            sumProb += probMatrix[i][j];
            if (real_rand() <= sumProb) {
                s.symb = i;
                s.comb = j;
                //qDebug() << i << j;
                if (lootMatrix[i][j] > (cashHave - cashTake*0.1)) {
                  //  getMaxPossible(s);
                    s.symb = 1;
                    s.comb = 0;
                    if (cashPay > 0.901*cashTake) {
                        s.symb = 0;
                        s.comb = 0;
                    }
                }
              //  auto a = lootMatrix[s.symb][s.comb];
                m_sum += lootMatrix[s.symb][s.comb];
                cashPay += lootMatrix[s.symb][s.comb];
                cashHave -= lootMatrix[s.symb][s.comb];
                return s;
            }
        }
    }
    return s;
}

void MainWindow::drawPic(QString symb, int count)
{
    QString s1, s2, s3, s4;
    s4 = "nene";
    for (int i = 0; i < ui->tableWidget->columnCount(); i++) {
        genColumn(symb, s1, s2, s3, s4);
        ui->tableWidget->setItem(0, i, new QTableWidgetItem(s1));//  ->item(i,j)->setText("X");
        ui->tableWidget->item(0,i)->setBackgroundColor(Qt::white);

        ui->tableWidget->setItem(1, i, new QTableWidgetItem(s2));
        ui->tableWidget->item(1,i)->setBackgroundColor(Qt::white);

        ui->tableWidget->setItem(2, i, new QTableWidgetItem(s3));
        ui->tableWidget->item(2,i)->setBackgroundColor(Qt::white);
    }

    if (symb == "none") {
        return;
    }

    int c = count;
    int col = 0;

    while (c > 0) {
        ui->tableWidget->item(1, col)->setText(symb);
        ui->tableWidget->item(1,col)->setBackgroundColor(Qt::red);
        col += 1;
        c -= 1;
    }
}

void MainWindow::genColumn(QString winS, QString &s1, QString &s2, QString &s3, QString &s4)
{
    QList<QString> buf = sym;
    int cas, num;
    if (s4 == "nene") {
        cas = 0;
    }
    if (s4 == "end") {
        cas = 1;
    }

    if (winS == "none") {
        switch (cas) {
        case 0:
            num = rand() % 9;
            s1 = buf[num];
            buf.removeAt(num);

            num =  rand() % 8;
            s2 = buf[num];
            buf.removeAt(num);

            num =  rand() % 7;
            s3 = buf[num];

            s4 = s2;
            return;
            break;
        case 1:
            num =  rand() % 9;
            s1 = buf[num];
            buf.removeAt(num);

            num =  rand() % 8;
            s2 = buf[num];
            buf.removeAt(num);

            num =  rand() % 7;
            s3 = buf[num];
            return;
        default:
            for (int i = 0; i < buf.size(); i++) {
                if (s4 == buf[i]) {
                    buf.removeAt(i);
                    break;
                }
            }

            num =  rand() % 8;
            s2 = buf[num];
            buf.removeAt(num);

            buf.append(s4);

            num =  rand() % 8;
            s1 = buf[num];
            buf.removeAt(num);

            num =  rand() % 7;
            s3 = buf[num];

            s4 = "end";
            return;
        }
    }
    else {
        for (int i = 0; i < buf.size(); i++) {
            if (winS == buf[i]) {
                buf.removeAt(i);
                break;
            }
        }
        int num =  rand() % 8;
        s1 = buf[num];
        buf.removeAt(num);

        num =  rand() % 7;
        s2 = buf[num];
        buf.removeAt(num);

        num =  rand() % 6;
        s3 = buf[num];
    }
}

void MainWindow::on_startMoreButton_clicked()
{
    int count = ui->iterationLine->text().toInt();



    for (int i = 0; i < count; i++) {
        on_startButton_clicked();
    }

    ui->cash->setText(QString("%1").arg(m_sum));
    ui->winRate->setText(QString("%1").arg((double)m_win / m_count));
    ui->loseCash->setText(QString("%1").arg(cashTake));
    ui->cashBack->setText(QString("%1").arg((double)m_sum / cashTake));
}

void MainWindow::on_clearButton_clicked()
{
    m_count = 0;
    m_win = 0;
    m_sum = 0;
    cashTake = 0;
    cashHave = 0;
    cashPay = 0;
}
