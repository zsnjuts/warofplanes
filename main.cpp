#include "Control.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    /* 启动画面 */
    QPixmap welcome("images/welcome.png");
    QSplashScreen splash(welcome);
    splash.resize(welcome.size());
    splash.show();
    app.processEvents();

    Control *ctrl = new Control(800,600,
                                "images/myplane.png",50,5,"images/mybullet.png",1,
                                "images/enemyplane.png",1,"images/enemybullet.png",1);

    /* 显示界面 */
    QGraphicsView *view = new QGraphicsView;
    splash.finish(view);
    view->setWindowTitle(QObject::tr("WAR OF PLANES"));
    view->setScene(ctrl);
    view->setBackgroundBrush(QBrush(QPixmap("images/background.png")));
    view->show();

    return app.exec();
}
