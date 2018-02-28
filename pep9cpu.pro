# -------------------------------------------------
# Project created by QtCreator 2009-12-01T13:18:25
# -------------------------------------------------
TEMPLATE = app
TARGET = Pep9CPU

QT += webenginewidgets
QT += widgets
QT += printsupport

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
    sim.cpp \
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
    objectcodelabel.cpp \
    specification.cpp \
    byteconverterinstr.cpp \
    aboutpep.cpp \
    cpugraphicsitems.cpp
HEADERS += mainwindow.h \
    byteconverterhex.h \
    byteconverterdec.h \
    byteconverterchar.h \
    byteconverterbin.h \
    mainmemory.h \
    sim.h \
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
    objectcodelabel.h \
    specification.h \
    byteconverterinstr.h \
    aboutpep.h \
    cpugraphicsitems.h \
    shapes_one_byte_data_bus.h \
    shapes_two_byte_data_bus.h
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
OTHER_FILES += help/images/registeraddresssignals.png \
    help/figures/exer1204.pepcpu \
    help/figures/exer1206.pepcpu \
    images/Pep9cpu-icon.png \
    images/Pep9cpu-icon-about.png \
    help/usingpep9cpu.html \
    help/pep9reference.html \
    help/pep9os.txt \
    help/microcode.html \
    help/exercises.html \
    help/examples.html \
    help/debugging.html \
    help/cpu.html
RESOURCES += pep9cpuresources.qrc \
    helpresources.qrc

DISTFILES += \
    package/package.xml \
    packages/package.xml \
    packages/pep9cpu/meta/package.xml \
    packages/pep9cpu/package.xml \
    packages/pep9cpu/License.txt \
    packages/pep9cpu/installscript.qs \
    packages/pep9cpu/control.qs \
    config/control.qs \
    config/configmac.xml \
    config/configwin32.xml
#0.     Clean install directory if it exists
#1.     Create install directory tree if it doesn't exist
#2.     Copy static files to install directory
#3.     Compile the program
#4.     Symlink in application
#5.     Run package creator

#Step 0
message(Remove |true debug statement from following line)
!debug|true{#Remove |true

copy.commands += $${QMAKE_DEL_TREE} $$clean_path($$OUT_PWD/Installer);

#Step 1
copy.commands += $${QMAKE_MKDIR} $$OUT_PWD/Installer; \
    $${QMAKE_MKDIR} $$OUT_PWD/Repository; \
    $${QMAKE_MKDIR} $$OUT_PWD/Installer/packages; \
    $${QMAKE_MKDIR} $$OUT_PWD/Installer/packages/pep9cpu; \
    $${QMAKE_MKDIR} $$OUT_PWD/Installer/packages/pep9cpu/meta; \
    $${QMAKE_MKDIR} $$OUT_PWD/Installer/packages/pep9cpu/data; \
    $${QMAKE_MKDIR} $$OUT_PWD/Installer/config;

#Export the operating system specific config file
repoDir
macx{
    repoDir = $$OUT_PWD/Repository/mac
    copy.commands += $${QMAKE_MKDIR} $$repoDir;
    copy.commands += $${QMAKE_COPY_DIR} $$PWD/config/configmac.xml $$OUT_PWD/Installer/config/config.xml;
}
else:win32{
    repoDir=$$OUT_PWD/Repository/win32
    copy.commands += $${QMAKE_MKDIR} $$repoDir;
    copy.commands += $${QMAKE_COPY_DIR} $$PWD/config/configwin32.xml $$OUT_PWD/Installer/config/config.xml;
}
else:linux{
    error(Linux build tools not yet available)
    repoDir = $$OUT_PWD/Repository/linux
    copy.commands += $${QMAKE_MKDIR} $$repoDir;
    copy.commands += $${QMAKE_COPY_DIR} $$PWD/config/configlinux.xml $$OUT_PWD/Installer/config/config.xml;
}
else{
    message(No binary repository generation is available for this platform)
}
#Step 2
copy.commands +=   $${QMAKE_COPY_DIR} $$PWD/images/icon.icns $$OUT_PWD/Installer/config; \
    $${QMAKE_COPY_DIR} $$PWD/images/icon.ico $$OUT_PWD/Installer/config; \
    $${QMAKE_COPY_DIR} $$PWD/images/Pep9cpu-icon.png $$OUT_PWD/Installer/config; \
    $${QMAKE_COPY_DIR} $$PWD/packages/pep9cpu/package.xml $$OUT_PWD/Installer/packages/pep9cpu/meta; \
    $${QMAKE_COPY_DIR} $$PWD/packages/pep9cpu/License.txt $$OUT_PWD/Installer/packages/pep9cpu/meta; \
    $${QMAKE_COPY_DIR} $$PWD/packages/pep9cpu/installscript.qs $$OUT_PWD/Installer/packages/pep9cpu/meta; \
    $${QMAKE_COPY_DIR} $$PWD/config/control.qs $$OUT_PWD/Installer/config;

first.depends += $(first) copy
export(copy.commands)
QMAKE_EXTRA_TARGETS += first copy

#Step 3
#Let the program compile

#Step 4
copydata.commands = $(COPY_DIR) $$OUT_PWD/Pep9CPU.app $$OUT_PWD/Installer/packages/pep9cpu/data
first.depends += $(first) copydata
export(first.depends)
export(copydata.commands)
QMAKE_EXTRA_TARGETS += first copydata

#Step 5
QtInstallerBin=$$clean_path($$[QT_INSTALL_LIBS]/../../../tools/Qtinstallerframework/3.0/bin)
exists($$QtInstallerBin/binarycreator):exists($$QtInstallerBin/repogen){
    example.commands = $$QtInstallerBin/binarycreator -c $$OUT_PWD/Installer/config/config.xml -p $$OUT_PWD/Installer/packages Installer/PEP9CPUInstaller;
    example.commands += $$QtInstallerBin/repogen --update-new-components -p $$OUT_PWD/Installer/packages $$repoDir;
    first.depends += $(first) example
    export(example.commands)
    QMAKE_EXTRA_TARGETS += example
}
else{
    warning("QT binary creator isn't installed, aborting installer creation")
}
}
##TODO:
# Only create Installer directory if in release

