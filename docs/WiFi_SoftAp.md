## WiFi SoftAP
---

### 📚 Suggested Reading

- **Wi-Fi Driver Guide**  
  https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-guides/wifi.html  
  (Explains Wi-Fi flow, modes, and driver behavior)

- **ESP-NETIF API**  
  https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/network/esp_netif.html  
  Key points:
  - Provides an abstraction layer over the TCP/IP stack  
  - Thread-safe APIs  
  - Used to create default network interfaces (Station/AP)

- **Wi-Fi API Reference**  
  https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/network/esp_wifi.html

---

### 1. Initialization order (very important)

ESP-IDF requires components to be initialized in the **correct order**:

1. `nvs_flash_init()` — required if Wi-Fi uses NVS (almost always)
2. `esp_netif_init()` — initializes TCP/IP abstraction layer
3. `esp_event_loop_create_default()` — enables event delivery
4. `esp_wifi_init()` — initializes Wi-Fi driver

If any of these is skipped, Wi-Fi services may fail silently.
 <small>***Note:** steps 2 and 3 can be swapped without issues.*</small>

---

### 2. Event handling

Wi-Fi and IP networking in ESP-IDF are **event-driven**.  
Instead of continuously polling, your application must react to events such as:

| Event                                  | Meaning |
|---------------------------------------|---------|
| `WIFI_EVENT_AP_START`                 | Access Point started |
| `WIFI_EVENT_AP_STACONNECTED`           | A client connected to the SoftAP |
| `IP_EVENT_STA_GOT_IP`                  | IP address received (DHCP successful) |
| `WIFI_EVENT_STA_DISCONNECTED`          | Lost connection / retry needed |

And many others. A typical pattern is:
- The handler **does minimal work**
- It forwards the event to a dedicated FreeRTOS task (Wi-Fi manager)
- The manager decides what to start/stop (HTTP, MQTT, OTA, etc.)

---

### 3. Creating the SoftAP (high-level flow)

1. Create the default AP network interface (`esp_netif_create_default_wifi_ap`)
2. Fill a `wifi_config_t` struct (SSID, password, channel, auth mode)
3. Set the Wi-Fi mode (`WIFI_MODE_AP` or `WIFI_MODE_APSTA`)
4. Apply configuration using `esp_wifi_set_config`
5. Start Wi-Fi using `esp_wifi_start`

After this point, the device broadcasts its own Wi-Fi network and external clients can connect to it.

### 4. Best Practices and Common Issues

**Configuration Tips:**
- Use `WIFI_AUTH_WPA2_PSK` for basic security
- Choose non-overlapping Wi-Fi channels

**Common Problems:**
- **Initialization failures**: Check NVS initialization and event loop
- **DHCP issues**: Ensure proper netif configuration
- **Connection drops**: Verify signal strength and power settings

**Debugging:**
- Enable Wi-Fi logs with `esp_log_level_set("wifi", ESP_LOG_DEBUG)`
- Monitor events to track connection state changes
- Check return codes from all Wi-Fi functions

*Note: SoftAP mode consumes more power than Station mode - consider this for battery-powered devices.*