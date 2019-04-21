# Project Scrimshaw

Code for my WIP Raspbery Pi based E-ink smartwatch prototype.

## Additional notes

To get libinput to work (thus get keybord working on Pi or for testing), run
```
sudo usermod -a -G input $USER
```
This will give the user raw access to input devices. so maybe not the best idea if you're concerned about keyloggers
