## DHT Sensor
---

### 📚 Suggested Reading

- **DHT22 Datasheet**
    https://cdn-shop.adafruit.com/datasheets/Digital+humidity+and+temperature+sensor+AM2302.pdf
    *Note: Some datasheets have misleading informations. I recommend this one.*

- **DHT22 library for ESP32 (ESP-IDF)**
    https://github.com/Andrey-m/DHT22-lib-for-esp-idf

--- 

### 1. Overview
The DHT22 implementation is heavily based on Andrey-m's library. Instead of using it, a custom implementation was created to better understand the sensor communication protocol and timing requirements.

---

### 2. Communication Protocol

**Initialization sequence:**
1. MCU sends start signal (pull bus LOW for 1-10ms)
2. MCU releases bus and waits for sensor response
3. DHT22 responds with 80µs LOW + 80µs HIGH signal
4. Data transmission begins immediately after

**Data frame structure:**
- 40 bits total: 16-bit humidity + 16-bit temperature + 8-bit checksum
- Each bit transmitted as: 
    - 50µs LOW start
    - 26-28µs HIGH = '0'
    - 70µs HIGH = '1'

---

### 3. Implementation Details

**Humidity reading:**
- Two bytes represent humidity × 10
- Example: `0x01 0x2F` → `0x012F` = `303` → `30.3% RH`

**Temperature reading:**
- MSB (bit 15) indicates sign: 0 = positive, 1 = negative
- Remaining 15 bits represent temperature × 10
- Example: `0x80 0x57` → `0x8057` (MSB=1 = negative) → `0x0057 = 87` → `-8.7°C`
  - After reading byte 2 (Most significant Temperature byte), mask it with 0x7F to remove signal

**Checksum validation:**
- last 8 bits of Sum of the first 4 bytes (humidity + temperature) should equal checksum byte
- Essential for data integrity verification

**Timing Critical Sections**

- Use `esp_timer_get_time()` for microsecond-level timing
- use `ets_delay_us()` for microsecond-level delay
- Implement busy-wait loops for signal edge detection

---