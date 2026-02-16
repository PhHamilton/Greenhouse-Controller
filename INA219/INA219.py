from smbus2 import SMBus

class INA219():
    REG_CONFIG = 0x00
    REG_SHUNT_VOLTAGE = 0x01
    REG_BUS_VOLTAGE = 0x02
    REG_POWER = 0x03
    REG_CURRENT = 0x04
    REG_CALIBRATION = 0x05

    def __init__(self, I2C_bus: int, device_addr: int):
        self.device_addr = device_addr
        self.i2c_bus = I2C_bus
        self.bus = SMBus(self.i2c_bus)

    def _write_u16(self, register: int, value: int):
        high = (value >> 8) & 0xFF
        low  = value & 0xFF
        self.bus.write_i2c_block_data(self.device_addr, register, [high, low])

    def _read_u16(self, register):
        data = self.bus.read_i2c_block_data(self.device_addr, register, 2)
        return (data[0] << 8) | data[1]

    def deInit(self):
        self.bus.close()

    def Initialize(self):
        #Config: 32v range, 320 mV shunt, 12-bit ADC
        config = 0x019F
        self._write_u16(self.REG_CONFIG, config)

        # Calibration: 0.1 Ohm shunt, 3.2A max
        calibration = 4096
        self._write_u16(self.REG_CALIBRATION, calibration)

    def _get_bus_voltage(self):
        raw = self._read_u16(self.REG_BUS_VOLTAGE)
        return (raw >> 3) * 0.004

    def _get_shunt_voltage(self):
        raw = self._read_u16(self.REG_SHUNT_VOLTAGE)

        if raw > 0x7FFF:
            raw -= 0x10000

        return raw * 0.00001

    def _get_current(self):
        raw = self._read_u16(self.REG_CURRENT)

        if raw > 0x7FFF:
            raw -= 0x10000
        return raw * 0.0001

    def _get_power(self):
        raw = self._read_u16(self.REG_POWER)
        return raw * 0.002

    def GetLatestSample(self):
        return {
                 "bus_voltage": self._get_bus_voltage(),
                 "shunt_voltage" : self._get_shunt_voltage(),
                 "current"    : self._get_current(),
                 "power"      : self._get_power()
               }

if __name__ == "__main__":
    import time
    address = 0x40
    ina219 = INA219(1, address)

    print(ina219.GetLatestSample())

    ina219.deInit()

