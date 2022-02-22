#include "taskresolver.h"
#include <cmath>
#include <algorithm>
#include <QJsonDocument>
#include <QJsonValue>
#include <QJsonArray>
using namespace std;
const double epsilon =  1e-5;
const double doubleNull = 0.0;
taskResolver::taskResolver()
{



}
bool taskResolver::IsEqual(const double& f1, const double& f2, const double& epsilon){
    return abs(f1 - f2) <= epsilon * abs(f1);
}

bool taskResolver::hasIntersection(const Line& line1, const Line& line2){
    const Point& l1e = line1.end;
    const Point& l1b = line1.begin;
    const Point& l2e = line2.end;
    const Point& l2b = line2.begin;

    //определители
    double xN = (l2e.x - l2b.x) * (l1b.y - l2b.y) - (l2e.y - l2b.y) * (l1b.x - l2b.x);
    double yN = (l1e.x - l1b.x) * (l1b.y - l2b.y) - (l1e.y - l1b.y) * (l1b.x - l2b.x);
    double d  = (l2e.y - l2b.y) * (l1e.x - l1b.x) - (l2e.x - l2b.x) * (l1e.y - l1b.y);

    bool unableToCrossByX = max(min(l1b.x, l1e.x), min(l2b.x, l2e.x)) > min(max(l1b.x, l1e.x), max(l2b.x, l2e.x)); // проверяем потенциальную возможность пересечься по X
    bool unableToCrossByY =  max(min(l1b.y, l1e.y), min(l2b.y, l2e.y)) > min(max(l1b.y, l1e.y), max(l2b.y, l2e.y)); //проверяем потенциальную возможность пересечься по Y
    if ( unableToCrossByX || unableToCrossByY)
        return false;
    else if ( IsEqual(d,doubleNull, epsilon) ) //если отрезки лежат на параллельных прямых
        return IsEqual(xN,doubleNull, epsilon); //то проверяем - лежат ли они на одной прямой и имеют точку пересечения.
    else if (d < 0) {   //инверсия для универсализации проверки пересечения
        xN = - xN; yN = - yN; d = -d;
    }

    //отрезки не лежат на параллельных прямых и имеют возможность пересечения - проверяем, пересекаются ли они
    bool xnInRange = xN >= 0.0 and xN <= d;
    bool ynInRange = yN >= 0.0 and yN <= d;
    return xnInRange && ynInRange ;
}




