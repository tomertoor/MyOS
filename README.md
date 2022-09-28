# Operating System

## Requieremnts

 - [Docker](https://www.docker.com/) - for building the OS.
 - [Qemu](https://www.qemu.org/) - emulator.


## Setup

Build an image for our build-environment:
 - `docker build buildenv -t myos-buildenv`

## Build

Enter build environment:
 - Linux or MacOS: `docker run --rm -it -v "$(pwd)":/root/env myos-buildenv`
 - Windows (CMD): `docker run --rm -it -v "%cd%":/root/env myos-buildenv`
 - Windows (PowerShell): `docker run --rm -it -v "${pwd}:/root/env" myos-buildenv`

Build for x86 (other architectures may come in the future):
 - `make build-x86_64`
 - If you are using Qemu, please close it before running this command to prevent errors.

To leave the build environment, enter `exit`.

## Emulate


Run the ISO file with:
 - `qemu-system-x86_64 -cdrom dist/x86_64/kernel.iso`
 - Note: Close it after using.
