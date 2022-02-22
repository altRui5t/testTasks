#include "shape.h"

#include <cmath>
#include <algorithm>
#include <QJsonDocument>
#include <QJsonValue>
#include <QJsonArray>
#include <sstream>

const double epsilon =  1e-5;

const std::map<QString, int> Shape::figureName = {{"Triangle",0},{"Square",1},{"Circle",2}};
namespace ShapeName{
    enum ShapeTypes {Triangle, Square, Circle} ;
}

Shape::Shape()
{

}

bool Shape::knowShape(const QString& type){
    auto index = figureName.find(type);
    return index != figureName.end();
}


bool Shape::setParam(const QString& key, const QJsonValue& value){
    bool Result = validateParam(key, value);

    if (Result)
        parameters[key] = value;
    return Result;
}

QString Shape::getParam(const QString& key){
    if (parameters.contains(key))
        return parameters[key].toString();
    else{
        QString reason = "Not defined key: " + key;
        throw std::invalid_argument(reason.toStdString());
    }
}

bool Shape::operator<(const std::list<std::shared_ptr<Shape>>& Shapes){
    double squareThis = this->calcArea();
    double listSquare = 0.0;
    for (const auto &shape : Shapes){
        listSquare += shape->calcArea();
        if (squareThis < listSquare)
            return true;
    }
    return false;
}

bool Shape::operator>(const std::list<std::shared_ptr<Shape>>& Shapes){
    double squareThis = this->calcArea();
    double listSquare = 0.0;
    for (const auto &shape : Shapes){
        listSquare += shape->calcArea();
        if (squareThis <= listSquare)
            return false;
    }
    return true;
}

bool Shape::operator<=(const std::list<std::shared_ptr<Shape>>& Shapes){
    return !(*this > Shapes);
}

bool Shape::operator>=(const std::list<std::shared_ptr<Shape>>& Shapes){
    return !(*this < Shapes);
}


bool Shape::operator==(const std::list<std::shared_ptr<Shape>>& Shapes){
    double squareThis = this->calcArea();
    double listSquare = 0.0;
    for (const auto &shape : Shapes){
        listSquare += shape->calcArea();
        if (squareThis < listSquare)
            return false;
    }
    return abs(squareThis - listSquare) < epsilon * abs(squareThis);
}

QString Shape::toJsonString() const {
    //todo should think about include json serialize format
    QJsonDocument doc(parameters);
    return doc.toJson();
}



bool writeShapesToFile(const std::list<std::shared_ptr<Shape>>& Shapes, std::ofstream& outFileStream){
    if (!outFileStream.is_open())
        return false;
    outFileStream << "{\"Shapes\":[ ";
    for (const auto &shape : Shapes){
     outFileStream << shape->toJsonString().toStdString() << ",";
    }
    outFileStream.seekp(-1,std::ios_base::cur);
    outFileStream << "]}" << std::endl;

    return true;
}
std::list<std::shared_ptr<Shape>> readShapesFromFile(std::ifstream& inFileStream){
    if (!inFileStream.is_open())
        throw std::invalid_argument("Filestream is not open!");

    std::list<std::shared_ptr<Shape>> Result;

    std::stringstream info;
    info << inFileStream.rdbuf();
    std::string buffer = info.str();

    QJsonDocument document = QJsonDocument().fromJson(QByteArray(buffer.data(), buffer.size()));
    if (document.isNull()){
        throw std::logic_error("No json information in ifstream!");
    }
    if (document.isObject())
        if (!document.object().contains("Shapes"))
            throw std::logic_error("Wrong json format in filestream!");

    QJsonArray shapes = document["Shapes"].toArray();

    for (auto elements : shapes) {
        QJsonObject shape = elements.toObject();
        if (!shape.contains("Type"))
            continue; // possible add message to std::err
        QString type = shape["Type"].toString();

        if (!Shape::knowShape(type))
            continue; // possible add message to std::err
        try{
            switch (Shape::figureName.at(type)){
                case ShapeName::Triangle :
                    //todo need check consistence of json before call constructor
                    Result.push_back(std::make_shared<Triangle>(Point2D(shape["A"].toObject()), Point2D(shape["B"].toObject()), Point2D(shape["C"].toObject())));
                    break;
                case ShapeName::Square :
                    //todo need check consistence of json before call constructor
                    Result.push_back(std::make_shared<Square>(Point2D(shape["A"].toObject()), Point2D(shape["B"].toObject()), Point2D(shape["C"].toObject())));
                break;
                case ShapeName::Circle :
                    //todo need check consistence of json before call constructor
                    Result.push_back(std::make_shared<Circle>(Point2D(shape["A"].toObject()), Point2D(shape["B"].toObject())));
                break;
                default:
                    continue;
            }
        }catch(const std::invalid_argument& e){
            //todo inform about errors
        }
    }

    return Result;
}
