# Introduction

This repo contains a simple test of Deep Sleep power saving for the `EVK-NINA-W10`.  It checks that a value can be written to the retained registers, a wake-up time can be set in the RTC and then the processor can be put to sleep and wake up on the RTC interrupt with the remembered thing still there.

# Building And Downloading

To build this project you need to follow the instructions here to setup up the Espressif build environment; I used the Windows version:

https://docs.espressif.com/projects/esp-idf/en/latest/get-started/index.html#setup-toolchain

Then clone this project into the `c:/msys32/home/user-name/esp` directory, where `user-name` is replaced with your user name within the Espressif build environment.

You should then run the Espressif `msys` command prompt, `cd` into the build directory `~/esp/nina-w10-power-test` and run `make menuconfig` to tell it which `COM` port your `EVK-NINA-W10` is attached to.  You can then build and download the code from the command prompt by entering `make flash` and, when the flash programmer starts addressing your `COM` port, hold down the `BOOT` button on the `EVK-NINA-W10` and press the `RESET` button on the  `EVK-NINA-W10` (while still holding `BOOT` down).  Once programming has commenced you can release both buttons.

You can also build under Eclipse ("Eclipse for C/C++ Development", AKA CDT); just use `File -> Import...` and, in the dialog that pops up, choose `C/C++ -> Import Existing Projects into Workspace`, set the root directory to `c:/msys32/home/user-name/esp/nina-w10-power-test` (where `user-name` is replaced with your user name within the Espressif build environment) and import that project.  You may then need to set up the Eclipse environment, which is specific to that project, as described here:

https://docs.espressif.com/projects/esp-idf/en/latest/get-started/eclipse-setup.html 

When building under Eclipse it is not always clear when the build has failed: you need to watch for red lines in the console window.  It does, however, give you nice context highlighting in the editor window and search capability, so it is worth doing if you intend to spend some time doing development.

# Running

Once the `EVK-NINA-W10` board has been programmed, run `make monitor` at the Espressif `msys` command prompt to run the Espressif debug environment (IDF Monitor) and then press the `RESET` button on the `EVK-NINA-W10` board.  With the IDF Monitor you can view the system state and any `printf()` output.