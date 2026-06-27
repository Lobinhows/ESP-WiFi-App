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

NVS operations follow a consistent pattern: open namespace, execute your operation (read, write, or erase), commit if needed, then close.

**Saving Wi-Fi credentials (SSID & Password):**
- Open namespace with `nvs_open("wifi_config", NVS_READWRITE, &handle)`
- Save credentials using `nvs_set_blob()` (or `nvs_set_str()`)
- Commit with `nvs_commit()` and close with `nvs_close()`
- *When:* After successfully connecting to an access point

**Loading stored credentials:**
- Open namespace as `NVS_READONLY`
- Read with `nvs_get_blob()` (call twice: first for size, then for data)
- *When:* On ESP32 startup, before Wi-Fi initialization

**Clearing credentials:**
- Use `nvs_erase_key()` for specific keys or `nvs_erase_all()` for entire namespace
- *When:* Max connection retries reached or user disconnects manually