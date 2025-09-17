#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QDebug>
#include <QFile>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    setWindowFlags(Qt::FramelessWindowHint | Qt::Window);

    ui->setupUi(this);

    QFont font = ui->ButtonExit->font(); // изменение шрифта

    ui->ButtonExit->setGeometry(550, 15, 25, 25);
    font.setPointSize(13);
    ui->ButtonExit->setFont(font);
    ui->ButtonExit->setStyleSheet("color: red;background-color: rgb(30, 46, 66);");
    ui->ButtonExit->setText("X");


    ui->chooseFileButton->setGeometry(55, 40 , 116, 30);
    font = ui->chooseFileButton->font();
    font.setPointSize(12);
    ui->chooseFileButton->setFont(font);
    ui->chooseFileButton->setStyleSheet("color: white; background-color: rgb(30, 46, 66);");
    ui->chooseFileButton->setText("Road to surse");

    ui->filePathEdit->setGeometry(180, 40, 80, 30);
    font = ui->filePathEdit->font();
    font.setPointSize(12);
    ui->filePathEdit->setFont(font);
    ui->filePathEdit->setStyleSheet("border: none ");
    ui->filePathEdit->setPlaceholderText("D:\\");

    ui->checkBox_SursFile->setGeometry(56, 90, 120, 30);
    font = ui->checkBox_SursFile->font();
    font.setPointSize(12);
    ui->checkBox_SursFile->setFont(font);
    ui->checkBox_SursFile->setStyleSheet("color: white;");
    ui->checkBox_SursFile->setText("Clear surs file");

    ui->keyEdit->setGeometry(180, 125, 80, 30);
    font = ui->keyEdit->font();
    font.setPointSize(12);
    ui->keyEdit->setFont(font);
    ui->keyEdit->setStyleSheet("border: none ");
    ui->keyEdit->setPlaceholderText("Key hex");

    ui->startButton->setGeometry(250, 280, 100, 30);
    font = ui->startButton->font();
    font.setPointSize(15);
    ui->startButton->setFont(font);
    ui->startButton->setStyleSheet("color: white; background-color: rgb(30, 46, 66);");
    ui->startButton->setText("Start");

    ui->progressBar->setGeometry(55, 220, 0, 0);

    connect(ui->chooseFileButton, &QPushButton::clicked, this, &MainWindow::chooseFile);
    connect(ui->filePathEdit, &QLineEdit::textChanged, this, &MainWindow::onLineEditTextChanged);
    connect(ui->keyEdit, &QLineEdit::textChanged, this, &MainWindow::onLineEditTextChanged);
    connect(ui->startButton, &QPushButton::clicked, this, &MainWindow::startProcessing);
    connect(ui->ButtonExit, &QPushButton::clicked, this, &MainWindow::close);
    connect(ui->checkBox_SursFile, &QCheckBox::toggled, this, [=](bool checked){
                                                                            if (checked) {
                                                                                clear = true;
                                                                            } else {
                                                                                clear = false;
                                                                            }
                                                                        });
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::chooseFile()
{

    ui->progressBar->setGeometry(55, 220, 0, 0);
    inputFile = QFileDialog::getOpenFileName(this,
                                             tr("Выберите файл"),
                                             QDir::homePath(),
                                             tr("Text and Bin files (*.txt *.bin);;All Files (*.*)"));

    if (!inputFile.isEmpty()) {
        ui->filePathEdit->setText(inputFile);
    }
}


void MainWindow::onLineEditTextChanged(const QString &text) // Динамически менять размер поле ввода
{
    if (text.size() < 60){
        qint16 newWidth = qMax(minWidth, text.size() * widthPerChar);
        QLineEdit *edit = qobject_cast<QLineEdit*>(sender());
        QRect rect = edit->geometry();

        rect.setWidth(newWidth);
        edit->setGeometry(rect);
    }
}


void MainWindow::startProcessing()
{
    if (inputFile.isEmpty()) {
        QMessageBox::warning(this, "Ошибка", "Сначала выберите файл");

        return;
    }

    QString outputFile = QFileDialog::getSaveFileName(this, "Сохранить как",
                                                      QDir::homePath(),
                                                      tr("Text and Bin files (*.txt *.bin);;All Files (*.*)"));
    if (outputFile.isEmpty()) return;

    bool ok;
    quint64 key = ui->keyEdit->text().toULongLong(&ok, 16); // проверка ключа hex

    if (!ok) {
        QMessageBox::warning(this, "Ошибка", "Введите корректный ключ (в hex)");
        return;
    }

    QThread *thread = new QThread;
    Worker *worker = new Worker;
    worker->setParams(inputFile, outputFile, key);
    worker->moveToThread(thread);

    ui->progressBar->setGeometry(55, 220, 500, 25); // устанавливаем размер для шкалы загрузки

    connect(thread, &QThread::started, worker, &Worker::process);
    connect(worker, &Worker::progress, this, &MainWindow::updateProgress);
    connect(worker, &Worker::finished, this, &MainWindow::onFinished);
    connect(worker, &Worker::finished, worker, &Worker::deleteLater);
    connect(worker, &Worker::finished, thread, &QThread::quit);
    connect(thread, &QThread::finished, thread, &QThread::deleteLater);

    thread->start();
}



void MainWindow::updateProgress(int value)
{
    ui->progressBar->setValue(value);
}

void MainWindow::onFinished(bool success, const QString &message)
{
    QMessageBox::information(this, success ? "Готово" : "Ошибка", message);


    if (clear){
        QFile file(inputFile);
        if (file.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
            file.close();
        }
    }

}



void MainWindow::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        m_drag = true;
        m_dragPosition = event->globalPosition().toPoint() - frameGeometry().topLeft();
        event->accept();
    }
}

void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
    if (m_drag && (event->buttons() & Qt::LeftButton)) {
        move((event->globalPosition().toPoint()) - m_dragPosition);
        event->accept();
    }
}

void MainWindow::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        m_drag = false;
        event->accept();
    }
}







