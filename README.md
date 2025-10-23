# About
This project was developed as part of the [IoT Application Development with ESP32 using the ESP-IDF](https://www.udemy.com/course/iot-application-development-with-the-esp32-using-the-esp-idf/) course on Udemy. The main goal was to practice building Wi-Fi based applications on the ESP32 using the ESP-IDF framework and understand how to structure scalable embedded projects.

During the implementation, I worked on topics such as FreeRTOS task scheduling and synchronization, event-driven design, and secure MQTT communication with AWS IoT Core. The project also includes OTA firmware updates, a local HTTP server with a custom web page, non-volatile storage, SNTP time synchronization, and hardware interaction using an RGB LED and a button with interrupts.

This repository serves both as a portfolio example and as a personal reference of the development flow using the ESP-IDF — from project setup, to cloud integration, to deployment over-the-air.

#### :pushpin: To-Do list:
- [ ] The web page was provided by the course. I will not upload it until I create one myself.
- [ ] Reorganize source and header files

---

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

###  Project setup
Before writing code, some options must be set in `menuconfig`:

- OTA partition scheme
- Wi-Fi buffer sizes depending on mode
- **Wi-Fi NVS Flash** enabled (recommended)

When NVS storage is enabled, ESP-IDF automatically persists Wi-Fi configuration in flash.

---

:heavy_exclamation_mark: To keep the documentation organized, each major step is described in a separate file inside the `docs/` folder, following the same order in which the system was built.

- [Starting SoftAP](docs/WiFi_SoftAP.md)
- [HTTP Server](docs/HTTP_Server.md)
- [OTA Updates](docs/OTA_updates.md)
