#! /bin/bash

# Requirements: gcc, python3
if ! [ -x "$(command -v gcc)" ]; then
    echo "Error: gcc is not installed." >&2
    exit 1
fi
if ! [ -x "$(command -v python3 --version)" ]; then
    echo "Error: python3 is not installed." >&2
    exit 1
fi

# check root
if [ "$EUID" -ne 0 ]; then
    echo "Please run as root"
    exit
fi

echo "Installing cheapsec..."

# mkdir /opt/cheapsec
mkdir /opt/cheapsec

# copy data, src and testing to /opt/cheapsec
cp -r data /opt/cheapsec/data
cp -r src /opt/cheapsec/src
cp -r testing /opt/cheapsec/testing

# make cheapsec executable
chmod +x /opt/cheapsec/src/cheapsec.py

# create symlink to cheapsec in /usr/local/bin
ln -s /opt/cheapsec/src/cheapsec.py /usr/local/bin/cheapsec

echo "Done."
