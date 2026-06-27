## AWS IoT Core Integration
---

### 📚 Suggested Reading

- **ESP-AWS-IoT Repository**  
  https://github.com/espressif/esp-aws-iot

- **AWS IoT Core Documentation**  
  https://docs.aws.amazon.com/iot/latest/developerguide/what-is-aws-iot.html

- **AWS IoT Device SDK for Embedded C Tutorial**  
  https://docs.aws.amazon.com/iot/latest/developerguide/iot-embedded-c-sdk.html

---

### 1. Overview

AWS IoT Core integration allows the ESP32 to securely communicate with AWS cloud services using MQTT over TLS. The implementation in this project is based on the `mqtt_demo_mutual_auth` example from the official espressif/esp-aws-iot repository .

**Key characteristics:**
- Mutual TLS authentication (client certificate + private key)
- MQTT over TLS (port 8883)
- Publish/Subscribe messaging model
- Secure cloud communication

---

### 2. AWS IoT Core Setup (High-Level Flow)

To connect an ESP32 to AWS IoT Core, you need to configure three main components in AWS :

1. **Create a Thing**  
   - Virtual representation of your device in AWS IoT registry 
   - Navigate: AWS IoT Console → Manage → Things → Create a single thing

2. **Create and Attach a Policy**  
   - Defines what operations (connect, publish, subscribe) your device can perform 
   - For testing, use wildcard (`"*"`) for actions and resources; restrict for production 

3. **Generate Certificates**  
   - Auto-generate X.509 device certificate, public key, and private key 
   - **Important**: Download all files immediately (only available once) 
   - Required files:
     - Device certificate (`device.pem.crt`)
     - Private key (`private.pem.key`)
     - Root CA certificate (`AmazonRootCA1.pem`)

4. **Obtain Endpoint**  
   - Format: `xxxxxxxxxx-ats.iot.<region>.amazonaws.com` 
   - Find in: AWS IoT Console → Settings → Device data endpoint

---

### 3. Implementation (ESP32 Side)

The integration uses the `mqtt_demo_mutual_auth` example from the esp-aws-iot repository. You need to copy `mqtt_demo_mutual_auth.c`, `demo_config.h` and `Kconfig.projbuild` files from esp-aws-iot to your project folder. 

**Setup steps:**

1. **Clone the repository:**
   ```bash
   git clone https://github.com/espressif/esp-aws-iot.git
   ```

2. **Place certificates:**
   ```
   esp-aws-iot/examples/mqtt/tls_mutual_auth/main/certs/
   ├── client.crt          # (device.pem.crt)
   ├── privateKey.pem      # (private.pem.key)
   └── root_cert_auth.crt  # (AmazonRootCA1.pem)
   ```

3. **Configure via menuconfig:**
   ```bash
   idf.py menuconfig
   ```
   Set:
   - Wi-Fi SSID and Password
   - MQTT broker endpoint (your AWS endpoint)
   - MQTT client identifier 

4. **Build and flash:**
   ```bash
   idf.py build flash monitor
   ```
---

### 4. Tips

**Certificates:**
- Never commit certificates or private keys to version control
- Keep certificates organized: `client.crt`, `privateKey.pem`, `root_cert_auth.crt`

**Configuration:**
- Double-check your AWS endpoint format: `xxxxxxxxxx-ats.iot.<region>.amazonaws.com`
- Use the same client ID in code as the Thing name in AWS Console
- Verify Wi-Fi credentials before testing AWS connection

**Debugging:**
- Monitor serial output for connection errors
- Use AWS IoT Console → Test → MQTT test client to verify published messages
- Enable `CONFIG_AWS_IOT_LOG_LEVEL_DEBUG` in menuconfig for detailed logs

