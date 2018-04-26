# Steps to setup on a fresh SD card

## Download Raspbian Lite
Normal Raspbian will work, but it's a bit bloated.

## Write image to card

## Boot the Pi

## Configure
use `sudo raspi-config`

### Correct keyboard layout if needed
Try typing `#`, `~` and `|`. If They don't all work, switch the keyboard layout in localization options. This takes effect immediately.

### Connect to WiFi network

### Enable SPI for display

## Install required packages
```
#               |      utils    |  build  | epdif  |                 hedgehog                   |clients
sudo apt install network-manager git cmake wiringpi libxkbcommon-dev libinput-dev libwayland-dev weston
```

## Clone the git repo
```
git clone https://github.com/wmww/scrimshaw
```

## More to follow, stay tuned.

