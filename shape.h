#ifndef SHAPE_H
#define SHAPE_H
#include <cmath>
#include <map>
#include <string>
#include <list>
#include <fstream>
#include <QJsonObject>
#include <QString>
#include <exception>
#include <algorithm>
#define localEpsilon 1e-5
struct Point2D{
    double X;
    double Y;
    Point2D(){
        X = 0.0;
        Y = 0.0;
    }
    Point2D(double _X, double _Y){
        X = _X;
        Y = _Y;
    }
    Point2D(const QJsonObject& point){
        X = point["X"].toDouble();
        Y = point["Y"].toDouble();
    }
    operator QJsonValue() const{
        QJsonObject temp;
        temp["X"] = X;
        temp["Y"] = Y;
        return temp;
    }

    double getDistance(const Point2D& toPoint) const {
        return sqrt( pow(X - toPoint.X, 2) + pow(Y - toPoint.Y,2 ) );
    }
};

class Shape
{
public:
    Shape();

    static const std::map<QString, int> figureName;
    static bool knowShape(const QString& type);

    QString getParams();
    QString getParam(const QString& key) ;
    //to improve performance - 3 implementation
    bool operator<(const std::list<std::shared_ptr<Shape>>& Shapes);
    bool operator>(const std::list<std::shared_ptr<Shape>>& Shapes);
    bool operator==(const std::list<std::shared_ptr<Shape>>& Shapes);
    bool operator>=(const std::list<std::shared_ptr<Shape>>& Shapes);
    bool operator<=(const std::list<std::shared_ptr<Shape>>& Shapes);


    QString toJsonString() const;
    bool fromJsonString(const std::string& jsonStringObject);

    virtual double calcArea() const = 0;

 protected:
    QJsonObject parameters;
    QString ShapeName;
    bool setParam(const QString& key,const QJsonValue& value );

    virtual bool validateParam(const QString& key,const QJsonValue& value ) = 0;


};


bool writeShapesToFile(const std::list<std::shared_ptr<Shape>>&, std::ofstream&);
std::list<std::shared_ptr<Shape>> readShapesFromFile(std::ifstream&);

class Triangle : public Shape{
public:
    Triangle() = delete;
    Triangle(const Point2D& _A,const Point2D& _B,const Point2D& _C ){
        setParam("Type", "Triangle");
        points["A"] = _A;
        setParam("A", _A);
        points["B"] = _B;
        setParam("B", _B);
        points["C"] = _C;
        setParam("C", _C);
    };
    double calcArea() const override{
        const Point2D & A = points.at("A");
        const Point2D & B = points.at("B");
        const Point2D & C = points.at("C");

        double temp = (B.X - A.X) * (C.Y - A.Y);
        double temp2= (B.Y - A.Y) * (C.X - A.X);

        return abs(temp - temp2) * 0.5;
    }

private:
    std::map<QString,Point2D> points;

    //todo fill validate value function to avoid crash on reading from file
    bool validateParam(const QString &key, const QJsonValue &value) override {return true;};
};

class Square : public Shape{
public:
    Square() = delete;
    Square(const Point2D& _A,const Point2D& _B,const Point2D& _C  ){
        double side1 = _A.getDistance(_B);
        double side2 = _B.getDistance(_C);
        double diagonal = _A.getDistance(_C);

        bool haveDiffBetweenSide = fabs (side1 - side2 ) > localEpsilon;
        bool haveDiffWithDiagonal = fabs (sqrt ( side1 * side1 + side2 * side2 ) - diagonal) > localEpsilon;
        if ( haveDiffBetweenSide || haveDiffWithDiagonal )
            throw std::invalid_argument("Not a square.");

        setParam("Type", "Square");
        points["A"] = _A;
        setParam("A", _A);
        points["B"] = _B;
        setParam("B", _B);
        points["C"] = _C;
        setParam("C", _C);
    };
    double calcArea() const override{
        const Point2D & A = points.at("A");
        const Point2D & B = points.at("B");

        double side1 = A.getDistance(B);

        return side1 * side1;
    }

private:
    std::map<QString,Point2D> points;

    //todo fill validate value function to avoid crash on reading from file
    bool validateParam(const QString &key, const QJsonValue &value) override {return true;};
};

class Circle : public Shape{
public:
    Circle() = delete;
    Circle(const Point2D& _A,const Point2D& _B){
        setParam("Type", "Circle");

        points["A"] = _A;
        setParam("A", _A);
        points["B"] = _B;
        setParam("B", _B);
    };
    double calcArea() const override{
        const Point2D & A = points.at("A");
        const Point2D & B = points.at("B");

        double radius = A.getDistance(B);

        return M_PI_4 * radius * radius;
    }

private:
    std::map<QString,Point2D> points;

    //todo fill validate value function to avoid crash on reading from file
    bool validateParam(const QString &key, const QJsonValue &value) override {return true;};
};



#endif // SHAPE_H
