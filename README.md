# Práctica 5 - Ejercicio 2: Lectura de etiquetas RFID (RC522) con ESP32-S3

## Descripción

En este ejercicio se implementa la lectura de etiquetas RFID utilizando el módulo **RC522** mediante comunicación **SPI** con una placa **ESP32-S3**.

El sistema detecta tarjetas RFID cercanas, lee su identificador único (**UID**) y lo muestra a través del **puerto serie**, permitiendo identificar cada tarjeta de forma individual.

---

## Objetivos

* Comprender el uso del bus **SPI** con periféricos externos.
* Aprender a inicializar y utilizar el módulo **RFID RC522**.
* Detectar tarjetas RFID y leer su UID.
* Mostrar datos en el monitor serie para depuración.
* Integrar librerías externas en **PlatformIO**.

---

## Material utilizado

* Placa **ESP32-S3**
* Módulo **RFID RC522**
* Tarjetas o llaveros RFID
* Cables Dupont
* **Visual Studio Code + PlatformIO**

---

## Conexión SPI (RC522 → ESP32-S3)

| RC522    | ESP32-S3 |
| -------- | -------: |
| SDA (SS) |  GPIO 10 |
| SCK      |  GPIO 12 |
| MOSI     |  GPIO 11 |
| MISO     |  GPIO 13 |
| RST      |   GPIO 9 |
| GND      |      GND |
| 3.3V     |  3.3V ⚠️ |

> ⚠️ Importante: El módulo RC522 funciona a **3.3V**, no utilizar 5V.

---

## Configuración de PlatformIO

Archivo `platformio.ini`:

```ini
[env:esp32-s3-devkitm-1]
platform = espressif32
board = esp32-s3-devkitm-1
framework = arduino

monitor_speed = 115200

lib_deps =
    miguelbalboa/MFRC522
```

---

## Código implementado

```cpp
#include <SPI.h>
#include <MFRC522.h>

#define SS_PIN 10
#define RST_PIN 9

MFRC522 mfrc522(SS_PIN, RST_PIN);

void setup() {
  Serial.begin(115200);
  delay(2000);

  SPI.begin(12, 13, 11, SS_PIN);
  mfrc522.PCD_Init();

  Serial.println("Lectura del UID");
}

void loop() {
  if (mfrc522.PICC_IsNewCardPresent()) {
    if (mfrc522.PICC_ReadCardSerial()) {

      Serial.print("Card UID:");

      for (byte i = 0; i < mfrc522.uid.size; i++) {
        Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
        Serial.print(mfrc522.uid.uidByte[i], HEX);
      }

      Serial.println();

      mfrc522.PICC_HaltA();
    }
  }
}
```

---

## Funcionamiento del programa

### 1. Inicialización

* Se inicia la comunicación serie a 115200 baudios.
* Se inicializa el bus SPI.
* Se configura el módulo RC522.

---

### 2. Detección de tarjeta

El sistema comprueba continuamente si hay una tarjeta presente:

```cpp
mfrc522.PICC_IsNewCardPresent();
```

---

### 3. Lectura del UID

Si se detecta una tarjeta, se lee su identificador único:

```cpp
mfrc522.PICC_ReadCardSerial();
```

---

### 4. Visualización por puerto serie

El UID se imprime en formato hexadecimal:

```text
Card UID: 3A 7F 21 9C
```

Cada tarjeta RFID tiene un UID diferente.

---

### 5. Finalización de lectura

Se detiene la comunicación con la tarjeta actual:

```cpp
mfrc522.PICC_HaltA();
```

---

## Salida esperada

### Al iniciar el sistema:

```text
Lectura del UID
```

### Al acercar una tarjeta:

```text
Card UID: XX XX XX XX
```

---

## Posibles errores

### No detecta tarjetas

* Conexión incorrecta de pines
* Falta inicialización de SPI
* Alimentación incorrecta (usar 3.3V)

---

### Error de compilación

```text
MFRC522.h: No such file or directory
```

**Solución:** añadir la librería en `platformio.ini`.

---

### No aparece nada en el monitor serie

* Monitor serie cerrado
* Velocidad incorrecta
* No se ha reiniciado la placa

---

## Conclusión

Este ejercicio demuestra cómo utilizar un lector RFID mediante SPI para identificar tarjetas de forma única.

El uso del módulo **RC522** junto con la **ESP32-S3** permite implementar sistemas de identificación, control de accesos o registro de usuarios, siendo una base fundamental para proyectos más avanzados.

---
