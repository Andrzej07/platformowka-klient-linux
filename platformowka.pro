TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

CONFIG(debug, debug|release) {
    DESTDIR = build/debug
} else {
    DESTDIR = build/release

SOURCES += main.cpp \
    App.cpp \
    Camera2D.cpp \
    Errors.cpp \
    GameScreen.cpp \
    GLSLProgram.cpp \
    ImageLoader.cpp \
    InputManager.cpp \
    IOManager.cpp \
    Level.cpp \
    MenuButton.cpp \
    MenuScreen.cpp \
    picoPNG.cpp \
    Player.cpp \
    ResourceManager.cpp \
    Sprite.cpp \
    SpriteBatch.cpp \
    SpriteFont.cpp \
    TextureCache.cpp \
    Timer.cpp \
    Window.cpp \
    MultiScreen.cpp

DISTFILES += \
    platformowka.pro.user \
    Sieci Platformowka.vcxproj \
    Sieci Platformowka.vcxproj.filters \
    Sieci Platformowka.vcxproj.user

HEADERS += \
    App.h \
    Camera2D.h \
    Errors.h \
    GameScreen.h \
    GLSLProgram.h \
    GLTexture.h \
    ImageLoader.h \
    InputManager.h \
    IOManager.h \
    Level.h \
    MenuButton.h \
    MenuScreen.h \
    picoPNG.h \
    Player.h \
    ResourceManager.h \
    Sprite.h \
    SpriteBatch.h \
    SpriteFont.h \
    TextureCache.h \
    Timer.h \
    Vertex.h \
    Window.h \
    MultiScreen.h

unix:!macx: LIBS += -lSDL2main -lSDL2 -lSDL2_ttf
unix:!macx: LIBS += -lGL -lGLU -lGLEW
