TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

LIBS += -fopenmp

SOURCES += \
    src/BTtree.cpp \
    src/main.cpp \
    src/puzzle.cpp

HEADERS += \
    include/BTtree.h \
    include/puzzle.h \

DISTFILES +=
