QT += widgets

HEADERS     = includes/codeeditor.h
SOURCES     = main.cpp \
              sources/codeeditor.cpp
# install
target.path = $$[QT_INSTALL_EXAMPLES]/widgets/widgets/codeeditor
INSTALLS += target
