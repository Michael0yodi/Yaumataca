/**
 * @file hid_generic_snes.cpp
 * @author Michael Roslund
 * @brief
 * @version 0.1
 * @date 2026-05-17
 *
 * @copyright Copyright (c) 2026
 *
 */

#include "default_hid_handler.hpp"
#include "hid_handler_builder.hpp"

#include "controller_port.hpp"
#include "pico/stdlib.h"

class GenericSnesHidHandler : public DefaultHidHandler {
  public:
    void process_report(std::span<const uint8_t> d) override {
        if (d.size() < 6) {
            return;
        }

        const uint8_t x = d[0];
        const uint8_t y = d[1];
        const uint8_t buttons = d[5];

        GamepadReport aj;

        aj.left = x < 0x40;
        aj.right = x > 0xC0;
        aj.up = y < 0x40;
        aj.down = y > 0xC0;

        // Button 1 green: 0x8F
        aj.fire = (buttons & 0x80) != 0;

        // Button 2 yellow: 0x4F
        aj.sec_fire = (buttons & 0x40) != 0;

        // Button 4 blue: 0x1F
        // Autofire
        aj.auto_fire = (buttons & 0x10) != 0;

        // Button 3 red: 0x2F
        aj.third_fire = (buttons & 0x20) != 0;

        if (target_) {
            target_->process_gamepad_report(aj);
        }
    }

    ReportType expected_report() override {
        return kGamePad;
    }
};

static HidHandlerBuilder builder(0x081f, 0xe401, []() { return std::make_unique<GenericSnesHidHandler>(); }, nullptr);