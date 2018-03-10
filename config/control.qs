function func() {

}
function Controller() {
    var widget = gui.pageById(QInstaller.Introduction); // get the introduction wizard page
    if (widget != null)
        widget.packageManagerCoreTypeChanged.connect(onPackageManagerCoreTypeChanged);
    if (installer.isUninstaller()) {
        //installer.setDefaultPageVisible(QInstaller.Introduction, false);
        //installer.setDefaultPageVisible(QInstaller.ComponentSelection, false);
        //installer.setDefaultPageVisible(QInstaller.LicenseCheck, false);
    }
}

Controller.prototype.IntroductionPageCallback  = function(){
    if(installer.isUninstaller()){
    }
    var widget = gui.currentPageWidget();
    if (widget != null) {
        widget.findChild("PackageManagerRadioButton").text = "Add or Remoce Pep9 Applications";
        widget.findChild("UninstallerRadioButton").text = "Uninstall Pep9CPU"
        widget.findChild("UpdaterRadioButton").text = "Update Pep9CPU"
    }

}

Controller.prototype.LicenseAgreementPageCallback = function(){
    var widget = gui.currentPageWidget();
    if (widget != null) {
        widget.AcceptLicenseRadioButton.checked = true;
    }
}

Controller.prototype.FinishedPageCallback = function(){
    var widget = gui.currentPageWidget();
    if (widget != null && !installer.isUninstaller()){
        widget.RunItCheckBox.visible = true;
        widget.RunItCheckBox.checked = true;
        widget.RunItCheckBox.text = "Run Pep9CPU";
    }
}

onPackageManagerCoreTypeChanged = function(){

    var widget = gui.pageById(QInstaller.Introduction);
    if (widget != null) {
        //widget.findChild("PackageManagerRadioButton").visible = false;
    }
}
