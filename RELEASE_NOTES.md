# Release Notes

## 2026-08-10 - Version 2.0.0

> [!NOTE]
> Major project restructuring release with package-based configs, improved Home Assistant integration, and a fully reworked README.

### 📦 Package and template split

The project structure was reorganized so the iFan04-specific ESPHome logic is now stored in dedicated package files instead of being duplicated directly inside the template files.

Previously, the repository mainly consisted of template files that each carried the full device configuration. With this change:

- `main.yaml` contains the default iFan04 package logic
- `main_adv_buzzer.yaml` contains the advanced buzzer variant
- `main_4_speeds.yaml` contains the alternate four-speed fan variant
- the template files now act as starter configurations that import one of those packages

This split makes the project easier to maintain, reuse, and extend because the shared iFan-specific behavior lives in one place per variant instead of being repeated across templates.

> **⭐ Special thanks to @aso824 for the idea to split the configuration using packages.**

### 🔔 Buzzer behavior update

The default buzzer behavior was changed so sound is only produced when a button on the RF remote control is pressed.

This keeps audible feedback tied to explicit remote-control actions and avoids broader buzzer activity during normal device operation.

### 🏠 Home Assistant RC events

Remote-control commands are now forwarded to Home Assistant as events.

The previously used `RC button ...` binary sensors were replaced by these RC command events, making it easier to build automations from remote button presses without maintaining a separate binary sensor for each button action.

### 📘 README rework

The README was completely reworked to better explain the project structure, package variants, templates, hardware details, and typical usage patterns.

It now separates stable project documentation from dated change history, with ongoing release-specific details moved into this release notes document.

## 2025-08-24


### 🚀 OTA updates and ESPHome 2025.8.0

Firmware size improved enough with ESPHome 2025.8.0 that OTA updates became practical again for the templates in this repository.

The logger level was also reduced to `INFO` to save additional flash space compared to the default `DEBUG` level.

| Configuration | ESPHome 2025.6.1 binary size | ESPHome 2025.6.1 flash usage | ESPHome 2025.8.0 binary size | ESPHome 2025.8.0 flash usage |
| ------------- | ---------------------------- | ---------------------------- | ---------------------------- | ---------------------------- |
| `template.yaml` | 615257 | 60.1% | 573337 | 56.0% |
| `template_simple.yaml` | 574069 | 56.1% | 533585 | 52.1% |
| `template.yaml` with `logger.level = INFO`, no `web_server`, and no fallback AP / `captive_portal` | 523163 | 51.1% | 497071 | 48.5% |
| New `template.yaml` with `logger.level = INFO` | - | - | 561737 | 54.9% |

## 2025-02-22

> [!NOTE]
> External component migration release with buzzer improvements and the four-speed variant.

### 🔧 External component migration

Support for ESPHome custom components was removed, so the RF receiver implementation was migrated to an external component named `ifan_remote`.

Repository:

- https://github.com/rh1rich/esphome-ifan-remote

At the same time, the buzzer handling was expanded through a new external component named `morse_code`.

Repository:

- https://github.com/rh1rich/esphome-morse-code

### 🔔 Buzzer improvements

Remote button presses can produce buzzer feedback when the buzzer is enabled.

The advanced buzzer variant also supports a user-defined Home Assistant action named `play_morse_code`, allowing arbitrary text to be rendered as Morse code on the buzzer.

### 🌀 Four-speed fan variant

An alternate four-speed template and package were introduced for installations where the capacitor setup makes four distinct fan speeds useful.

Relay mapping:

| Fan speed | Used relays |
| --------- | ----------- |
| low | 1 |
| mid 1 | 2 |
| mid 2 | 1 + 2 |
| high | 3 |

This is most relevant when the fan capacitors are changed to values that make the two middle states measurably different.