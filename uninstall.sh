#! /bin/bash

# check root
if [ "$EUID" -ne 0 ]; then
    echo "Please run as root"
    exit
fi

# rm /usr/local/bin/cheapsec
rm /usr/local/bin/cheapsec

# rm -r /opt/cheapsec
rm -r /opt/cheapsec
