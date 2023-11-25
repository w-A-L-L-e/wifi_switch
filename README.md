#   Wifi Web Light Switch v1 beta

Uses wemos Esp8266 module and relay to allow switching lights over the internet.

This allows to switch on lights over the wifi. When board first boots it starts as access point. You can connect to it with browser by joining
its created wifi network. It then allows selecting wifi network + set user + pass in the browser. After this it boots and shows a webpage
that allows toggling a relay on/off in the browser.

Webpage with actual device shown here. The only tricky part to get working was the relais itself. Because we have a 3.3v level pins on the wemos d1 we also need to connect the VCC to the 3.3v and not the 5v. With better opto coupled boards you normally take the 5v line here. But this got the relay stuck in permanent on mode. By just connecting following lines everything works reliably (however its best to also supply 3.3v directly now as the regulator does not like the relais on for long periods, its getting noticeably hot after a while).
```
Connections:
VCC  -> 3.3V on wemos
In1  -> D3 pin (GPIO0) 
GND  -> GND
```

![relay_board_off](https://github.com/w-A-L-L-e/wifi_switch/assets/710803/96335c53-61ec-45bf-a9d6-8bd71e06789a)


When pressing the button, the state changes to ON and also the relay + leds switch on:

![relay_board_on](https://github.com/w-A-L-L-e/wifi_switch/assets/710803/c78237ca-83c6-49d1-bb8d-97717bc92300)
