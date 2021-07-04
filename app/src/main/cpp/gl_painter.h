//
// Created by Admin on 2021/6/24.
//

#ifndef PURENATIVECALCULATORDEMO_GL_PAINTER_H
#define PURENATIVECALCULATORDEMO_GL_PAINTER_H

#include <string>
#include "app_engine.h"
#include "rect_view.h"
#include "text_rect_view.h"
#include "view_util.h"
#include "painter_color.h"


class GlPainter{
private:
    struct engine* context;

public:

    GlPainter() = default;
    GlPainter(engine *engine);
    void drawRect(RectView rectView);
    void drawRect(TextRectView rectView);
    void drawRect(GLfloat vVertices[4], struct Painter_Color *color , const std::string& text);

    ~GlPainter();
};



#endif //PURENATIVECALCULATORDEMO_GL_PAINTER_H
