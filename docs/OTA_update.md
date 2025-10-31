## OTA Firmware Update
---

### 📚 Suggested Reading

- **OTA Updates and API reference**
    https://docs.espressif.com/projects/esp-idf/en/stable/esp32/api-reference/system/ota.html

- **Partition Tables** 
    https://docs.espressif.com/projects/esp-idf/en/stable/esp32/api-guides/partition-tables.html

---

### 1. Overview
- Allows a device to update itself from data received (over WiFi, for example) while a normal firmware is running.
- Requires configurating the partition table with at least 2 OTA slots (ota_0 and ota_1) and an "OTA Data Partition"(see [HTTP Server](HTTP_Server.md))
- The OTA operation functions writes the new firmware to whichever partition is not selected for boot.
- OTA Data partition determines which image should be used in the next boot

--- 

## 2. Partition Table Requirements

**Minimum partition setup:**
- Two OTA app partitions (`ota_0`, `ota_1`)
- One OTA data partition (`otadata`)
- Factory partition (optional, for fallback)

**Key considerations:**
- Each OTA partition must be large enough for your firmware
- OTA data partition (typically 0x2000 bytes) stores boot selection
- Custom partition tables are often necessary (see [HTTP Server](HTTP_Server.md))

---

## 3. OTA Update Process

**High-level flow with implementation details:**

1. **Receive firmware file** via network connection
   - For HTTP OTA: Use `httpd_req_recv()` to receive data chunks
   - **Critical**: First chunk contains HTTP headers and multipart boundaries
        - Remove header data before passing to OTA functions
   - Parse multipart/form-data to find the actual `.bin` file start

2. **Initialize OTA process** with `esp_ota_begin()`
   - Selects the inactive OTA partition automatically
   - Validates partition space and prepares for writing
   - Must be called **after** identifying the actual firmware data start

3. **Write firmware data** using `esp_ota_write()`
   - Process received chunks sequentially
   - Monitor write operations for errors

4. **Finalize and validate** with `esp_ota_end()`
   - Verifies firmware integrity and completeness
   - Returns error if validation fails (corrupted image, incomplete write)

5. **Set boot partition** with `esp_ota_set_boot_partition()`
   - Updates OTA data partition to point to the new firmware
   - Ensures the new image will boot on next restart

6. **Restart ESP32** to launch the updated firmware
   - New firmware runs immediately after reboot
   - Automatic rollback occurs if new firmware fails to boot


### :warning: First chunk management
- First data chunk contains: `Content-Type: multipart/form-data` boundaries
- Look for pattern: `\r\n\r\n` to find the start of binary data
- Boundary strings must be skipped before calling `esp_ota_write()`

**Example data structure:**
```
------WebKitFormBoundaryXYZ123
Content-Disposition: form-data; name="firmware"; filename="firmware.bin"
Content-Type: application/octet-stream

<ACTUAL BINARY DATA STARTS HERE>
```


