function Component(){
    gui.pageWidgetByObjectName("LicenseAgreementPage").entered.connect(changeLicenseLabels);
    installer.installationFinished.connect(this, Component.prototype.installationFinishedPageIsShown);
    installer.finishButtonClicked.connect(this, Component.prototype.installationFinished);

    //gui.pageWidgetByObjectName("Introduction").entered.connect(changeLicenseLabels);
}

Component.prototype.createOperations = function(){
    component.createOperations();
    if(installer.value("os") == "win"){
        var registryVC2010x86 = installer.execute("reg", new Array("QUERY", "HKLM\\SOFTWARE\\WOW6432Node\\Microsoft\\VisualStudio\\14.0\\VC\\Runtimes\\x64", "/v", "Installed"))[0];
        if(registryVC2010x86==false){
            component.addOperation("Execute", "@TargetDir@\\vcredist_x64.exe", "/norestart");
        }
        component.addOperation("CreateShortcut", "@TargetDir@/Pep9CPU.exe", "@StartMenuDir@/Pep9CPU.lnk",
                    "workingDirectory=@TargetDir@","description=Run Pep9CPU");
    }
}
Component.prototype.installationFinishedPageIsShown = function(){
    /*
    Attempt at adding another checkbox to add a desktop shortcut
    try {
        if (installer.isInstaller() && installer.status == QInstaller.Success) {
            installer.addWizardPageItem( component, "QCheckBox", installer.InstallationFinished );
        }
    } catch(e) {
        console.log(e);
    }*/
}

Component.prototype.installationFinished = function(){

}
changeLicenseLabels = function()
{
    //page = gui.pageWidgetByObjectName("LicenseAgreementPage");
    //page.AcceptLicenseLabel.setText("Yes I do!");
    //page.RejectLicenseLabel.setText("No I don't!");
}
