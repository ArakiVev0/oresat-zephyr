# oresat-zephyr

OreSat firmware projects using Zephyr RTOS.

## Setup

**NOTE:** This mostly rework of https://docs.zephyrproject.org/latest/develop/getting_started/index.html

### Install dependencies

- Arch Linux

    ```bash
    sudo pacman -S git cmake ninja gperf ccache dfu-util dtc wget \
      python-dev python-pip python-setuptools python-tk python-wheel \
      xz file make sdl2 stlink
    ```

- Debian Linux

    ```bash
    sudo apt install --no-install-recommends git cmake ninja-build gperf \
      ccache dfu-util device-tree-compiler wget \
      python3-dev python3-pip python3-setuptools python3-tk python3-wheel \
      xz-utils file make libsdl2-dev libmagic1 stlink-tools
    ```

### Install Zephyr SDK

- Arch Linux (from AUR)

    ```bash
    paru -S zephyr-sdk-bin
    ```

- Everything else

    ```bash
    cd /opt
    wget https://github.com/zephyrproject-rtos/sdk-ng/releases/download/v0.16.8/zephyr-sdk-0.16.8_linux-x86_64.tar.xz
    tar xvf zephyr-sdk-0.16.8_linux-x86_64.tar.xz
    cd zephyr-sdk-0.16.8
    ./setup.sh
    ```

### Add udev rule:

```bash
sudo cp /opt/zephyr-sdk-0.16.8/sysroots/x86_64-pokysdk-linux/usr/share/openocd/contrib/60-openocd.rules /etc/udev/rules.d
sudo udevadm control --reload
```

### Install west

```bash
pip install west
```

### Get the source code

```bash
west init -m https://github.com/oresat/oresat-zephyr --mr master zephyr-workspace
cd zephyr-workspace
west update
```

### Export Zephyr CMake package:

```bash
west zephyr-export
```

### Install the rest of Zephyr's Python dependencies

```bash
pip install -r zephyr/scripts/requirements.txt
```

### Install OreSat Configs

While the latest oresat-configs release can be install via `pip`,
for development, it is better to have latest code from the git repo.

```bash
cd oresat-configs
pip install -r requirements.txt
./build_and_install.sh
cd -
```

### Test compile and flash

```bash
cd zephyr/samples/basic/blinky
west build -p always -b nucleo_f091rc .
west flash --runner openocd
```

## Tools

- [clang-format]: Used to auto format the code. Can be installed with
  the `clang-format` package.

### CAN Tools

Since the main communication bus on OreSat is a CAN bus, CAN bus monitoring
tools are needed for development and testing.

- [candump]: Useful for monitoring if there is some CAN communication. Can be
  installed with `can-utils` package.
- [SavvyCAN]: A GUI to decode and display/graph signals from CAN messages.
  Use `oresat-configs` to generate the .dbc file used to decode CAN messages
  and their signals with SavvyCAN.

[clang-format]:https://clang.llvm.org/docs/ClangFormat.html
[candump]:https://manpages.debian.org/testing/can-utils/candump.1.en.html
[SavvyCAN]:https://github.com/collin80/SavvyCAN
