#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QThread>
#include "worker.h"
#include <QMainWindow>
#include <QMouseEvent>



QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void chooseFile();
    void startProcessing();
    void updateProgress(int value);
    void onLineEditTextChanged(const QString &text);
    void onFinished(bool success, const QString &message);
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

private:
    Ui::MainWindow *ui;
    QString inputFile{};
    qint16 minWidth = 80; // базовая длина поля ввода
    qint16 widthPerChar = 9; // базовая ширина
    bool clear = false; // для checkBox
    bool m_drag = false; // для интерактивности
    QPoint m_dragPosition;
};
#endif // MAINWINDOW_H
