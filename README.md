# DryerBuzzer

The dryer in my apartment doesn't have an end-of-cycle chime so I paired an ESP32 with a cheap vibration sensor, a battery from a vape that my ex left behind, and some code. When the dryer is done, you'll hear the buzzer if you're around and get an email in case you're out and about.


# BOM

 - [Seeed Studio XIAO ESP32C3](https://www.amazon.com/gp/aw/d/B0B94JZ2YF): This contains a lithium charging circuit
 - [SW-420 Vibration Sensor](https://www.amazon.com/dp/B0BKZ7L1SS)
 - [Mini Active Piezo Buzzer](https://www.amazon.com/UPEYXDQR-Electronic-Computers-Components-12x9-5mm/dp/B0GQPF7MDH)
 - [Self-Lock Micro Push Button Switch](https://www.amazon.com/dp/B0BZHZDBBY)
 - An LED
 - An old vape your ex left behind
 - Wire, solder, etc.


## Details

Vibration sensor: The vibration sensor is kind of neat in the simple way it works. There's basically a rod that floats inside of a coil and when it makes contact with the coil, you get a 1 and when there's no contact you get 0.

Captive Portal: It uses the NetWizard library to create its own wifi network. Once connected to it, you enter your regular wifi info to get you connected to your wifi.

Configuration: Using mdns, you'll be able to reach the configuration page at http://dryerBuzzer.local

<img width="1007" height="959" alt="image" src="https://github.com/user-attachments/assets/c5a44946-7f01-471e-8013-f1e8b6cb7add" />

