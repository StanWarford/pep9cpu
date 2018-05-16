#Installer Configuration
#Internal version number, used to version installer by name
VERSION_NUMBER = "92"
#Name of the installation tool
OUTPUT_INSTALLER_NAME=$$TARGET$$VERSION_NUMBER

#Data Configuration
#Look for all files below starting in this directory
PATH_PREFIX = $$PWD
#Each of the following variables should be a space separated list
UNIVERSAL_ICONS = ""
UNIVERSAL_DATA = "LICENSE"
MAC_ICONS = "" #No special icons for Mac
MAC_DATA = "" #No additional data for Mac
WINDOWS_ICONS = "images/icon.ico" "image/Pep9cpu-icon.png"
WINDOWS_DATA = "" #No additional data for Windows
LINUX_ICONS = "" #No implementation for Linux
LINUX_DATA = "" #No implementation for Linux
