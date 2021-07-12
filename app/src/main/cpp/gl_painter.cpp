//
// Created by 45010 on 2021/7/4.
//


#include <GLES3/gl3.h>
#include <memory>
#include "gl_painter.h"
#include "view_util.h"
#include "gl_vertices.h"
#include <string>



GlPainter::GlPainter(struct engine *engine) {
    this->context = engine;
}

GlPainter::~GlPainter() {
    free(context);
}



void GlPainter::drawRect(RectView rectView) {
    //view 顶点
    GlVertices glVertices[4];
    memset(glVertices,0, 4 * sizeof(GlVertices));
    //获取顶点
    ViewUtil::get_instance().getRectVertices(rectView,glVertices);

    //gl 顶点
    GLfloat v[12];
    memset(v,0, 12 * sizeof(GLfloat));
    //转换顶点
    ViewUtil::get_instance().transformGlVertices2float(glVertices,v);

    //转换颜色结构体
    struct Painter_Color painterColor{};
    //初始化一下
    memset(&painterColor,0,sizeof(Painter_Color));

    ViewUtil::get_instance().getPainterColor(rectView.getBgColor(),&painterColor);

    drawRect(v, &painterColor, "");
}

void GlPainter::drawRect(TextRectView rectView) {

    //转换顶点
    GlVertices glVertices[4];
    memset(glVertices,0, 4 * sizeof(GlVertices));
    ViewUtil::get_instance().getRectVertices(rectView,glVertices);

    GLfloat v[12];
    memset(v,0,12*sizeof(GLfloat));
    ViewUtil::get_instance().transformGlVertices2float(glVertices,v);

    //转换颜色
    struct Painter_Color painterColor{};
    memset(&painterColor,0,sizeof(Painter_Color));

    ViewUtil::get_instance().getPainterColor(rectView.getBgColor(),&painterColor);

    drawRect(v, &painterColor, rectView.getText());
}

void GlPainter::drawRect(GLfloat vVertices[4],struct Painter_Color *color, const std::string &text) {


    // 1. 通知OpenGL ES用于绘制的2D渲染表面的原点(x,y)坐标，宽度和高度
    glViewport(0,0,context->width,context->height);
    // 2. 清除颜色缓冲区；有颜色、深度和模板缓冲区
    glClear(GL_COLOR_BUFFER_BIT);
    // 3. 将程序设置为活程序
    glUseProgram(context->programObject);
    // void glVertexAttribPointer (GLuint index, GLint size, GLenum type, GLboolean normalized,
    // GLsizei stride, const void *pointer);
    // index为第几个属性，属性有顶点的位置为0/纹理为1/法线为3；size为一个顶点所有数据的个数，这里XYZ为3个；
    // type为顶点描述数据的类型，这里为FLOAT；normalized为是否需要显卡把数据归一化到-1到+1区间，这里不需要为FALSE
    // stride连续顶点属性之间的偏移量，0表示他们为紧密排列在一起的。pointer为顶点数组
    // 4. 加载顶点位置到GL中
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,0,vVertices);
    // 5. 使用顶点数组顶点位置属性，也就是0属性；如果什么都不使用的话，就使用常量顶点属性0
    glEnableVertexAttribArray(0);
    // 6. 设置线粗
    glLineWidth(10);
    // 7. 绘制矩形
    glDrawArrays(GL_LINE_LOOP, 0, 4);

}

void GlPainter::flushCanvas() {
    eglSwapBuffers(context->display,context->surface);
}


























