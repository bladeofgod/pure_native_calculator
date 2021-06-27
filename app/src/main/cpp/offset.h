//
// Created by 45010 on 2021/6/27.
//

#ifndef PURENATIVECALCULATORDEMO_OFFSET_H
#define PURENATIVECALCULATORDEMO_OFFSET_H

#include "rect_view.h"

class Offset{
public:
    float x;
    float y;

    Offset():x(0),y(0){}

    Offset(const Offset &o){
        this->x = o.x;
        this->y = o.y;
    }
    Offset(float a,float b):x(a),y(b){}

    Offset& operator= (const Offset &o){
        this->x = o.x;
        this->y = o.y;
        return *this;
    }

};



#endif //PURENATIVECALCULATORDEMO_OFFSET_H













