#include "hx711.hpp"
#include "util.hpp"

auto main(const int argc, const char* const argv[]) -> int {
    if(argc != 4) {
        print("usage: hx711 <gpiochip> <clk_pin> <dout_pin>");
        return 1;
    }

    auto hx = hx711::HX711::create(argv[1], atoi(argv[2]), atoi(argv[3]));

    hx.power_off();
    hx.power_up();

    // throw the first value to set gain
    hx.read_word(hx711::InputSelect::a128);

    while(true) {
        auto cmd = std::string();
        std::getline(std::cin, cmd);
        if(cmd.size() == 0) {
            continue;
        }
        switch(cmd[0]) {
        // read
        case 'r':
            print(hx.read_word(hx711::InputSelect::a128));
            break;
        // quit
        case 'q':
            return 0;
        default:
            warn("unknown command: ", cmd);
            break;
        }
    }
    return 0;
}
