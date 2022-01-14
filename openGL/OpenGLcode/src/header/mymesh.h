#ifndef MYMESH_H
#define MYMESH_H

#include <GL/freeglut.h>
#include <vector>
#include <string>
using namespace std;
class Mymesh{
public:
    Mymesh(string filename);//构造函数
    void Draw();//绘制函数
private:
    vector<vector<GLfloat>>vSets;//存放顶点(x,y,z)坐标
    vector<vector<GLint>>fSets;//存放面的三个顶点索引
};
#endif // !MYMESH_H
#pragma once
