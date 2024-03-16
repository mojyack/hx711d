#pragma once
#include <cstdint>

#include <gpiod.h>

namespace hx711 {
// input channel and gain selection
enum class InputSelect {
    a128 = 1,
    a64  = 3,
    b32  = 2,
};

struct HX711 {
    gpiod_line* clk;
    gpiod_line* dat;

    auto set_clk(int value) -> void;

    auto get_dat() const -> int;

    auto is_ready() const -> bool;

    auto read_bit() -> uint8_t;

    auto read_byte() -> uint8_t;

    auto read_word(InputSelect next_gain) -> int;

    auto power_off() -> void;

    auto power_up() -> void;

    static auto create(const char* chip_name, int clk_num, int dat_num) -> HX711;
};
} // namespace hx711
