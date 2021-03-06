######################################################################
# Automatically generated by qmake (2.01a) ?? 9? 6 19:52:34 2013
######################################################################

QT += gui declarative
TEMPLATE = app
TARGET = 
DEPENDPATH += . src
INCLUDEPATH += . src

RCC_DIR += .qrc
MOC_DIR += .moc
OBJECTS_DIR += .obj

# Input
HEADERS += src/apev2tag.h \
           src/encoder.h \
           src/id3v2tag.h \
           src/lyrics3v2.h \
           src/lyricsmodel.h \
           src/lyricspathmodel.h \
           src/main.h \
           src/mplayer.h \
           src/pathmodel.h \
           src/playlistlistmodel.h \
           src/playlistmodel.h \
           src/setting.h
SOURCES += src/apev2tag.cpp \
           src/encoder.cpp \
           src/id3v2tag.cpp \
           src/lyrics3v2.cpp \
           src/lyricsmodel.cpp \
           src/lyricspathmodel.cpp \
           src/main.cpp \
           src/mplayer.cpp \
           src/pathmodel.cpp \
           src/playlistlistmodel.cpp \
           src/playlistmodel.cpp \
           src/setting.cpp
RESOURCES += app.qrc
target.path += /opt/musicbox/bin
desktop.path = /usr/share/applications
desktop.files += musicbox.desktop
INSTALLS += target desktop
