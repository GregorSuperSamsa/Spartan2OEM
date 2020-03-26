# Spartan 2 OEM I2C
![alt text](https://raw.githubusercontent.com/GregorSuperSamsa/Spartan2OEM/master/documentation/Spartan2_OEM.png)\
\
Spartan 2 OEM I2C is a low-cost, miniature Wideband O2 Controller with I2C communication. Up to 16 devices may be addressed on a single bus.\
\
This Arduino library is based on the examples provided by Alan To at [14point7](https://www.14point7.com), developer of the Spartan2 OEM I2C.

### Hardware requirements
Connect Arduino SDA to Spartan 2 OEM SDA.\
Connect Arduino SLC to Spartan 2 OEM SCL.\
Both SDA and SLC lines require 4.7k pull-up resistors.

### Usage

Create object of type Spartan 2 OEM:
```
Spartan2OEM::Spartan2OEM();
```

Initialize the library instance with a pointer to the Wire library and the I2C address of the slave device:
```
void Spartan2OEM::Initialize(TwoWire* i2c_bus, const uint8_t &i2c_address);
```

Request I2C data from the slave:
```
bool Spartan2OEM::RequestData();
```

Process received data:
```
void Spartan2OEM::ProcessData();
```

Get Lambda:
```
float Spartan2OEM::Lambda() const;
```

Get AFR ratio:
```
float Spartan2OEM::AfrRatio() const;
```

Get sensor temperature in degrees Celsius:
```
uint16_t Spartan2OEM::TemperatureC() const;
```

Get sensor temperature in degrees Fahrenheit:
```
uint16_t Spartan2OEM::TemperatureF() const;
```

Change slave I2C address:\
***Please, refer to the Spartan 2 OEM I2C manual for enabling this functionality***
```
bool Spartan2OEM::ChangeI2CAddress(const uint8_t &new_address);
```

