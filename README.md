# MicroUI

MicroUI is a tiny UI framework by @rxi. It runs in ~1100 SLOC of C.
The bundled example uses SDL2 to demonstrate the framework's capabilities.

I've ported it to Linux C++ so that it runs without an X server.

## Building

Simple:

```bash
# Install dependencies
sudo apt install libcairo2-dev libpango1.0-dev libinput-dev

git clone https://github.com/J-Cake/microui-framebuffer.git --recursive
# make sure ./microui is successfully cloned

make libmicro.so app
```

## Running

Where it gets tricky, is deploying it.

I'm using a QEMU VM running Ubuntu, so I've set up an SSH system which can deploy the compiled app / library automatically.
To replicate my setup, you'll need a VM. I chose Ubuntu Server because it has no graphics stack. Through a not-so-simple process,
I enabled SSH access, and pushed the two files to it.
> **Tip**: If you copy the built library file over SCP, then move it to `/usr/lib`, it'll retain its permissions,
> and you can use the makefile's `install` script to push updates automatically.

Once here, the user will need to become a member of the `input` and `video` groups, unless you have SUDO access (and can be bothered to type it in every time)

```bash
usermod -a -G input $USER
usermod -a -G video $USER
```

Once that's done, the example should run like any other executable.

```
./build/app
```
