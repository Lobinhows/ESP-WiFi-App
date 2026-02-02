## WiFi Connection (WiFi Station Mode)
--- 
### 📚 Suggested Reading

- **Wi-Fi Driver Guide**  
  https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-guides/wifi.html

- **ESP-NETIF API**  
  https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/network/esp_netif.html

- **Wi-Fi API Reference**  
  https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/network/esp_wifi.html

---
### 1. Overview

Station (STA) mode allows the ESP32 to connect to an existing Wi-Fi network as a client. This is the most common mode for IoT devices that need internet access or need to communicate with other devices on a local network.

**Key characteristics:**
- Connects to existing Wi-Fi access points (vs. creating its own like SoftAP)
- Obtains IP address via DHCP
- Supports WPA/WPA2/WPA3 security
- Enables internet connectivity for the device

---

### 2. Initialization and Event Handling

**Initialization order** is identical to SoftAP mode. Refer to [WiFi SoftAP](WiFi_SoftAp.md) for the required sequence.

**Event handling** follows the same event-driven pattern as SoftAP. Key Station-specific events:

| Event | Meaning | Typical Action |
|-------|---------|----------------|
| `WIFI_EVENT_STA_START` | Station mode started | Begin connection attempt |
| `WIFI_EVENT_STA_CONNECTED` | Connected to AP | Wait for IP address |
| `WIFI_EVENT_STA_DISCONNECTED` | Lost connection | Attempt reconnection |
| `IP_EVENT_STA_GOT_IP` | Got IP address | Start network services |
| `IP_EVENT_STA_LOST_IP` | Lost IP address | Reinitialize network |

---

### 3. Connecting to a Network (high-level flow)

1. Create the default Station network interface (`esp_netif_create_default_wifi_sta`)
2. Fill a `wifi_config_t` struct with SSID, password, and authentication mode
3. Set the Wi-Fi mode (`WIFI_MODE_STA` or `WIFI_MODE_APSTA`)
4. Apply configuration using `esp_wifi_set_config()`
5. Start Wi-Fi using `esp_wifi_start()`
6. Initiate connection using `esp_wifi_connect()`

---

### 4. Power Management

**Wi-Fi Power Save Modes**:

| Mode | Best For | Impact |
|------|----------|--------|
| `WIFI_PS_NONE` | High performance, AC power | No latency, high power |
| `WIFI_PS_MIN_MODEM` | Balanced, battery-powered | Moderate latency/power |
| `WIFI_PS_MAX_MODEM` | Maximum battery life | Higher latency, lowest power |

**Important consideration:** Power save modes increase latency. Choose based on:
- Battery vs. mains power
- Required response time
- Data transfer frequency
