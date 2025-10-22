#!/bin/bash

IMAGE_FILE="/home/bright/Documents/EpaperQt/test.bmp"
REMOTE_USER="pi"
REMOTE_HOST="192.168.8.39"
REMOTE_PATH="~/e-Paper/E-paper_Separate_Program/4inch_e-Paper_E/RaspberryPi_JetsonNano/c/pic"

echo "Copying image to Raspberry Pi..."
scp "$IMAGE_FILE" "$REMOTE_USER@$REMOTE_HOST:$REMOTE_PATH"

echo "Connecting to Raspberry Pi and running commands..."
ssh "$REMOTE_USER@$REMOTE_HOST" << 'EOF'
cd e-Paper/E-paper_Separate_Program/4inch_e-Paper_E/RaspberryPi_JetsonNano/c/
sudo make clean
sudo make -j4
./epd
