//
// Created by 45010 on 2021/6/27.
//

#ifndef PURENATIVECALCULATORDEMO_VIEW_UTIL_H
#define PURENATIVECALCULATORDEMO_VIEW_UTIL_H

#include "gl_vertices.h"
#include "rect_view.h"
#include "offset.h"
#include <cmath>
#include <iostream>
#include "gl_painter.h"
#include "painter_color.h"

/*
 *
 * transform screen coordinator to opengl coordinator
 *
 */

class ViewUtil{

private:
    ViewUtil() {
        std::cout<<"constructor called !"<<std::endl;
    }
    //表示删除默认拷贝构造函数，即不能进行默认拷贝
    ViewUtil(ViewUtil&)=delete;
    //禁止赋值拷贝
    ViewUtil& operator=(const ViewUtil&)=delete;

    ViewUtil(float width,float height) {
        this->screenWidth = width;
        this->screenHeight = height;
        screenCenter = Offset(width/2,height/2);
    }

    static ViewUtil* m_instance_ptr;

    /*
     * transform coordinate system
     */
    GlVertices transformCSOffset(float x, float y) {
        float left = (x - screenCenter.x) / screenCenter.x;
        float top = (y - screenCenter.y) / screenCenter.y;
        return GlVertices(left,top);
    }

public:
    float screenWidth;
    float screenHeight;

    // middle of the screen
    // offset(left,top)
    Offset screenCenter;

    ~ViewUtil(){
        std::cout<<"destructor called !"<<std::endl;
    }

    static void init(float width,float height) {
        if(m_instance_ptr == nullptr) {
            m_instance_ptr = new ViewUtil(width,height);
        }
    }

    static ViewUtil* get_instance() {
        if(m_instance_ptr == nullptr) {
            m_instance_ptr = new ViewUtil(0,0);
        }
        return m_instance_ptr;
    }

    /*
     * get GlVertices[4] by rectView's size and position
     *
     */

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

    /*
     * transform GlVertices[4] to float[12]
     *
     */

    void transformGlVertices2float(GlVertices glVertices[4], float v[12]) {
        int j = 0;
        for(int i = 0;i < 12 ; ) {
            GlVertices *t = &glVertices[j];
            v[i] = t->x;
            v[i+1] = t->y;
            v[i+2] = t->z;
            i +=3;
            j++;
        }
    }

    /*
     * transform color[4] to Painter_Color
     *
     */
    void getPainterColor(float color[4] , struct Painter_Color *painterColor) throw(std::string) {

        for(int i = 0; i < 4;i++) {
            if(i < 3) {
                if(color[i] > 255) {
                    throw "color's value must smaller than 255";
                }
            } else {
                if(color[4] > 1) {
                    throw "alpha's value must smaller than 1";
                }
            }
        }
        painterColor->red       = color[0];
        painterColor->green     = color[1];
        painterColor->blue      = color[2];
        painterColor->alpha     = color[3];

    }



};




#endif //PURENATIVECALCULATORDEMO_VIEW_UTIL_H
