//
// Created by 45010 on 2021/6/27.
//

#ifndef PURENATIVECALCULATORDEMO_VIEW_UTIL_H
#define PURENATIVECALCULATORDEMO_VIEW_UTIL_H

#include "gl_vertices.h"
#include "rect_view.h"
#include "offset.h"
#include <cmath>

/*
 *
 * transform screen coordinator to opengl coordinator
 *
 */

class ViewUtil{
public:
    float screenWidth;
    float screenHeight;

    // middle of the screen
    // offset(left,top)
    Offset screenCenter;

    ViewUtil():screenWidth(0.0),screenHeight(0.0),screenCenter(0.0,0.0){}

    ViewUtil(float width,float height) {
        screenCenter = Offset(width/2,height/2);
    }

    void getRectVertices(RectView rectView, GlVertices *rectLTRB) {

        float x = rectView.getPosition().x;
        float y = rectView.getPosition().y;
        GlVertices leftTop = transformCSOffset(x,y);

        float a = rectView.getPosition().x + rectView.getWidth();
        float b = rectView.getPosition().y;
        GlVertices rightTop = transformCSOffset(a,b);

        float f = rectView.getPosition().x;
        float g = rectView.getPosition().y + rectView.getHeight();
        GlVertices leftBottom = transformCSOffset(f,g);

        float j = rectView.getPosition().x + rectView.getWidth();
        float k = rectView.getPosition().y + rectView.getHeight();
        GlVertices rightBottom = transformCSOffset(j,k);

        GlVertices rect[4] = {leftBottom,leftTop,rightBottom,rightTop};
        for(int i = 0; i < 4; i++) {
            *rectLTRB = rect[i];
            rectLTRB ++;
        }
        //free(rect);
    }

private:


    /*
     * transform coordinate system
     */
    GlVertices transformCSOffset(float x, float y) {
        float left = (x - screenCenter.x) / screenCenter.x;
        float top = (y - screenCenter.y) / screenCenter.y;
        return GlVertices(left,top);
    }

};




#endif //PURENATIVECALCULATORDEMO_VIEW_UTIL_H
