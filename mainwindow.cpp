#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDoubleValidator>
#include <QLabel>
#include <QLayout>
#include "shape.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    {
        layoutToCentral = new QGridLayout;
        console = new QTextBrowser;
        layoutToCentral->addWidget(console, 1,1);
        //initResolverInput();
        ui->centralwidget->setLayout(layoutToCentral);
    }

    testResolver();
    testSerializer("testFile.json");
    testCompare();

}

MainWindow::~MainWindow()
{
    //?    delete [] this->resolverInput;
    delete ui;
}

bool MainWindow::initResolverInput(){
    int resolverWindowsCount = 8;
    resolverInput = new QLineEdit*[resolverWindowsCount];

    for (int c = 0; c < resolverWindowsCount; c++)
        resolverInput[c] = new QLineEdit(this);
    int startX = 550;
    int startY = 40;
    int X = startX, Y = startY;
    int stepHorizontal = 40;
    int stepVertical = 20;
    int gridVerticalCount = 2;
    int gridHorizontalCount = 4;


    QGridLayout* resolverLayout = new QGridLayout;
    layoutToCentral->addLayout(resolverLayout, 1,2);

    QGridLayout* resolverInputLayout = new QGridLayout;
    resolverLayout->addLayout(resolverInputLayout,1,1);
    QGridLayout* resolverButtonLayout = new QGridLayout;
    resolverLayout->addLayout(resolverButtonLayout,2,1);
    QLabel* lineLabel = new QLabel[gridVerticalCount];

    for (int j = 0; j < gridVerticalCount; j++){
        lineLabel[j].setText("Line " + QString::number(j+1));
        lineLabel[j].setFixedWidth(100);
        resolverInputLayout->addWidget(&lineLabel[j], j, 1);
        for (int i = 0; i < gridHorizontalCount; i++){

            int index = j*gridHorizontalCount + i;

            QString coord = (index%2)? "Y":"X";
            QString startOrEnd = (i/2)? " End ":" Start ";
            QString toolTipText = "Line: " + QString::number(j+1) + startOrEnd + coord;

            resolverInput[index]->setToolTip(toolTipText);
            resolverInput[index]->setFixedWidth(stepHorizontal);
            resolverInput[index]->setFixedHeight(stepVertical);
            resolverInput[index]->move(X,Y);

            X += stepHorizontal + stepHorizontal / 2 ;
            auto dv = new QDoubleValidator();
            resolverInput[index]->setValidator(dv);
            resolverInputLayout->addWidget(resolverInput[index], j, i+2);

        }
        Y += stepVertical + stepVertical / 2 ;
        X = startX;

    }

    resolverButton = new QPushButton;
    resolverButton->setText("Start intersection resolver");
    resolverButtonLayout->addWidget(resolverButton, 3,1);



    return true;
}

void MainWindow::testResolver(){

    taskResolver test;

    Line line1 {{0,0},{10,10}};
    Line line2 {{5,5},{7,7}};

    assert((true == test.hasIntersection(line1,line2)));
    addTextToConsole("First  test passed");
    Line line3 {{0,0},{5,5}};
    Line line4 {{6,6},{7,7}};

    assert((false == test.hasIntersection(line3,line4)));
    addTextToConsole("Second test passed");


    Line line5 {{-5.5,-5.5},{5.5,5.5}};
    Line line6 {{6.5,6.5},{-6.5,-6.5}};

    assert((true == test.hasIntersection(line5,line6)));
    addTextToConsole("Third test passed");

    Line line7 {{-5.6,-5.6},{5.4,5.4}};
    Line line8 {{-5.4,-5.4},{5.6,5.6}};

    assert((true == test.hasIntersection(line7,line8)));
    addTextToConsole("Fourth test passed");


}

void MainWindow::testSerializer(QString pathToFile){
    addTextToConsole("Start serializer test");

    std::list<std::shared_ptr<Shape>> TestShapes;

    TestShapes.push_back(std::make_shared<Triangle>(Point2D(1,1),Point2D(2,2), Point2D(0,2)));
    TestShapes.push_back(std::make_shared<Square>(Point2D(0,0),Point2D(0,2), Point2D(2,2)));
    TestShapes.push_back(std::make_shared<Circle>(Point2D(1,1),Point2D(2,2)));

    addTextToConsole("Before:");
    QString before;
    for (auto const &shape : TestShapes){
        QString tmp = shape->toJsonString();
        before += tmp;
        addTextToConsole(tmp);
    }

    std::ofstream jsonFileOut;
    jsonFileOut.open(pathToFile.toStdString());
    assert((true == jsonFileOut.is_open()));

    addTextToConsole("File " + pathToFile +  " has opened");
    try{
        assert((true == writeShapesToFile(TestShapes, jsonFileOut)));
    }catch(...){
        addTextToConsole("Something gone wrong on serialize");
        jsonFileOut.close();
    }
    addTextToConsole("Serialize test passed");

    addTextToConsole("Start deserializer test");

    jsonFileOut.close();
    std::ifstream jsonFileIn;
    jsonFileIn.open(pathToFile.toStdString());
    assert((true == jsonFileIn.is_open()));
     std::list<std::shared_ptr<Shape>> FromSerializer;
    addTextToConsole("File " + pathToFile +  " has opened");
    try{
       FromSerializer = readShapesFromFile(jsonFileIn);
    }catch(...){
        addTextToConsole("Something gone wrong on deserialize");
        jsonFileIn.close();
    }

    jsonFileIn.close();

    addTextToConsole("After:");
    QString after;

    for (auto const &shape : FromSerializer){
        QString tmp  = shape->toJsonString();
        after += tmp;
        addTextToConsole(tmp);
    }

    assert (( after == before ));

    addTextToConsole("Deserialize test passed");
    addTextToConsole("Serialize-deserialize test passed");

}

void MainWindow::testCompare(){
    addTextToConsole("Start compare test");

    std::list<std::shared_ptr<Shape>> TestShapes;

    TestShapes.push_back(std::make_shared<Triangle>(Point2D(1,1),Point2D(2,2), Point2D(0,2)));
    TestShapes.push_back(std::make_shared<Square>(Point2D(0,0),Point2D(0,2), Point2D(2,2)));
    TestShapes.push_back(std::make_shared<Circle>(Point2D(1,1),Point2D(2,2)));

    Square sq(Point2D(0,0),Point2D(0,2), Point2D(2,2));
    assert((true == (sq < TestShapes)));
    Square sq2(Point2D(0,0),Point2D(0,10), Point2D(10,10));
    assert((true == (sq2 > TestShapes)));
    addTextToConsole("Compare test passed");


}


void MainWindow::addTextToConsole(QString text){
    if (console)
    console->append(text);
}
