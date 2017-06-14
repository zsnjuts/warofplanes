#include "Control.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    Control *ctrl = new Control(800,600,3,
                                "images/myplane.png",50,"images/mybullet.png",1,1,
                                "images/enemyplane.png",1,"images/enemybullet.png",1,1);

    /* ÏÔÊ¾½çÃæ */
    QGraphicsView *view = new QGraphicsView;
    view->setScene(ctrl);
    view->show();

    return app.exec();
}
