QT += core gui widgets
CONFIG += c++20
TEMPLATE = app
TARGET = animal_shelter_care_board

INCLUDEPATH += include

SOURCES += \
 src/care_board.cpp \
 src/main.cpp

HEADERS += \
 include/care_board.h