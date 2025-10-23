## HTTP Local Web Server
---

### 📚 Suggested Reading

- **ESP HTTP Server**  
  https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/protocols/esp_http_server.html

- **Official Example**  
  https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/protocols/esp_http_server.html#application-example

- **Embedding Binary Data**  
  https://docs.espressif.com/projects/esp-idf/en/stable/esp32/api-guides/build-system.html#embedding-binary-data

---

### 1. Partition table considerations

Since HTML/CSS/JS files are embedded into the firmware image, they increase flash usage.  
For OTA projects, both `ota_0` and `ota_1` must be large enough to hold the final binary.

For ESP-IDF v5 and above:
- The default `two_ota` table may be too small
- A **custom partition table** is recommended

**Custom partition table setup:**
1. Create a CSV file (e.g., `partition.csv`) in the project root
2. Open menuconfig: menuconfig → Partition Table → Partition Table → Custom partition table CSV
3. Set the path to the CSV file
4. Rebuild project so ota_0/ota_1 sizes take effect

---

### 2. Embedding files into the firmware

To serve files through HTTP, they must be embedded at build time.

**Process overview:**
1. Add `EMBED_FILES` to `main/CMakeLists.txt`
2. List each file that will be served by the HTTP server
3. During the build, ESP-IDF generates linker symbols
4. The server reads from these symbols and sends the file contents in the response

**Important notes:**
- File names are converted into symbols using the pattern  
`_binary_<file_name>_start` / `_binary_<file_name>_end`
- Special characters (`-`, `.`, etc.) are replaced with `_`
- This applies to `.html`, `.css`, `.js`, images, etc.

The practical mapping between embedded files and server response handling is described in more detail in `web-ui.md`.

---

### 3. HTTP Server Initialization

ESP-IDF requires the HTTP server to be initialized after network connectivity is established:

1. Ensure Wi-Fi is connected (Station mode) or SoftAP is running
2. Create server handler (`httpd_handle_t`)
3. Create server configuration (`httpd_config_t`) with desired parameters
4. Start server using `httpd_start()`
5. Register all URI handlers before clients connect.

---

### 4. URI Handlers (core of the HTTP server)

Every HTTP route must have a corresponding **URI handler**. 

**Handler registration process:**
1. Create an `httpd_uri_t` object
2. Define URI path and method (GET/POST)
3. Implement callback function
    - At this point of the project, it consists of:
        1. Set content type (HTML/JS/CSS/etc.) using `httpd_resp_set_type()`
        2. Send embedded binary data in the response using `httpd_resp_send()`
4. Register URI handler with the server handler instance using `httpd_register_uri_handler()`

**Handler responsibilities:**
| Step | Action |
|------|--------|
| 1 | Parse request parameters and headers |
| 2 | Set appropriate content type and HTTP status |
| 3 | Retrieve embedded file data using binary symbols |
| 4 | Send response with correct content length |
| 5 | Handle different HTTP methods appropriately |

Handlers are registered **after** the server is started but **before** clients connect.
<small> :pencil2: *Ideally, register all URI handlers immediately after `httpd_start()`*</small>

---

### 5. Server lifecycle management

**Starting the server:**
1. Configure server (`httpd_config_t`) - set port, stack size, etc.
2. Start server (`httpd_start`)
3. Register all URI handlers
4. Browser accesses page using ESP32's IP

**Stopping the server:**
- Use `httpd_stop()` when network connectivity is lost
- Restart server when connectivity is restored
- Proper cleanup prevents memory leaks and ensures stability

---

### 6. Common patterns and best practices

- **Minimal work in handlers**: Like Wi-Fi events, HTTP handlers should do minimal processing and defer heavy work to tasks
- **Error handling**: Always check return codes from HTTP server functions
- **Memory management**: ESP-HTTP-Server handles most memory internally, but custom allocations must be freed
- **Concurrent connections**: Default configuration supports multiple simultaneous clients

*Note: The HTTP server runs in its own task, so handlers should be thread-safe when accessing shared resources.*