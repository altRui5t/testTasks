#ifndef TASKRESOLVER_H
#define TASKRESOLVER_H

#include <map>
#include <string>
#include <list>
#include <fstream>
#include <QJsonObject>
#include <QString>
struct Point {
  float x;
  float y;
};
struct Line
{
  Point begin;
  Point end;
};


class taskResolver
{
public:
    taskResolver();
    bool hasIntersection(const Line& line1, const Line& line2);
    bool IsEqual(const double& f1, const double& f2, const double& epsilon);

};

#endif // TASKRESOLVER_H
