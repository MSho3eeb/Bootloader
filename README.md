**Tiva C Bootloader Demo**
This project demonstrates a simple bootloader implementation for the Tiva C series microcontroller. The bootloader allows you to switch between two different applications stored in flash memory by pressing the reset button.

**Table of Contents**
Overview
Hardware Requirements
Project Structure
How It Works
Getting Started


**Overview**
Bootloaders are essential for updating firmware in embedded systems without the need for specialized programming equipment. This project showcases a bootloader for the Tiva C microcontroller, allowing you to load and switch between two different applications stored in flash memory. This can be particularly useful for testing and managing different firmware versions on your device.

**Hardware Requirements**
Tiva C series microcontroller (e.g., TM4C123GXL)
Code Composer Studio or another suitable IDE for flashing and debugging
Project Structure
The project is organized as follows:

Bootloader: The bootloader code is located in the bootloader directory. It is responsible for managing the applications in flash memory and controlling the application switching logic.

App1 and App2: These directories contain the code for the two applications you want to switch between. Each application is compiled separately and stored in different flash memory regions.

How It Works
The memory is divided into four sections:

0x0000 to 0x9FFC: Running application section
0x10000 to 0x10FFC: Hex code of App 1
0x20000 to 0x20FFC: Hex code of App 2
0x30000 to 0x40000: Bootloader section
The vector table is shifted between the bootloader's vector table and the running application's vector table.

On the first reset, the bootloader runs, allowing you to choose which application to run.

After selecting an application, the bootloader calls the reset of that application to initiate a hard reset.

You can switch between App 1 and App 2 by pressing the reset button. The bootloader allows you to choose which application to run on each reset.

Getting Started
Clone this repository to your development machine.

Open the project in your preferred IDE (e.g., Code Composer Studio).

Compile and flash the bootloader onto your Tiva C microcontroller.

Compile and flash App 1 and App 2 onto your microcontroller, each to their respective memory regions.

Connect your Tiva C microcontroller to a power source and press the reset button to begin using the bootloader.

Follow the on-screen instructions to select and run either App 1 or App 2.

Enjoy testing and switching between different applications on your Tiva C microcontroller using the bootloader!

