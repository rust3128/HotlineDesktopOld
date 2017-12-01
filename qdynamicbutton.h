#ifndef QDYNAMICBUTTON_H
#define QDYNAMICBUTTON_H

#include <QPushButton>
#include <QCommandLinkButton>
#include <QCheckBox>
#include <QRadioButton>


class QDynamicButton : public QPushButton
{
public:
    explicit QDynamicButton(int brendID, QWidget *parent =0);
    ~QDynamicButton();
    int getButtonID(); //Функция возврата локального номер кнопки


private:
    int buttonID = 0;   //Номер кнопки
};


class QDynamicCheckButton : public QCheckBox
{
public:
    explicit QDynamicCheckButton(int columnID, QWidget *parent =0);
    ~QDynamicCheckButton();
    int getColumnID(); //Функция возврата локального номер кнопки


private:
    int buttonID = 0;   //Номер кнопки
};


class QDynamicRadioButton : public QRadioButton
{
public:
    explicit QDynamicRadioButton(int pctypeID, QWidget *parent =0);
    ~QDynamicRadioButton();
    int gettypeID(); //Функция возврата локального номер кнопки


private:
    int radioID = 0;   //Номер кнопки
};

#endif // QDYNAMICBUTTON_H
