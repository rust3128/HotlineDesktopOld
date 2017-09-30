#include "qdynamicbutton.h"

QDynamicButton::QDynamicButton(int brendID, QWidget *parent) :
    QPushButton(parent)
{
    buttonID = brendID;
}

QDynamicButton::~QDynamicButton()
{

}

int QDynamicButton::getButtonID()
{
    return buttonID;
}


QDynamicCheckButton::QDynamicCheckButton(int columnID, QWidget *parent) :
    QCheckBox(parent)
{
    buttonID = columnID;
}

QDynamicCheckButton::~QDynamicCheckButton()
{

}

int QDynamicCheckButton::getColumnID()
{
    return buttonID;
}

QDynamicRadioButton::QDynamicRadioButton(int pctypeID, QWidget *parent) :
    QRadioButton(parent)
{
    radioID = pctypeID;
}

QDynamicRadioButton::~QDynamicRadioButton()
{

}

int QDynamicRadioButton::gettypeID()
{
    return radioID;
}
