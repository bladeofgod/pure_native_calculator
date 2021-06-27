//
// Created by 45010 on 2021/6/27.
//

#ifndef PURENATIVECALCULATORDEMO_RECT_VIEW_H
#define PURENATIVECALCULATORDEMO_RECT_VIEW_H


#include "offset.h"


/*
 * gl coordinator :
 *
 * [-1.0,1.0]---------------------[1.0,1.0]
 *
 *
 *
 *                   [0.0,0.0]
 *
 *
 *
 * [-1.0,-1.0]---------------------[1.0,-1.0]
 *
 */

class RectView{
private:
    float width;
    float height;
    // [[-1.0,-1.0],[1.0,1.0]]
    // x = left ,y = top
    Offset position;
    //r g b a
    float bgColor[4];

public:
    RectView(float w,float h,Offset position,float bg[4]) {
        width = w;
        height = h;
        this->position = position;
        memset(bgColor,0,4 * sizeof(float ));
        memcpy(bgColor,bg,4 * sizeof(float ));
    }

    ~RectView(){
        free(bgColor);
        free(&position);
    }

    Offset getPosition() {
        return position;
    }

    float getWidth() {
        return width;
    }

    float getHeight() {
        return height;
    }

    float* getBgColor() {
        return bgColor;
    }

};



#endif //PURENATIVECALCULATORDEMO_RECT_VIEW_H





