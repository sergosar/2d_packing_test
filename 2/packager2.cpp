#include "packager2.h"
#include "renderarea.h"


#include <QFile>
#include <QTextStream>
#include <QtAlgorithms>
#include <cmath>
#include <QtDebug>

#include <algorithm>
using namespace std;


void Packager2::setSTRIPH(int value)
{
    STRIPH = value;
}
void Packager2::setSTRIPW(int value)
{
    STRIPW = value;
}


void Packager2::init(QList<QSize> rects, int H, int W)
{
    _rectangles.clear();

    _rectangles = rects;
    unpacked=rects;
    setSTRIPH(H);
    setSTRIPW(W);
}

int Packager2::getSize(void)
{
    return rectangles.size();
}


vector<int> cornerEdgeDegree(QRect &r, QList<QRect> & list);
short int cornerDegree(vector<int>& v);
short int edgeDegree(vector<int>& v);


void Packager2::UseAlgorithm(void)
{
    rectangles.clear();

     // deleting oversizing rects from list
    for(auto rect: _rectangles) {
        if(rect.height()>STRIPH || rect.width()>STRIPW){
            _rectangles.removeOne(rect);
        }
    }

    //init start bounds rectangles
    rectangles.push_back(QRect(-5,-5, 5,STRIPH+10));
    rectangles.push_back(QRect(0,-5, STRIPW,5));
    rectangles.push_back(QRect(STRIPW,-5,5,STRIPH+10));
    rectangles.push_back(QRect(0,STRIPH,STRIPW,5));
    QList<QPoint> coaP;
    QList<CoaPoint> coaPointsList;
        // think about ..
    for(int i = 0; i<rectangles.size()-1; i++){
        for(int j=i+1; j<rectangles.size(); j++){
            QPoint coa = getIntersCoords(rectangles[i], rectangles[j]);
            if(coa.x()!=-1 && !coaP.contains(coa))   //нужно ли contains???
            {
                coaP.push_back(coa);
                coaPointsList.push_back(CoaPoint(rectangles[i],rectangles[j], coa));
            }
        }
    }

    //Basic programm

    while(true) {
           //step first
           //




        QList<CoAction> actions;
        for(auto coaPoint: coaPointsList)
        {
            for(auto& size : unpacked)
            {
                QRect r1= QRect(coaPoint.point.x(),coaPoint.point.y(), size.width(),size.height());
                QRect r2= QRect(coaPoint.point.x(),coaPoint.point.y(), size.height(),size.width());
                if(feasibleCOA(r1,coaPoint))
                    actions.push_back(CoAction(coaPoint, r1));
                if(feasibleCOA(r2,coaPoint))
                    actions.push_back(CoAction(coaPoint, r2));
            }
        }
        //No feasible COA? break
        if(actions.size()==0) break;
        //calculation  cavingDegree, cornerDegree, edgeDegree;
        //cavingDegree

        for(auto& action: actions) {
            action.cavingDegree=calcCavingDegree(action);
            vector<int> edges = cornerEdgeDegree(action.testRect, rectangles);
            action.cornerDegree=cornerDegree(edges);
            action.edgeDegree = edgeDegree(edges);
        }

        //Step 2 sort by
        auto coaSort = [](const CoAction &a1, const CoAction& a2)
        {

            if(a1.cavingDegree!=a2.cavingDegree)
                return a1.cavingDegree>a2.cavingDegree;
//           else if(a1.testRect.y()>a2.testRect.y()&&
//                    a1.testRect.height()*a1.testRect.width()==a2.testRect.height()*
//                    a2.testRect.width())
//                return a1.testRect.bottom()>a2.testRect.bottom();
            else if(a1.cornerDegree!=a2.cornerDegree)
                return a1.cornerDegree>a2.cornerDegree;
            else if(a1.edgeDegree!=a2.edgeDegree)
                return a1.edgeDegree>a2.edgeDegree;
            else if(a1.testRect.bottom()!=a2.testRect.bottom())
                return a1.testRect.bottom()<a2.testRect.bottom();
            else if(a1.cPoint.point.y()!=a2.cPoint.point.y())
                return a1.cPoint.point.y()<a2.cPoint.point.y();
            else if(a1.cPoint.point.x()!=a2.cPoint.point.x())
                return a1.cPoint.point.x()<a2.cPoint.point.x();
            else if (a1.testRect.width()*a1.testRect.height()!=a2.testRect.width()*a2.testRect.height())
                return a1.testRect.width()*a1.testRect.height()>a2.testRect.width()*a2.testRect.height();
            else return a1.testRect.width()>a2.testRect.width();

        };
        sort(actions.begin(),actions.end(),coaSort);
//        if(actions.size()>1)
//        while(actions[0].testRect.bottom()>actions[1].testRect.bottom()&&
//                actions[0].testRect.width()*actions[0].testRect.height()==
//                actions[1].testRect.width()*actions[1].testRect.height())
//            actions.removeFirst();


        rectangles.push_back(actions[0].testRect);
        unpacked.removeOne(QSize((min(actions[0].testRect.width(),actions[0].testRect.height())),
                max(actions[0].testRect.width(),actions[0].testRect.height())));

        QPoint removePoint= rectangles.last().topLeft();
        QMutableListIterator<CoaPoint> it(coaPointsList);

        while(it.hasNext()){
            if(it.next().point==removePoint) it.remove();
        }

        for(int i = 0; i<rectangles.size()-1; i++){

            QPoint coa = getIntersCoords(rectangles[i], rectangles.last());
            if(coa.x()!=-1 && !coaP.contains(coa))   //нужно ли contains???
            {
                coaP.push_back(coa);
                coaPointsList.push_back(CoaPoint(rectangles[i],rectangles.last(), coa));
            }
        }





//        qDebug()<<coaPointsList.size();

    }

}



