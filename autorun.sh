cd build
cmake .. || { echo 'cmake failed' ; exit 1; }
make hedgehog_epdif || { echo 'make failed' ; exit 1; }
./hedgehog_epdif &
sleep 2
weston-terminal
