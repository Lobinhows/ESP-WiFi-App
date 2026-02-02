## Button with Interrupt and Semaphore
---

### 📚 Suggested Reading

- **Semaphore API (v4.2)**
    https://docs.espressif.com/projects/esp-idf/en/v4.2/esp32/api-reference/system/freertos.html#semaphore-api

- **GPIO**
    https://docs.espressif.com/projects/esp-idf/en/stable/esp32/api-reference/peripherals/gpio.html

---

### 1. Semaphore Overview

Semaphores are implemented as queues: "Taking" decreases a count, "Giving" increases a count
Trying to take a semaphore while the count is 0 can either poll the semaphore (xTicksToWait = 0) or block indefinitely (xTicksToWait = portMAX_DELAY)
---