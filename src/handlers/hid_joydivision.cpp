/**
 * @file hid_joydivision.cpp
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

class JoyDivisionHidHandler : public DefaultHidHandler {
  public:
    void process_report(std::span<const uint8_t> d) override {
        if (d.size() < 3) {
            return;
        }

        const uint8_t buttons = d[0];
        const int8_t x = static_cast<int8_t>(d[1]);
        const int8_t y = static_cast<int8_t>(d[2]);

        GamepadReport aj;

        aj.fire = (buttons & 0x01) != 0;
        aj.sec_fire = (buttons & 0x02) != 0;
        aj.auto_fire = (buttons & 0x04) != 0;

        aj.left = x < -32;
        aj.right = x > 32;
        aj.up = y < -32;
        aj.down = y > 32;

        if (target_) {
            target_->process_gamepad_report(aj);
        }
    }

    ReportType expected_report() override {
        return kGamePad;
    }
};

static HidHandlerBuilder builder(0x16c0, 0x27dc, []() { return std::make_unique<JoyDivisionHidHandler>(); }, nullptr);