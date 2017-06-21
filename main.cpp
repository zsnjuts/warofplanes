#include "Control.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    /* 启动画面 */
    QPixmap welcome(":/images/welcome_new.png");
    QSplashScreen splash(welcome);
    splash.resize(welcome.size());
    splash.show();
    app.processEvents();
    Sleep(1000);

    Control *ctrl = new Control;

    /* 显示界面 */
    QGraphicsView *view = new QGraphicsView;
    splash.finish(view);
    view->setWindowTitle(QObject::tr("WAR OF PLANES"));
    view->setScene(ctrl);
    view->setBackgroundBrush(QBrush(QPixmap(":/images/background.png")));
    view->show();

    return app.exec();
}
