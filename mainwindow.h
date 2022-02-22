#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLineEdit>
#include <QTextBrowser>
#include <QPushButton>
#include <QGridLayout>
#include "taskresolver.h"
#include "shape.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void testResolver();
    void addTextToConsole(QString text);
    void testSerializer(QString pathToFile);
    void testCompare();
private:
    bool initResolverInput();
    Ui::MainWindow *ui;
    QLineEdit  **resolverInput;
    QTextBrowser* console;
    QPushButton* resolverButton;
    QGridLayout* layoutToCentral;
};
#endif // MAINWINDOW_H
