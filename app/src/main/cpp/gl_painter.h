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

//    const GLfloat triangleVertices[] = {
//            0.0, 1.0, -6.0,// Triangle top centre
//            -1.0, -1.0, -6.0,// bottom left
//            1.0, -1.0, -6.0// bottom right
//            };

//    GLint vertices[] = { 25, 25,
//                         100, 325,
//                         175, 25,
//                         175, 325,
//                         250, 25,
//                         325, 325 };
//
//    glColor4f(color.red,color.green,color.blue,color.alpha);
//    glVertexPointer(3,GL_FLOAT,0,triangleVertices);
//    glEnableClientState(GL_VERTEX_ARRAY);
//    glDrawArrays(GL_TRIANGLES,0,9);
//    glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_SHORT, triangleVertices);

}






#endif //PURENATIVECALCULATORDEMO_GL_PAINTER_H