//    pack(_rectangles,STRIPH, STRIPW);

//    for(auto rect : rectangles){
//        QRect rectd;
//        rectd.setRect(0,0,rect.width(),rect.height());
//        unpacked.removeOne(rectd);
//    }




bool decreasingComparsion(const QRect r1, const QRect r2)
{
    if(r1.height()!=r2.height())
        return r1.height() > r2.height();
    else return r1.width()>=r2.width();
}

QList<QString> Packager2::unList()
{
    if (unpacked.empty())
        return QList<QString>();

 //   qSort( unpacked.begin(), unpacked.end(), decreasingComparsion);
    QList<QString> uList;
    QString temp ="";
//    for(QRect rect: unpacked){
//        if(temp.isEmpty()) {
//            temp = QString::number(rect.height()) + " " + QString::number(rect.width())+ " " + "1";
//        } else if(temp.split(" ")[0].toInt()==rect.height() && temp.split(" ")[1].toInt()==rect.width()) {
//            temp=QString::number(rect.height()) + " " + QString::number(rect.width())+ " " + QString::number(temp.split(" ")[2].toInt()+1);
//        } else {
//            uList.push_back(temp);
//            temp=QString::number(rect.height()) + " " + QString::number(rect.width())+ " " + "1";
//        }
//    }
    uList.push_back(temp);
    return uList;
}

QPoint Packager2::getIntersCoords(QRect& r1, QRect& r2){
    if((r2.bottom()<r1.bottom() && r2.bottom()>=r1.top()-1 && r2.right()>r1.right()) ||
            (r1.right()<r2.right() && r1.right()>=r2.left() && r1.bottom()> r2.bottom()))
        return  QPoint(r1.right()+1, r2.bottom()+1);
    else if((r1.bottom()<r2.bottom() && r1.bottom()>=r2.top()-1 && r1.right()>r2.right())||
            (r2.right()<r1.right() && r2.right()>=r1.left() && r2.bottom()> r1.bottom()))
        return  QPoint(r2.right()+1, r1.bottom()+1);
  //  else if()

    return QPoint(-1,-1);
}

bool Packager2::edgeOverlap(QRect& r1,QRect& r2) {
    // наложение по вернехней или нижней грани
    if((r1.top()==r2.bottom()+1 || r1.bottom()+1 ==r2.top()) &&
            (max(r1.right(), r2.right())-min(r1.left(),r2.left())+1)<
            (r1.width()+r2.width()))
        return true;
    //наложение по правол левой грани
    if((r1.left()==r2.right()+1 || r1.right()+1 == r2.left()) &&
            (max(r1.bottom(), r2.bottom())-min(r1.top(),r2.top())+1)< (r1.height()+r2.height()))
        return true;
    return false;
}

bool Packager2::Intersected(QRect &r){
    bool intersected=false;
    for(auto rect: rectangles){
        if(rect.intersects(r)){
            intersected=true;
            break;
        }
    }
    return intersected;

}

bool Packager2:: feasibleCOA(QRect& r, CoaPoint& cPoint ){
    if(edgeOverlap(r, cPoint.r1) && edgeOverlap(r, cPoint.r2) && !Intersected(r))
        return true;
    return false;
}

double Packager2::calcCavingDegree(CoAction& action){
    double d=10000.0;
    for(auto& rect: rectangles){
        if(action.cPoint.r1!=action.testRect &&
                action.cPoint.r2!=action.testRect)
            if(int a=dist(rect, action.testRect)<d)
                d=a;

    }
    return 1-d/sqrt(action.testRect.width()*action.testRect.height());

}

double xc(QRect& r){
    return (0.0+r.right()+1+r.left())/2;
}

double yc(QRect& r){
    return (0.0+r.bottom()+r.top()+1)/2;
}
double Packager2::dist(QRect& r1, QRect& r2){
    double x1=xc(r1);
    double x2 =xc(r2);
    double y1 = yc(r1);
    double y2 = yc(r2);

    return fmax(fabs(x1-x2)-(r1.width()+r2.width())/2.0,0)+
            fmax(fabs(y1-y2)-(r1.height()+r2.height())/2.0,0);

};

vector<int> cornerEdgeDegree(QRect &r, QList<QRect> & list){
    //left,  right, bottom, top (in classic)
    vector<int> result= {0,0,0,0};
    for(auto& rect:list)
    {
        if((max(r.bottom(), rect.bottom())-min(r.top(),rect.top())+1)< (r.height()+rect.height()))
        {
            if(r.left()==rect.right()+1) result[0]+=1;
            else if(r.right()+1 == rect.left()) result[1]+=1;
        }
        else if((max(r.right(), rect.right())-min(r.left(),rect.left())+1)<
            (r.width()+rect.width()))
        {
            if(r.top()==rect.bottom()+1) result[2]+=1;
            else if(r.bottom()+1 ==rect.top()) result[3]+=1;
        }
    }
    return result;
}
short int cornerDegree(vector<int>& v){
    short int res=2;
    if(v[1]>0) res+=1;
    if(v[3]>0)  res+=1;
    return res;
}
short int edgeDegree(vector<int>& v){
    short int res=0;
    for(int i=0;i<4;i++){
        res+=v[i];
    }
    return res;
}
