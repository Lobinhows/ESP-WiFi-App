# About
This project was developed as part of the [IoT Application Development with ESP32 using the ESP-IDF](https://www.udemy.com/course/iot-application-development-with-the-esp32-using-the-esp-idf/) course on Udemy. The main goal was to practice building Wi-Fi based applications on the ESP32 using the ESP-IDF framework and understand how to structure scalable embedded projects.

During the implementation, I worked on topics such as FreeRTOS task scheduling and synchronization, event-driven design, and secure MQTT communication with AWS IoT Core. The project also includes OTA firmware updates, a local HTTP server with a custom web page, non-volatile storage, SNTP time synchronization, and hardware interaction using an RGB LED and a button with interrupts.

This repository serves both as a portfolio example and as a personal reference of the development flow using the ESP-IDF — from project setup, to cloud integration, to deployment over-the-air.




## Creating a Wi-Fi Application

Creating a Wi-Fi application from scratch in ESP-IDF can be quite complex, and Espressif itself recommends starting from an existing example and iterating on top of it rather than building everything manually from zero.

To better understand the process, it's important to know the stack of components involved:

| Layer         | Responsibility                                             |
|--------------|-------------------------------------------------------------|
| esp_netif     | Network interface abstraction (IP layer, DHCP, etc.)        |
| Wi-Fi driver  | Handles radio/connection logic, events, authentication     |
| Event system  | Delivers status changes to the application                 |
| Application   | Consumes those events and reacts (e.g., start HTTP/OTA)    |

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
###  1. Project setup
Before writing code, some options must be set in `menuconfig`:

- OTA partition scheme
- Wi-Fi buffer sizes depending on mode
- **Wi-Fi NVS Flash** enabled (recommended)

When NVS storage is enabled, ESP-IDF automatically persists Wi-Fi configuration in flash.

---

### 2. Initialization order (very important)

ESP-IDF requires components to be initialized in the **correct order**:

1. `nvs_flash_init()` — required if Wi-Fi uses NVS (almost always)
2. `esp_netif_init()` — initializes TCP/IP abstraction layer
3. `esp_event_loop_create_default()` — enables event delivery
4. `esp_wifi_init()` — initializes Wi-Fi driver

If any of these is skipped, Wi-Fi services may fail silently.
 <small>*Note: steps 2 and 3 can be swapped without issues.*</small>

---

### 3. Event handling

Wi-Fi and IP networking in ESP-IDF are **event-driven**.  
Instead of continuously polling, your application must react to events such as:

| Event                                  | Meaning |
|---------------------------------------|---------|
| `WIFI_EVENT_AP_START`                 | Access Point started |
| `WIFI_EVENT_AP_STACONNECTED`           | A client connected to the SoftAP |
| `IP_EVENT_STA_GOT_IP`                  | IP address received (DHCP successful) |
| `WIFI_EVENT_STA_DISCONNECTED`          | Lost connection / retry needed |

A typical pattern is:
- The handler **does minimal work**
- It forwards the event to a dedicated FreeRTOS task (Wi-Fi manager)
- The manager decides what to start/stop (HTTP, MQTT, OTA, etc.)

---

### 4. Creating the SoftAP (high-level flow)

1. Create the default AP network interface (`esp_netif_create_default_wifi_ap`)
2. Fill a `wifi_config_t` struct (SSID, password, channel, auth mode)
3. Set the Wi-Fi mode (`WIFI_MODE_AP` or `WIFI_MODE_APSTA`)
4. Apply configuration using `esp_wifi_set_config`
5. Start Wi-Fi using `esp_wifi_start`

After this point, the device broadcasts its own Wi-Fi network and external clients can connect to it.