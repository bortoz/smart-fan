# Smart fan

_Smart fan_ is a small project written for my Embedded Systems exam. The project is based on [Zephyr RTOS](https://www.zephyrproject.org/) and consists of a fan that orients itself according to the room temperature.

The project is compatible with ESP32 boards, but it can be easily ported to other boards supported by Zephyr.

### Hardware

- ESP32 board
- amg8833 thermal camera
- servo motor
- fan

### Installation

First, follow the [Zephyr installation guide](https://docs.zephyrproject.org/latest/getting_started/index.html) to install West, Zephyr SDK and their dependencies.

Then clone this repository and install the required modules:
```bash
git clone https://github.com/bortoz/smart-fan.git
cd smart-fan
west init .
west update
```

### Building

```bash
west build app
```

### Flashing

```bash
west flash
```

### Serial monitor

```bash
west espressif monitor
```
