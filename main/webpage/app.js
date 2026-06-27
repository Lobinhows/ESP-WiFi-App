/**
 * Add globals here
 */
var seconds = null;
var otaTimerVar = null;
var wifiConnectInterval = null;
var isConnected = false;

/**
 * Initialize functions here.
 */
$(document).ready(function () {
    getUpdateStatus();
    startSensorInterval();
    startLocalTimeInterval();
    if (isConnected == false) {
        checkConnectStatus()
    }

    $("#connect_button").on("click", function () {
        checkCredentials();
    });
    $("#disconnect_wifi").on("click", function () {
        disconnectWifi();
    });
});

/**
 * Displays file name and size.
 */
function displayFileInfo() {
    var firm = document.getElementById("selected_file");
    var file = firm.files[0];

    const button = document.getElementById("select_button");
    button.value = "File: " + file.name + "\n Size: " + file.size + " bytes";
}

/**
 * Handles the firmware update.
 */
function updateFirmware() {
    // Form Data
    var formData = new FormData();
    var fileSelect = document.getElementById("selected_file");

    if (fileSelect.files && fileSelect.files.length == 1) {
        var file = fileSelect.files[0];
        formData.set("file", file, file.name);
        document.getElementById("ota_update_status").innerHTML = "Uploading " + file.name + ",\n\rFirmware Update in Progress...";

        // Http Request
        var request = new XMLHttpRequest();

        request.upload.addEventListener("progress", updateProgress);
        request.open('POST', "/OTAupdate");
        request.responseType = "blob";
        request.send(formData);
    }
    else {
        window.alert('Please select a file');
    }
}

/**
 * Progress on transfers from the server to the client (downloads).
 */
function updateProgress(oEvent) {
    if (oEvent.lengthComputable) {
        getUpdateStatus();
    }
    else {
        window.alert('total size is unknown');
    }
}

/**
 * Posts the firmware udpate status.
 */
function getUpdateStatus() {
    var xhr = new XMLHttpRequest();
    var requestURL = "/OTAstatus";
    xhr.open('POST', requestURL, false);
    xhr.send('ota_update_status');

    if (xhr.readyState == 4 && xhr.status == 200) {
        var response = JSON.parse(xhr.responseText);

        document.getElementById("latest_firmware").innerHTML = response.compile_date + " - " + response.compile_time

        // If flashing was complete it will return a 1, else -1
        // A return of 0 is just for information on the Latest Firmware request
        if (response.ota_update_status == 1) {
            // Set the countdown timer time
            seconds = 10;
            // Start the countdown timer
            otaRebootTimer();
        }
        else if (response.ota_update_status == -1) {
            document.getElementById("ota_update_status").innerHTML = "!!! Upload Error !!!";
        }
    }
}

/**
 * Displays the reboot countdown.
 */
function otaRebootTimer() {
    document.getElementById("ota_update_status").innerHTML = "OTA Firmware Update Complete. This page will close shortly, Rebooting in: " + seconds;

    if (--seconds == 0) {
        clearTimeout(otaTimerVar);
        window.location.reload();
    }
    else {
        otaTimerVar = setTimeout(otaRebootTimer, 1000);
    }
}

/**
 * Get DHT temperature and humidity values for webpage display
 */
function getSensorValues() {
    $.getJSON('/dhtSensor.json', function (data) {
        $("#temperature_reading").text('Temperature: ' + data['temp']);
        $("#humidity_reading").text('Humidity: ' + data['humidity']);
    });
}

/**
 * Set interval for getting sensor updates
 */
function startSensorInterval() {
    setInterval(getSensorValues, 5000);
}

/**
 * Clear connection credentials
 */
function stopWiFiConnectStatusInterval() {
    if (wifiConnectInterval != null) {
        clearInterval(wifiConnectInterval);
        wifiConnectInterval = null;
    }
}


/**
 * Get connection status
 */
function getWiFiConnectStatus() {
    var xhr = new XMLHttpRequest();
    var requestURL = "/wifiConnectStatus";
    xhr.open('POST', requestURL, false);
    xhr.send('wifi_connect_status');

    if (xhr.readyState == 4 && xhr.status == 200) {
        var response = JSON.parse(xhr.responseText);

        document.getElementById("wifi_connect_status").innerHTML = "Connecting...";

        if (response.wifi_connect_status == 2) {
            document.getElementById("wifi_connect_status").innerHTML = "<h4 class='rd'>Failed to connect. Check Ap credentials and compatibility</h4>";
            stopWiFiConnectStatusInterval();
        } else if (response.wifi_connect_status == 3) {
            document.getElementById("wifi_connect_status").innerHTML = "<h4 class='gr'>Connected succesfully!</h4>";
            document.getElementById('connect_info').style.display = 'flex';
            document.getElementById('WiFiConnect').style.display = 'none';
            stopWiFiConnectStatusInterval();
            getConnectInfo();
            isConnected = true;
        }

    }
}

