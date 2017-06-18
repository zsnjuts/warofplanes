QT += core gui
QT += multimedia #“Ù¿÷≤•∑≈
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

HEADERS += \
    Bullet.h \
    Control.h \
    EnemyPlane.h \
    MyPlane.h \
    Plane.h

SOURCES += \
    Bullet.cpp \
    Control.cpp \
    EnemyPlane.cpp \
    main.cpp \
    MyPlane.cpp \
    Plane.cpp

CONFIG += C++11
