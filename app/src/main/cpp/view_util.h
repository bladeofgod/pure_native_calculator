//
// Created by 45010 on 2021/6/27.
//

#ifndef PURENATIVECALCULATORDEMO_VIEW_UTIL_H
#define PURENATIVECALCULATORDEMO_VIEW_UTIL_H

#include "gl_vertices.h"
#include "rect_view.h"
#include "offset.h"

/*
 *
 * @param1 rectView : a rect
 * @param2 width : screen width
 * @param3 height : screen height
 *
 */

GlVertices* getRectVertices(RectView rectView,float width,float height) {

    const float halfW = width / 2;
    const float halfH = height / 2;

    float x = rectView.getPosition().x;
    float y = rectView.getPosition().y;

    GlVertices leftTop = GlVertices();

    GlVertices rect[4];


}


GlVertices getGlLeftTop() {}



#endif //PURENATIVECALCULATORDEMO_VIEW_UTIL_H
