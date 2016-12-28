#include "keyboardform.h"
#include "ui_keyboardform.h"

KeyBoardForm::KeyBoardForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::KeyBoardForm)
{
    ui->setupUi(this);

/*####################标题######################*/

    signalMapper = new QSignalMapper;

    signalMapper->setMapping(ui->pushButton_0, QString("0"));
    signalMapper->setMapping(ui->pushButton_1, QString("1"));
    signalMapper->setMapping(ui->pushButton_2, QString("2"));
    signalMapper->setMapping(ui->pushButton_3, QString("3"));
    signalMapper->setMapping(ui->pushButton_4, QString("4"));
    signalMapper->setMapping(ui->pushButton_5, QString("5"));
    signalMapper->setMapping(ui->pushButton_6, QString("6"));
    signalMapper->setMapping(ui->pushButton_7, QString("7"));
    signalMapper->setMapping(ui->pushButton_8, QString("8"));
    signalMapper->setMapping(ui->pushButton_9, QString("9"));

    signalMapper->setMapping(ui->pushButton_A, QString("A"));
    signalMapper->setMapping(ui->pushButton_B, QString("B"));
    signalMapper->setMapping(ui->pushButton_C, QString("C"));
    signalMapper->setMapping(ui->pushButton_D, QString("D"));
    signalMapper->setMapping(ui->pushButton_E, QString("E"));
    signalMapper->setMapping(ui->pushButton_F, QString("F"));
    signalMapper->setMapping(ui->pushButton_G, QString("G"));
    signalMapper->setMapping(ui->pushButton_H, QString("H"));
    signalMapper->setMapping(ui->pushButton_I, QString("I"));
    signalMapper->setMapping(ui->pushButton_J, QString("J"));
    signalMapper->setMapping(ui->pushButton_K, QString("K"));
    signalMapper->setMapping(ui->pushButton_L, QString("L"));
    signalMapper->setMapping(ui->pushButton_M, QString("M"));
    signalMapper->setMapping(ui->pushButton_N, QString("N"));
    signalMapper->setMapping(ui->pushButton_O, QString("O"));
    signalMapper->setMapping(ui->pushButton_P, QString("P"));
    signalMapper->setMapping(ui->pushButton_Q, QString("Q"));
    signalMapper->setMapping(ui->pushButton_R, QString("R"));
    signalMapper->setMapping(ui->pushButton_S, QString("S"));
    signalMapper->setMapping(ui->pushButton_T, QString("T"));
    signalMapper->setMapping(ui->pushButton_U, QString("U"));
    signalMapper->setMapping(ui->pushButton_V, QString("V"));
    signalMapper->setMapping(ui->pushButton_W, QString("W"));
    signalMapper->setMapping(ui->pushButton_X, QString("X"));
    signalMapper->setMapping(ui->pushButton_Y, QString("Y"));
    signalMapper->setMapping(ui->pushButton_Z, QString("Z"));
    signalMapper->setMapping(ui->pushButton_GANG, QString("-"));
    signalMapper->setMapping(ui->pushButton_XIE, QString("/"));
    signalMapper->setMapping(ui->pushButton_DOU, QString(","));
    signalMapper->setMapping(ui->pushButton_POINT, QString("."));

    signalMapper->setMapping(ui->pushButton_BANK, QString("\ "));

    connect(ui->pushButton_0, SIGNAL(clicked()),signalMapper, SLOT (map()));
    connect(ui->pushButton_1, SIGNAL(clicked()),signalMapper, SLOT (map()));
    connect(ui->pushButton_2, SIGNAL(clicked()),signalMapper, SLOT (map()));
    connect(ui->pushButton_3, SIGNAL(clicked()),signalMapper, SLOT (map()));
    connect(ui->pushButton_4, SIGNAL(clicked()),signalMapper, SLOT (map()));
    connect(ui->pushButton_5, SIGNAL(clicked()),signalMapper, SLOT (map()));
    connect(ui->pushButton_6, SIGNAL(clicked()),signalMapper, SLOT (map()));
    connect(ui->pushButton_7, SIGNAL(clicked()),signalMapper, SLOT (map()));
    connect(ui->pushButton_8, SIGNAL(clicked()),signalMapper, SLOT (map()));
    connect(ui->pushButton_9, SIGNAL(clicked()),signalMapper, SLOT (map()));

    connect(ui->pushButton_A, SIGNAL(clicked()),signalMapper, SLOT (map()));
    connect(ui->pushButton_B, SIGNAL(clicked()),signalMapper, SLOT (map()));
    connect(ui->pushButton_C, SIGNAL(clicked()),signalMapper, SLOT (map()));
    connect(ui->pushButton_D, SIGNAL(clicked()),signalMapper, SLOT (map()));
    connect(ui->pushButton_E, SIGNAL(clicked()),signalMapper, SLOT (map()));
    connect(ui->pushButton_F, SIGNAL(clicked()),signalMapper, SLOT (map()));
    connect(ui->pushButton_G, SIGNAL(clicked()),signalMapper, SLOT (map()));
    connect(ui->pushButton_H, SIGNAL(clicked()),signalMapper, SLOT (map()));
    connect(ui->pushButton_I, SIGNAL(clicked()),signalMapper, SLOT (map()));
    connect(ui->pushButton_J, SIGNAL(clicked()),signalMapper, SLOT (map()));
    connect(ui->pushButton_K, SIGNAL(clicked()),signalMapper, SLOT (map()));
    connect(ui->pushButton_L, SIGNAL(clicked()),signalMapper, SLOT (map()));
    connect(ui->pushButton_M, SIGNAL(clicked()),signalMapper, SLOT (map()));
    connect(ui->pushButton_N, SIGNAL(clicked()),signalMapper, SLOT (map()));
    connect(ui->pushButton_O, SIGNAL(clicked()),signalMapper, SLOT (map()));
    connect(ui->pushButton_P, SIGNAL(clicked()),signalMapper, SLOT (map()));
    connect(ui->pushButton_Q, SIGNAL(clicked()),signalMapper, SLOT (map()));
    connect(ui->pushButton_R, SIGNAL(clicked()),signalMapper, SLOT (map()));
    connect(ui->pushButton_S, SIGNAL(clicked()),signalMapper, SLOT (map()));
    connect(ui->pushButton_T, SIGNAL(clicked()),signalMapper, SLOT (map()));
    connect(ui->pushButton_U, SIGNAL(clicked()),signalMapper, SLOT (map()));
    connect(ui->pushButton_V, SIGNAL(clicked()),signalMapper, SLOT (map()));
    connect(ui->pushButton_W, SIGNAL(clicked()),signalMapper, SLOT (map()));
    connect(ui->pushButton_X, SIGNAL(clicked()),signalMapper, SLOT (map()));
    connect(ui->pushButton_Y, SIGNAL(clicked()),signalMapper, SLOT (map()));
    connect(ui->pushButton_Z, SIGNAL(clicked()),signalMapper, SLOT (map()));

    connect(ui->pushButton_GANG, SIGNAL(clicked()),signalMapper, SLOT (map()));
    connect(ui->pushButton_XIE, SIGNAL(clicked()),signalMapper, SLOT (map()));
    connect(ui->pushButton_DOU, SIGNAL(clicked()),signalMapper, SLOT (map()));
    connect(ui->pushButton_POINT, SIGNAL(clicked()),signalMapper, SLOT (map()));

    connect(ui->pushButton_BANK, SIGNAL(clicked()),signalMapper, SLOT (map()));

    connect(signalMapper, SIGNAL(mapped(QString)),this,SIGNAL(sendWord(QString)));

    connect(ui->pushButton_DEL,SIGNAL(clicked()),this,SIGNAL(delWord()));

    connect(ui->pushButton_CAP,SIGNAL(clicked()),this,SLOT(setCapsLock()));

    isCaps = true;

    ui->pushButton_A->setFocus();
}

