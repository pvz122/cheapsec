# cheapsec

A tool to inspect heap exploitabilities and security checks for glibc, just like `checksec`.

## Install

Firstly, clone the repository.

Then make sure you have installed `gcc` and `python3`.

Finally, run:

```shell
sudo ./install.sh
```

## Usage

Run `cheapsec -h` for usage:

```shell
root@pwn:~# cheapsec -h

Usage:  cheapsec        <executable path | glibc path>
        cheapsec -t[es] <executable path | glibc path>
        cheapsec -l     <glibc version>
        cheapsec -h

Options:
    [no option]:    Lookup exploitabilities and security checks for glibc library
    -l:             Lookup exploitabilities and security checks for the specified glibc version
    -t:             Run tests for exploitabilities and security checks
    -te:            Run tests for exploitabilities
    -ts:            Run tests for security checks
    -h:             Print help message

Arguments:
    <executable path>:      Path to executable, cheapsec will test the glibc library linked with executable
    <glibc path>:           Path to glibc library, e.g. /lib/x86_64-linux-gnu/
    <glibc version>:        Version of glibc library, e.g. 2.27
```
