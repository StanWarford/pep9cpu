# -------------------------------------------------
# Project created by QtCreator 2009-12-01T13:18:25
# -------------------------------------------------
TEMPLATE = app
TARGET = Pep9CPU

CONFIG -= debug_and_release \
    debug_and_release_target

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

#Generic paths that make future parts of the code easier
QtDir = $$clean_path($$[QT_INSTALL_LIBS]/..)
QtInstallerBin=$$clean_path($$QtDir/../../tools/Qtinstallerframework/3.0/bin)

#All that needs to be done for mac is to run the DMG creator.
#The DMG creator will only be run in Release mode, not debug.
!CONFIG(debug,debug|release):macx{
    dmgMaker.commands += $$QtDir/bin/macdeployqt $$OUT_PWD/Pep9CPU.app -no-plugins -dmg
    first.depends += $(first) dmgMaker
    export(first.depends)
    export(dmgMaker.commands)
    QMAKE_EXTRA_TARGETS += first dmgMaker
}
else:!CONFIG(debug,debug|release):win32{
    repoDir=$$OUT_PWD/Repository/win32
    #Create installer directory structure
    QMAKE_POST_LINK += $${QMAKE_MKDIR} \"$$OUT_PWD/Installer\" & \
        $${QMAKE_MKDIR} \"$$OUT_PWD/Installer/packages\" & \
        $${QMAKE_MKDIR} \"$$OUT_PWD/Installer/packages/pep9cpu\" & \
        $${QMAKE_MKDIR} \"$$OUT_PWD/Installer/packages/pep9cpu/meta\" & \
        $${QMAKE_MKDIR} \"$$OUT_PWD/Installer/packages/pep9cpu/data\" & \
        $${QMAKE_MKDIR} \"$$OUT_PWD/Installer/config\" &
    #Create a directory for update information
    !exists($$repoDir){
        QMAKE_POST_LINK += $${QMAKE_MKDIR} \"$$repoDir\" &
    }
    #Copy over files needed to create installer
    QMAKE_POST_LINK += $${QMAKE_COPY} \"$$shell_path($$PWD\config\configwin32.xml)\" \"$$shell_path($$OUT_PWD/Installer/config/config.xml)\" & \
        $${QMAKE_COPY} \"$$shell_path($$PWD/images/icon.ico)\" \"$$shell_path($$OUT_PWD/Installer/config)\" & \
        $${QMAKE_COPY} \"$$shell_path($$PWD/packages/pep9cpu/package.xml)\" \"$$shell_path($$OUT_PWD/Installer/packages/pep9cpu/meta)\" & \
        $${QMAKE_COPY} \"$$shell_path($$PWD/packages/pep9cpu/License.txt)\" \"$$shell_path($$OUT_PWD/Installer/packages/pep9cpu/meta)\" & \
        $${QMAKE_COPY} \"$$shell_path($$PWD/packages/pep9cpu/installscript.qs)\" \"$$shell_path($$OUT_PWD/Installer/packages/pep9cpu/meta)\" & \
        $${QMAKE_COPY} \"$$shell_path($$PWD/config/control.qs)\" \"$$shell_path($$OUT_PWD/Installer/config)\" &
    #Copy over executable
    QMAKE_POST_LINK +=  $${QMAKE_COPY} \"$$shell_path($$OUT_PWD/Pep9CPU.exe)\" \"$$shell_path($$OUT_PWD/Installer/packages/pep9cpu/data)\" &
    #Execute windeployqt to copy over needed binaries
    #Need to prune extra unneeded libraries, but the first goal is to get a working standalone program
    QMAKE_POST_LINK += \"$$QtDir/bin/windeployqt\" --no-translations --no-system-d3d-compiler \"$$OUT_PWD/Installer/packages/pep9cpu/data/Pep9CPU.exe\" &
    #Execute repository creator
    QMAKE_POST_LINK += \"$$QtInstallerBin/repogen\" --update-new-components -p $$OUT_PWD/Installer/packages $$repoDir &
    #Create installer
    QMAKE_POST_LINK += \"$$QtInstallerBin/binarycreator\" -c \"$$OUT_PWD/Installer/config/config.xml\" -p \"$$OUT_PWD/Installer/packages\" \"Installer/PEP9CPUInstaller\" &
}
linux{
}