KeyBoardForm::~KeyBoardForm()
{
    delete ui;
}

void KeyBoardForm::setCapsLock()
{
    if(isCaps)
    {

        ui->pushButton_A->setText("a");
        ui->pushButton_B->setText("b");
        ui->pushButton_C->setText("c");
        ui->pushButton_D->setText("d");
        ui->pushButton_E->setText("e");
        ui->pushButton_F->setText("f");
        ui->pushButton_G->setText("g");
        ui->pushButton_H->setText("h");
        ui->pushButton_I->setText("i");
        ui->pushButton_J->setText("j");
        ui->pushButton_K->setText("k");
        ui->pushButton_L->setText("l");
        ui->pushButton_M->setText("m");
        ui->pushButton_N->setText("n");
        ui->pushButton_O->setText("o");
        ui->pushButton_P->setText("p");
        ui->pushButton_Q->setText("q");
        ui->pushButton_R->setText("r");
        ui->pushButton_S->setText("s");
        ui->pushButton_T->setText("t");
        ui->pushButton_U->setText("u");
        ui->pushButton_V->setText("v");
        ui->pushButton_W->setText("w");
        ui->pushButton_X->setText("x");
        ui->pushButton_Y->setText("y");
        ui->pushButton_Z->setText("z");

        signalMapper->setMapping(ui->pushButton_A, QString("a"));
        signalMapper->setMapping(ui->pushButton_B, QString("b"));
        signalMapper->setMapping(ui->pushButton_C, QString("c"));
        signalMapper->setMapping(ui->pushButton_D, QString("d"));
        signalMapper->setMapping(ui->pushButton_E, QString("e"));
        signalMapper->setMapping(ui->pushButton_F, QString("f"));
        signalMapper->setMapping(ui->pushButton_G, QString("g"));
        signalMapper->setMapping(ui->pushButton_H, QString("h"));
        signalMapper->setMapping(ui->pushButton_I, QString("i"));
        signalMapper->setMapping(ui->pushButton_J, QString("j"));
        signalMapper->setMapping(ui->pushButton_K, QString("k"));
        signalMapper->setMapping(ui->pushButton_L, QString("l"));
        signalMapper->setMapping(ui->pushButton_M, QString("m"));
        signalMapper->setMapping(ui->pushButton_N, QString("n"));
        signalMapper->setMapping(ui->pushButton_O, QString("o"));
        signalMapper->setMapping(ui->pushButton_P, QString("p"));
        signalMapper->setMapping(ui->pushButton_Q, QString("q"));
        signalMapper->setMapping(ui->pushButton_R, QString("r"));
        signalMapper->setMapping(ui->pushButton_S, QString("s"));
        signalMapper->setMapping(ui->pushButton_T, QString("t"));
        signalMapper->setMapping(ui->pushButton_U, QString("u"));
        signalMapper->setMapping(ui->pushButton_V, QString("v"));
        signalMapper->setMapping(ui->pushButton_W, QString("w"));
        signalMapper->setMapping(ui->pushButton_X, QString("x"));
        signalMapper->setMapping(ui->pushButton_Y, QString("y"));
        signalMapper->setMapping(ui->pushButton_Z, QString("z"));

        isCaps = false;
    }
    else
    {
        ui->pushButton_A->setText("A");
        ui->pushButton_B->setText("B");
        ui->pushButton_C->setText("C");
        ui->pushButton_D->setText("D");
        ui->pushButton_E->setText("E");
        ui->pushButton_F->setText("F");
        ui->pushButton_G->setText("G");
        ui->pushButton_H->setText("H");
        ui->pushButton_I->setText("I");
        ui->pushButton_J->setText("J");
        ui->pushButton_K->setText("K");
        ui->pushButton_L->setText("L");
        ui->pushButton_M->setText("M");
        ui->pushButton_N->setText("N");
        ui->pushButton_O->setText("O");
        ui->pushButton_P->setText("P");
        ui->pushButton_Q->setText("Q");
        ui->pushButton_R->setText("R");
        ui->pushButton_S->setText("S");
        ui->pushButton_T->setText("T");
        ui->pushButton_U->setText("U");
        ui->pushButton_V->setText("V");
        ui->pushButton_W->setText("W");
        ui->pushButton_X->setText("X");
        ui->pushButton_Y->setText("Y");
        ui->pushButton_Z->setText("Z");

        signalMapper->setMapping(ui->pushButton_A, QString("A"));
        signalMapper->setMapping(ui->pushButton_B, QString("B"));
        signalMapper->setMapping(ui->pushButton_C, QString("C"));
        signalMapper->setMapping(ui->pushButton_D, QString("D"));
        signalMapper->setMapping(ui->pushButton_E, QString("E"));
        signalMapper->setMapping(ui->pushButton_F, QString("F"));
        signalMapper->setMapping(ui->pushButton_G, QString("G"));
        signalMapper->setMapping(ui->pushButton_H, QString("H"));
        signalMapper->setMapping(ui->pushButton_I, QString("I"));
        signalMapper->setMapping(ui->pushButton_J, QString("J"));
        signalMapper->setMapping(ui->pushButton_K, QString("K"));
        signalMapper->setMapping(ui->pushButton_L, QString("L"));
        signalMapper->setMapping(ui->pushButton_M, QString("M"));
        signalMapper->setMapping(ui->pushButton_N, QString("N"));
        signalMapper->setMapping(ui->pushButton_O, QString("O"));
        signalMapper->setMapping(ui->pushButton_P, QString("P"));
        signalMapper->setMapping(ui->pushButton_Q, QString("Q"));
        signalMapper->setMapping(ui->pushButton_R, QString("R"));
        signalMapper->setMapping(ui->pushButton_S, QString("S"));
        signalMapper->setMapping(ui->pushButton_T, QString("T"));
        signalMapper->setMapping(ui->pushButton_U, QString("U"));
        signalMapper->setMapping(ui->pushButton_V, QString("V"));
        signalMapper->setMapping(ui->pushButton_W, QString("W"));
        signalMapper->setMapping(ui->pushButton_X, QString("X"));
        signalMapper->setMapping(ui->pushButton_Y, QString("Y"));
        signalMapper->setMapping(ui->pushButton_Z, QString("Z"));

        isCaps = true;
    }
}

