TEMPLATE    = app
QT         += opengl 
CONFIG += c++11

INCLUDEPATH +=  /usr/include/glm  \
                ./Model 

FORMS += MyForm.ui

HEADERS += MyForm.h MyGLWidget.h

SOURCES += main.cpp MyForm.cpp \
           MyGLWidget.cpp  \
           ./Model/model.cpp

DISTFILES += \
    shaders/basicShader.frag \
    shaders/basicShader.vert
