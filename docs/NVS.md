## Non-volatile Storage (NVS)
--- 

### 📚 Suggested Reading

- **Non-Volatile Storage Library**
    https://docs.espressif.com/projects/esp-idf/en/v5.5.2/esp32/api-reference/storage/nvs_flash.html

---

### 1. Overview

- Operates on key-value pairs.
    - Keys are ASCII strings
    - Values can have the following types:
        - Integer: (u)int8_t to (u)int64_t
        - Zero-terminated string
        - Variable length binary data (blob) 
- Assigns each key-value pair to a **Namespace**

---
### 2. Implementation

- Saves SSID and Password at flash memory after successfuly connected to an access point.
    - Good for storing small values
- On startup, checks flash for any credentials and attempts a connection, if one is found.
- Clear flash memory if MAX Connections is reached, or if disconnect button is pressed.

