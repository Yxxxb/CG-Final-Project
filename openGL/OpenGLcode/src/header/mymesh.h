#ifndef MYMESH_H
#define MYMESH_H

#include <GL/freeglut.h>
#include <vector>
#include <string>
using namespace std;
class Mymesh{
public:
    Mymesh(string filename);//���캯��
    void Draw();//���ƺ���
private:
    vector<vector<GLfloat>>vSets;//��Ŷ���(x,y,z)����
    vector<vector<GLint>>fSets;//������������������
};
#endif // !MYMESH_H
#pragma once
