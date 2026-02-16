from gpiozero import DigitalOutputDevice

class OutputHandler():
    def __init__(self, gpio_pin: int, active_high: bool = True):
        self.gpio_pin = gpio_pin
        self.device = DigitalOutputDevice(gpio_pin, active_high = active_high)
        self.output_enabled = False

    def TurnOn(self):
        self.device.on()
        self.output_enabled = True

    def TurnOff(self):
        self.device.off()
        self.output_enabled = False

    def Toggle(self):
        if self.output_enabled:
            self.TurnOff()
            self.output_enabled = False
        else:
            self.TurnOn()
            self.output_enabled = True

    def GetOutputState(self):
        return self.output_enabled

    def Disable(self):
        self.device.close()


if __name__ == "__main__":
    import time
    output = OutputHandler(4)

    output.TurnOn()
    time.sleep(5)
    output.TurnOff()
    output.Disable()
