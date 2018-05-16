# -------------------------------------------------
# Project created by QtCreator 2009-12-01T13:18:25
# -------------------------------------------------
TEMPLATE = app
TARGET = Pep9CPU
QT += webenginewidgets
QT += widgets
QT += printsupport
QT += concurrent

# Mac icon/plist
ICON = images/icon.icns
QMAKE_INFO_PLIST = app.plist
QMAKE_MAC_SDK = macosx10.13

#Windows RC file for icon:
RC_FILE = pep9resources.rc

SOURCES += main.cpp \
    mainwindow.cpp \
    byteconverterhex.cpp \
    byteconverterdec.cpp \
    byteconverterchar.cpp \
    byteconverterbin.cpp \
    mainmemory.cpp \
    pep.cpp \
    microcodepane.cpp \
    cpupane.cpp \
    helpdialog.cpp \
    pephighlighter.cpp \
    microcodeeditor.cpp \
    memoryitemdelegate.cpp \
    objectcodepane.cpp \
    code.cpp \
    asm.cpp \
    tristatelabel.cpp \
    specification.cpp \
    byteconverterinstr.cpp \
    aboutpep.cpp \
    cpugraphicsitems.cpp \
    updatechecker.cpp \
    microcodeprogram.cpp \
    rotatedheaderview.cpp \
    disableselectionmodel.cpp \
    colors.cpp \
    cpudatasection.cpp \
    cpucontrolsection.cpp
HEADERS += mainwindow.h \
    byteconverterhex.h \
    byteconverterdec.h \
    byteconverterchar.h \
    byteconverterbin.h \
    mainmemory.h \
    pep.h \
    microcodepane.h \
    cpupane.h \
    helpdialog.h \
    pephighlighter.h \
    enu.h \
    microcodeeditor.h \
    memoryitemdelegate.h \
    objectcodepane.h \
    code.h \
    asm.h \
    tristatelabel.h \
    specification.h \
    byteconverterinstr.h \
    aboutpep.h \
    cpugraphicsitems.h \
    shapes_one_byte_data_bus.h \
    shapes_two_byte_data_bus.h \
    updatechecker.h \
    microcodeprogram.h \
    rotatedheaderview.h \
    disableselectionmodel.h \
    colors.h \
    cpudatasection.h \
    cpucontrolsection.h
FORMS += mainwindow.ui \
    byteconverterhex.ui \
    byteconverterdec.ui \
    byteconverterchar.ui \
    byteconverterbin.ui \
    mainmemory.ui \
    microcodepane.ui \
    cpupane.ui \
    helpdialog.ui \
    objectcodepane.ui \
    byteconverterinstr.ui \
    aboutpep.ui
OTHER_FILES += \
    help/images/registeraddresssignals.png \
    help/figures/exer1204.pepcpu \
    help/figures/exer1206.pepcpu \
    help/usingpep9cpu.html \
    help/pep9reference.html \
    help/pep9os.txt \
    help/microcode.html \
    help/exercises.html \
    help/examples.html \
    help/debugging.html \
    help/cpu.html \
    images/Pep9cpu-icon.png \
    images/Pep9cpu-icon-about.png
RESOURCES += \
    pep9cpuresources.qrc \
    helpresources.qrc \
    dark_style.qrc

DISTFILES += \
    rc/License.md \
    pep9cpudefs.pri \
    Installer/config/control.js \
    Installer/packages/XMLTester/installscript.js \
    Installer/InstallerConfig.pri \
    Installer/config/configlinux.xml \
    Installer/config/configwin32.xml \
    Installer/packages/XMLTester/package.xml \
    Installer/packages/XMLTester/License.txt

include("pep9cpudefs.pri")
include("Installer/InstallerConfig.pri")
