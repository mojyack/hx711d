#include <chrono>
#include <thread>

#include <gpiod.h>

#include "hx711.hpp"
#include "util.hpp"

namespace {
auto usleep(const int microsecs) -> void {
    std::this_thread::sleep_for(std::chrono::microseconds(microsecs));
}
} // namespace

namespace hx711 {
auto HX711::set_clk(const int value) -> void {
    DYN_ASSERT(gpiod_line_set_value(clk, value) == 0);
}

auto HX711::get_dat() const -> int {
    const auto v = gpiod_line_get_value(dat);
    DYN_ASSERT(v != -1);
    return v;
}

auto HX711::is_ready() const -> bool {
    return get_dat() == 0;
}

auto HX711::read_bit() -> uint8_t {
    set_clk(1);
    set_clk(0);
    return get_dat();
}

auto HX711::read_byte() -> uint8_t {
    auto value = 0;
    for(auto i = 0; i < 8; i += 1) {
        value <<= 1;
        value |= read_bit();
    }
    return value;
}

auto HX711::read_word(const InputSelect next_gain) -> int {
    while(!is_ready()) {
        usleep(10000);
    }
    auto value = 0;
    for(auto i = 0; i < 3; i += 1) {
        value <<= 8;
        value |= read_byte();
    }
    for(auto i = 0; i < int(next_gain); i += 1) {
        set_clk(1);
        set_clk(0);
    }
    return -(value & 0x800000) + (value & 0x7FFFFF);
}

auto HX711::power_off() -> void {
    set_clk(0);
    set_clk(1);
    usleep(100);
}

auto HX711::power_up() -> void {
    set_clk(0);
    usleep(100);
}

auto HX711::create(const char* const chip_name, const int clk_num, const int dat_num) -> HX711 {
    const auto chip = gpiod_chip_open_by_name(chip_name);
    DYN_ASSERT(chip != NULL);

    const auto clk = gpiod_chip_get_line(chip, clk_num);
    DYN_ASSERT(clk != NULL);
    DYN_ASSERT(gpiod_line_request_output(clk, "hx711", 1) == 0);

    const auto dat = gpiod_chip_get_line(chip, dat_num);
    DYN_ASSERT(dat != NULL);
    DYN_ASSERT(gpiod_line_request_input(dat, "hx711") == 0);

    return {clk, dat};
}
} // namespace hx711
