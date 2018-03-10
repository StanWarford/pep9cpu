function Component(){
    gui.pageWidgetByObjectName("LicenseAgreementPage").entered.connect(changeLicenseLabels);
    installer.installationFinished.connect(this, Component.prototype.installationFinishedPageIsShown);
    installer.finishButtonClicked.connect(this, Component.prototype.installationFinished);

    //gui.pageWidgetByObjectName("Introduction").entered.connect(changeLicenseLabels);
}

Component.prototype.createOperations = function(){
    component.createOperations();
    if (installer.value("os") == "win") {
                try {
                    var userProfile = installer.environmentVariable("USERPROFILE");
                    installer.setValue("UserProfile", userProfile);
                    component.addOperation("CreateShortcut", "@TargetDir@\\Pep9CPU.exe", "@UserProfile@\\Desktop\\Pep9CPU.lnk");
                } catch (e) {
                    // Do nothing if key doesn't exist
                }
    }
}
Component.prototype.installationFinishedPageIsShown = function(){
console.log("in function")
    try {
        if (installer.isInstaller() && installer.status == QInstaller.Success) {
        console.log("in if")
            installer.addWizardPageItem( component, "QCheckBox", installer.InstallationFinished );
        }
    } catch(e) {
        console.log(e);
    }
}

Component.prototype.installationFinished = function(){
    try {
        if (installer.isInstaller() && installer.status == QInstaller.Success) {
            var isReadMeCheckBoxChecked = component.userInterface( "InstallationFinished" ).QCheckBox.checked;
            if (isReadMeCheckBoxChecked) {
                QDesktopServices.openUrl("file:///" + installer.value("TargetDir") + "/README.txt");
            }
        }
    } catch(e) {
        console.log(e);
    }
}
changeLicenseLabels = function()
{
    //page = gui.pageWidgetByObjectName("LicenseAgreementPage");
    //page.AcceptLicenseLabel.setText("Yes I do!");
    //page.RejectLicenseLabel.setText("No I don't!");
}
