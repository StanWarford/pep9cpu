function Component()
{
    gui.pageWidgetByObjectName("LicenseAgreementPage").entered.connect(changeLicenseLabels);
    if (installer.value("os") === "win") {
        component.addOperation("CreateShortcut", "@TargetDir@/MyApp.exe", "@StartMenuDir@/MyApp.lnk");
    }
    //gui.pageWidgetByObjectName("Introduction").entered.connect(changeLicenseLabels);
}

changeLicenseLabels = function()
{
    //page = gui.pageWidgetByObjectName("LicenseAgreementPage");
    //page.AcceptLicenseLabel.setText("Yes I do!");
    //page.RejectLicenseLabel.setText("No I don't!");
}
