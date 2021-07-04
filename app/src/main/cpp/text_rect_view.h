//
// Created by 45010 on 2021/7/4.
//

#ifndef PURENATIVECALCULATORDEMO_TEXT_RECT_VIEW_H
#define PURENATIVECALCULATORDEMO_TEXT_RECT_VIEW_H

#include "rect_view.h"
#include <string>
#include "offset.h"


class TextRectView : public RectView{

private:
    std::string content;

public:
    TextRectView(std::string text, float w,float h,Offset position,float bg[4])
        : RectView(w, h, position, bg){
        this->content = text;
    }

    std::string getText() {
        return this->content;
    }

    void setText(std::string text) {
        this->content = text;
    }

};






#endif //PURENATIVECALCULATORDEMO_TEXT_RECT_VIEW_H
