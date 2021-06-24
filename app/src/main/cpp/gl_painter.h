//
// Created by Admin on 2021/6/24.
//

#ifndef PURENATIVECALCULATORDEMO_GL_PAINTER_H
#define PURENATIVECALCULATORDEMO_GL_PAINTER_H

struct Painter_Color{
    float red;
    float green;
    float blue;
    float alpha;
};

void drawRectWithText(Painter_Color color , std::string text) {
    glColor4f(color.red,color.green,color.blue,color.alpha);

}


#endif //PURENATIVECALCULATORDEMO_GL_PAINTER_H
