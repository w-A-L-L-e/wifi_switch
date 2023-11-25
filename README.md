#   Wifi Web Light Switch v1 beta

Uses wemos Esp8266 module and relay to allow switching lights over the internet.

This allows to switch on lights over the wifi. When board first boots it starts as access point. You can connect to it with browser by joining
its created wifi network. It then allows selecting wifi network + set user + pass in the browser. After this it boots and shows a webpage
that allows toggling a relay on/off in the browser.

Screenshot of the webpage you will see:
<img width="515" alt="Screenshot 2023-11-25 at 04 21 09" src="https://github.com/w-A-L-L-e/wifi_switch/assets/710803/dad158c0-2f42-4b72-8523-543f46e5aae1">

Webpage with actual device soldered together:
![relay_board_off](https://github.com/w-A-L-L-e/wifi_switch/assets/710803/96335c53-61ec-45bf-a9d6-8bd71e06789a)

When pressing the button, the state changes to ON and also the relay + leds switch on:
![relay_board_on](https://github.com/w-A-L-L-e/wifi_switch/assets/710803/c78237ca-83c6-49d1-bb8d-97717bc92300)
