TARGET = wstat_qt
QT += widgets


HEADERS       = mainwindow.h \
    wstat.h
SOURCES       = main.cpp \
                mainwindow.cpp \
    wstat.cpp
RESOURCES     = sdi.qrc

# install
target.path = .
INSTALLS += target
