#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QTime>

namespace Ui {
class MainWindow;
}

struct Symbol {
    int symb;
    int comb;
};


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_startButton_clicked();

    void on_startMoreButton_clicked();

    void on_clearButton_clicked();

private:
    Ui::MainWindow *ui;

    void genWinMatrix();
    void genLoseMatrix();
    void genCoefMatrix();
    void genProbMatrix();
    void getMaxPossible(Symbol &s);
    double lootRand();
    Symbol getSymbol();
    void drawPic(QString symb, int count);
    void genColumn(QString winS, QString &s1,  QString &s2, QString &s3, QString &s4);

    QTime midnight{0,0,0};

    QList<QList<int> > lootMatrix;
    QList<QList<double> > coefMatrix;
    QList<QList<double> > probMatrix;

    int m_count {0};
    int m_win {0};
    int m_sum {0};
    double cashHave {0};
    double cashTake {0};
    double cashPay {0};
};

#endif // MAINWINDOW_H
