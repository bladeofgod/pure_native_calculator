//
// Created by 45010 on 2021/6/27.
//

#ifndef PURENATIVECALCULATORDEMO_GL_VERTICES_H
#define PURENATIVECALCULATORDEMO_GL_VERTICES_H

#include "offset.h"
#include "rect_view.h"

class GlVertices{
public:
    float x;
    float y;
    float z;

    GlVertices(){
        x = 0;
        y = 0;
        z = 0;
    }
    GlVertices(float x, float y){
        this->x = x;
        this->y = y;
        this->z = 0;
    }

    GlVertices(float x, float y, float z){
        this->x = x;
        this->y = y;
        this->z = z;
    }
};




#endif //PURENATIVECALCULATORDEMO_GL_VERTICES_H