function checkConnectStatus() {
    var xhr = new XMLHttpRequest();
    var requestURL = "/wifiConnectStatus";
    xhr.open('POST', requestURL, false);
    xhr.send('wifi_connect_status');

    if (xhr.readyState == 4 && xhr.status == 200) {
        var response = JSON.parse(xhr.responseText);
        console.log(response.wifi_connect_status);
        if (response.wifi_connect_status == 2) { // Connection fail
            isConnected = false;
        } else if (response.wifi_connect_status == 3) {
            document.getElementById('connect_info').style.display = 'flex';
            document.getElementById('WiFiConnect').style.display = 'none';
            getConnectInfo();
            isConnected = true;
        }

    }
}




/**
 * Start interval for checking connection status.
 */
function startWiFiConnectStatusInterval() {
    wifiConnectInterval = setInterval(getWiFiConnectStatus, 2800);
}


/**
 * Connect WiFi function called using the SSID and password entered into text fields.
 */
function connectWiFi() {
    selectedSSID = $("#connect_ssid").val();
    password = $("#connect_pass").val();

    $.ajax({
        url: '/wifiConnect.json',
        dataType: 'json',
        method: 'POST',
        cache: false,
        headers: { 'my-connect-ssid': selectedSSID, 'my-connect-pass': password },
        data: { 'timestamp': Date.now() }
    });

    startWiFiConnectStatusInterval();
}

/**
 * Check credentials on connect_button onclick.
 */
function checkCredentials() {
    errorList = "";
    credsOk = true;

    selectedSSID = $("#connect_ssid").val();
    password = $("#connect_pass").val();

    if (selectedSSID == "") {
        errorList += "<h4 class='rd'>SSID cannot be empty</h4>";
        credsOk = false;
    }
    if (password == "") {
        errorList += "<h4 class='rd'>Password cannot be empty</h4>";
        credsOk = false;
    }

    if (credsOk == false) {
        $("#wifi_cred_errors").html(errorList);
    } else {
        $("#wifi_cred_errors").html("");
        connectWiFi();
    }


}

/**
 * Show password if box is checked
 */
function showPassword() {
    var x = document.getElementById("connect_pass");
    if (x.type === "password") {
        x.type = "text";
    } else {
        x.type = "password";
    }
}

/**
 * Gets connection information for display in web page
 */
function getConnectInfo() {
    $.getJSON('/wifiConnectInfo.json', function (data) {
        $("#connected_ap_label").html("Connected to: ");
        $("#connected_ap").text(data["ap"]);

        $("#ip_address_label").html("IP Address: ");
        $("#wifi_connected_ip").text(data["ip"]);

        $("#netmask_label").html("Netmask: ");
        $("#wifi_connected_netmask").text(data["netmask"]);

        $("#gateway_label").html("Gateway: ");
        $("#wifi_connected_gw").text(data["gateway"]);

        // Kinda doesn't make sense to be here, but also doesn't make sense to be anywhere if you can only see after connected to it...
        $("#ssid_label").html("Access point SSID: ");
        $("#ap_ssid").text(data["ap_ssid"]);

        document.getElementById('disconnect_wifi').style.display = 'block';
    });
}

/**
 * Disconnect WiFi when disconnect button is pressed
 */
function disconnectWifi() {
    $.ajax({
        url: '/wifiDisconnect.json',
        dataType: 'json',
        method: 'DELETE',
        cache: false,
        data: { 'timestamp': Date.now() }
    });
    isConnected = false;
    // Update web page
    document.getElementById("wifi_connect_status").innerHTML = "";
    document.getElementById("connect_info").style.display = 'none';
    document.getElementById("WiFiConnect").style.display = 'flex';
    setTimeout("location.reload(true);", 2000);
}

/**
 * Sets interval for displaying local time
 */
function startLocalTimeInterval() {
    setInterval(getLocalTime, 10000);
}

/**
 * Gets Local time
 * @note Connect ESP32 to the internet
 */
function getLocalTime() {
    $.getJSON('/localTime.json', function (data) {
        $("#local_time").text(data["time"]);
    });
}