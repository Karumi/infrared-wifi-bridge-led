#include <Arduino.h>

const String HTML_HEADER = "<!DOCTYPE html><html><head><style>body{background-color:gray;}.btn{padding:10px;color:#FFFFFF;font-size:20px;font-weight:100;border-radius:20px;background-color:#17AD20;text-decoration:none;float:bottom;cursor:pointer;}.btn-off{background-color:#547068;}.btn-normal{background-color:#17ADFF;}.btn:hover{background:#118218;text-decoration:none;}</style><title>KarumiLEDs</title></head>";
const String BODY_START = "<body><div class=\"container-fluid\"align=\"center\"><div class=\"row\"><div class=\"col-md-12\"><img width=\"100px\"height=\"100px\"alt=\"BootstrapImagePreview\"src=\"https://images.squarespace-cdn.com/content/56dd9af5e707eb815a7f9c86/1459333991094-B5E0UWE508B3WKZFNZ8O/karumi+copy.png?content-type=image%2Fpng\"/></br>";

String boardCode(String boardCode) {
    return "<p>"+ boardCode + "</p>";
}

String getLedButton(bool ledState) {
    String buttonClass = "";
    String buttonStatusName = "ON";

    if(!ledState) {
        buttonClass = "btn-off";
        buttonStatusName = "OFF";
    }

    return "</br><form action=\"relay\"method=\"POST\"><input type=\"submit\"value=\""+ buttonStatusName +"\"class=\"btn "+ buttonClass + "\"></input></form></br></br>";
}

const String WIFI_BUTTON = "<button type=\"button\"class=\"btn btn-normal\"onclick=\"showElement('pass-form')\">Configure WIFI</button></br></br>";

String getForm(bool showWifiConfig) {
    String showWifi = getVisibility(showWifiConfig);
    return "<form action=\"/setwifi\"method=\"post\"id=\"pass-form\"style=\"visibility:" + showWifi + "\"><label for=\"ssid\">Firstname:</label><input type=\"text\"name=\"ssid\"required=\"required\"><br><br><label for=\"password\">Lastname:</label><input type=\"text\"name=\"password\"required=\"required\"><br><br><input type=\"submit\"value=\"Submit\"class=\"btn\"></form></br></br>";
}

const String LED_STRIP_COMMANDS = "<button type=\"button\"class=\"btn btn-normal\"onclick=\"showElement('codes-table')\">Add LED Strip command</button>";

String getLedCommands(bool showCommandTable) {
    String showCommands = getVisibility(showCommandTable);
    return "<div id=\"codes-table\"style=\"visibility:" + showCommands + "\"><p>Press the Remote control button you want to configure</p>";
}

const String TABLE_FORM = "<form id=\"codesform\"action=\"/setcodes\"method=\"post\"><table id=\"codestable\"class=\"table table-bordered\"><thead><tr><th>CODES</th><th><input type=\"checkbox\" onclick=\"selectAll(this)\" />Select All<br/></th></tr></thead></table><input id=\"codesize\"type=\"hidden\"name=\"num-codes\"value=\"0\"></br><button type=\"button\"class=\"btn\"onclick=\"onSubmitCodes()\">Submit</button></form></br>";

const String HTML_FOOTER = "</div></div></div></div><script type=\"text/javascript\">var websock;window.onload = function() {connectWS();};function showElement(containerId){element=document.getElementById(containerId);if(element.style.visibility==\"hidden\"){element.style.visibility=\"visible\";}else{element.style.visibility=\"hidden\";}}function addTableRows(newCode){var tableDiv = document.getElementById('codes-table');if(tableDiv.style.visibility != 'hidden') {var table=document.getElementById(\"codestable\");var codeNumber=table.rows.length;var row=table.insertRow(codeNumber);var codeCell=row.insertCell(0);var checkboxCell=row.insertCell(1);codeCell.innerHTML=newCode;checkboxCell.innerHTML=\"<input type='checkbox' name='codecheck' value='\"+newCode+\"'>\";}}function onSubmitCodes(){console.log('sending codes');var codeSizeInput=document.getElementById(\"codesize\");var checkedInputs=document.querySelectorAll('input[type=\"checkbox\"]:checked');codeSizeInput.value=checkedInputs.length;if(checkedInputs.length>0){var array=checkedInputs;for(var i=0;i<codeSizeInput.value;i++){var input=array[i];input.name=\"code\"+i;}}var codesform=document.getElementById(\"codesform\");codesform.submit();}function selectAll(source) {var checkboxes=document.getElementsByName('codecheck');for (var i = 0; i < checkboxes.length; i++){checkboxes[i].checked=source.checked;}}function connectWS(){var url=new URL(\"ws\",window.location);url.protocol=\"ws:\";if(websock){websock.close();}websock=new WebSocket(url.href);websock.onmessage=function(evt){console.log('Data comming in: ' + evt.data);addTableRows(evt.data);};}</script></body></html>";

String getIndex() {
    return HTML_HEADER + 
           BODY_START + 
           boardCode(getBoardCode()) + 
           getLedButton(getTransistorStatus()) + 
           WIFI_BUTTON +
           getForm(false) + 
           LED_STRIP_COMMANDS + 
           getLedCommands(false) + 
           TABLE_FORM + 
           HTML_FOOTER;
}