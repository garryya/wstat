QT += widgets

HEADERS       = mainwindow.h \
    wstat.h
SOURCES       = main.cpp \
                mainwindow.cpp \
    wstat.cpp
RESOURCES     = sdi.qrc

# install
target.path = $$[QT_INSTALL_EXAMPLES]/widgets/mainwindows/sdi
INSTALLS += target
