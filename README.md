# BLE BME280 Sensor Node (nRF5340 & Zephyr)

Dieses Projekt implementiert einen Bluetooth Low Energy (BLE) Sensor-Knoten auf Basis des **nRF5340**. Es nutzt das **Zephyr RTOS**, um Umgebungsdaten eines **BME280** Sensors über I2C auszulesen und via standardisierter GATT-Services bereitzustellen.

## 🛠 Tech Stack
- **Hardware:** Nordic nRF5340 (DK oder Custom Board)
- **RTOS:** Zephyr RTOS
- **SDK:** nRF Connect SDK (NCS)
- **IDE:** Visual Studio Code mit nRF Connect Extension

## 📊 GATT Service Spezifikation
Der Knoten verwendet den standardisierten **Environmental Sensing Service (ESS)**:

| Feature | UUID | Datentyp | Einheit |
| :--- | :--- | :--- | :--- |
| **Service** | `0x181A` | ESS | - |
| **Temperatur** | `0x2A6E` | sint16 | 0.01 °C |
| **Feuchtigkeit**| `0x2A6F` | uint16 | 0.01 % |
| **Luftdruck** | `0x2A6D` | uint32 | 0.1 Pa |

## 🔌 Hardware-Anschluss (I2C)
Die Pin-Belegung wird über ein Devicetree-Overlay (`.overlay`) definiert. Standardbeispiel für nRF5340:

| BME280 Pin | nRF5340 Pin |
| :--- | :--- |
| **VCC** | 3.3V |
| **GND** | GND |
| **SCL** | P1.04 (SCL) |
| **SDA** | P1.05 (SDA) |

## 🚀 Installation & Build

1. **Projekt öffnen:**
   Öffne diesen Ordner in VS Code. Stelle sicher, dass das **nRF Connect Toolchain** korrekt konfiguriert ist.

2. **Konfiguration (`prj.conf`):**
   Wichtige Symbole müssen aktiviert sein:
   ```config
   CONFIG_I2C=y
   CONFIG_SENSOR=y
   CONFIG_BME280=y
   CONFIG_BT=y
   CONFIG_BT_PERIPHERAL=y
   CONFIG_BT_DEVICE_NAME="nRF5340_Sensor"
