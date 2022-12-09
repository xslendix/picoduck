alias arduino="arduino-cli --additional-urls 'https://github.com/earlephilhower/arduino-pico/releases/download/global/package_rp2040_index.json'"

arduino core update-index

arduino core install 'rp2040:rp2040'
arduino board attach -b 'rp2040:rp2040:rpipico' --board-options 'flash=2097152_1048576,usbstack=tinyusb'
arduino lib install 'Adafruit SPIFlash'
arduino config set sketch.always_export_binaries true

arduino compile -b 'rp2040:rp2040:rpipico' -e --board-options 'flash=2097152_1048576,usbstack=tinyusb'

