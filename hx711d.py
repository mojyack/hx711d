import subprocess


class HX711:
    def __init__(self, executable, gpiochip, clk_pin, dout_pin):
        self.hx711d = subprocess.Popen(
            [executable, gpiochip, str(clk_pin), str(dout_pin)],
            encoding="utf-8",
            stdin=subprocess.PIPE,
            stdout=subprocess.PIPE,
        )

    def __del__(self):
        if self.hx711d.poll() == None:
            self.quit()

    def send_command(self, command):
        self.hx711d.stdin.write(command + "\n")
        self.hx711d.stdin.flush()
        return self.hx711d.stdout.readline()[:-1]

    def read_value(self):
        return float(self.send_command("r"))

    def quit(self):
        return self.send_command("q")

    # helpers
    def read_average(self, times):
        total = 0
        for i in range(times):
            total += self.read_value()
        return total / times

    def read_median(self, size):
        arr = []
        for i in range(size * 2 + 1):
            arr.append(self.read_value())
        arr.sort()
        return arr[size]


def calibrate(executable, gpiochip, clk_pin, dout_pin):
    hx = HX711(executable, gpiochip, clk_pin, dout_pin)
    offset = hx.read_average(10)
    load = float(input("weight in g: "))
    value = hx.read_average(10) - offset
    scale = value / load
    print(f"offset={offset} scale={scale}")


if __name__ == "__main__":
    import sys

    if len(sys.argv) != 5:
        print("usage: python hx711d.py <hx711_path> <gpiochip> <clk_pin> <dout_pin>")
        exit(1)
    calibrate(sys.argv[1], sys.argv[2], int(sys.argv[3]), int(sys.argv[4]))
