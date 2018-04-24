# Steps to setup on a fresh SD card

## Set up the SD card

### Download Raspbian Lite
Normal Raspbian will work, but it's a bit bloated.

### Write image to card
Leave the card mounted for following steps

### Allow first WiFi connection
make a file on the SD card `/etc/network/interfaces.d/20-wifi` with the following contents (replacing `ssid` and `password` with the ssid and password of your current WiFi network). This is just so you can install the initial packages, you will be able to connect to other networks later.
```
auto lo

iface lo inet loopback
iface eth0 inet dhcp

allow-hotplug wlan0
auto wlan0

iface wlan0 inet dhcp
		wpa-ssid "ssid"
		wpa-psk "password"
```

### Enable SPI
SPI is needed for the display. In `/boot/config.txt` uncomment `dtparam=spi=on`.

## Install on the Pi

### Boot the Pi

### Correct keyboard layout
Try typing `#`, `~` and `|`. If They don't all work, do `sudo raspi-config` and switch the keyboard layout in localization options. This takes effect immediately.

### Install required packages
```
sudo apt install network-manager git wiringpi
```

## Clone the git repo
```
git clone https://github.com/wmww/scrimshaw
```

## More to follow, stay tuned.

