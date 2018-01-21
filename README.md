# testrepo



|||||||||||||||||||||||||||||||||||||List of exercices per chapter:|||||||||||||||||||||||||||||||||||||

====================================================================================1 Getting Started



====================================================================================2 Introduction to the STM32 F1
exaux2dot1______________
Exercise description:
	Example programming without Standard Peripheral Library using LEDs and Button
Etc:
	-
====================================================================================3 Skeleton Program
ex3dot1______________GDB on STM32
Exercise description:
	First simple program loaded into STM32 device
	Experiment GDB comands with the given program code
Etc:	-

====================================================================================4 STM32 Configuration
ex4dot1______________Blinking Lights
Exercise description:
	"Blinking Lights" first program that actualy uses peripherals (GPIOs)
	Try also "assertion violation" function utility
Etc:
	-
ex4dot2______________Blinking Lights with Pushbutton
Exercise description:
	More work on the previous exercise to use and aditional LED and pushbutton
Etc:
	-
ex4dot3______________Configuration without Standard Peripheral Library
Exercise description:
	Simple program without using Standard Peripheral Library
	This program uses a LED and pushbutton
Etc:
	-


====================================================================================5 Asynchronous Serial Communication
ex5dot1______________Testing the USB/UART Interface
Exercise description:
	Just use USB-UART bridge to generate signals and read them with a logic analyser/osciloscope (no code)
Etc:
	testing the USB/UART Interface
	Used comands:
		sudo stty -F /dev/ttyUSB0    //para configurar
		screen /dev/ttyUSB0      //para mandar comando, precisa do sudo
ex5dot2______________Hello World!
Exercise description:
	Simple application that repeatedly sends the string “hello world” from the STM32 to a host computer
Etc:
	-
ex5dot3______________Echo
Exercise description:
	In this program the STM32 echoes its UART input back to the host
Etc:
	Só não consegui aplicar os comandos com o CAT etc no linux


====================================================================================6 SPI
ex6dot1______________SPI Loopback
Exercise description:
	Program implementing a SPI Loopback in wich the STM32's MISO is directly conected to the MOSI.
Etc:
	-
ex6dot2______________Write and Test an EEPROM Module
Exercise description:
	Program to write, read etc from an EEPROM Module using SPI
Etc:
	-


====================================================================================7 SPI : LCD Display
ex7dot1______________Complete Interface Code
Exercise description:
	-
Etc:
	!!!!!!!!!explicar a situação de que o controlador do LCD do arduino afinal nao é o 7735R? mas sim o ILI9163C?+ explicar como 		escolher um ou outro.
ex7dot2______________Display Text
Exercise description:
	Extend your routine to support writing lines of text to the screen -
	consider how you might handle wrap.
Etc:
	Try to adapt adafruit cpp code (dont reinvent the wheel)

_ex7dot3______________Graphics
Exercise description:
	-
Etc:
	-


====================================================================================8 SD Memory Cards
_ex8dot1______________FAT File System
Exercise description:
	-
Etc:
	-


====================================================================================9 I2C – Wii Nunchuk
_ex9dot1______________Reading Wii Nunchuk
Exercise description:
	- 
Etc:
	see exaux9dot1 as starting point 
exaux9dot1______________
Exercise description:
	Code that gets values from nunchuck and print them on PC via USB/UART bridge
Etc:
	Try the other nunchuck


====================================================================================10 Timers
ex10dot1______________Ramping LED
exaux10dot1______________
Exercise description:
	-
Etc:
	-
_ex10dot2______________Hobby Servo Control
Exercise description:
	-
Etc:
	-
ex10dot3______________Ultrasonic Sensor
Exercise description:
	-
Etc:
	-


====================================================================================11 Interrupts
ex11dot1______________Timer Interrupt – Blinking LED
Exercise description:
	-
Etc:
	-
_ex11dot2______________Interrupt Driven Serial Communciations
Exercise description:
	-
Etc:
	-
_ex11dot3______________External Interrupt
Exercise description:
	-
Etc:
	-


====================================================================================12 DMA: Direct Memory Access
_ex12dot1______________SPI DMA module
Exercise description:
	-
Etc:
	-
_ex12dot2______________Display BMP Images from Fat File System
Exercise description:
	-
Etc:
	-


====================================================================================13 DAC : Digital Analog Converter
_ex13dot1______________Waveform Generator
Exercise description:
	-
Etc:
	-
_ex13dot2______________Application Software Driven Conversion
Exercise description:
	-
Etc:
	-
_ex13dot3______________Interrupt Driven Conversion
Exercise description:
	-
Etc:
	-
_ex13dot4______________Audio Player
Exercise description:
	-
Etc:
	-


====================================================================================14 ADC : Analog Digital Converter
_ex14dot1______________Continuous Sampling
Exercise description:
	-
Etc:
	-
_ex14dot2______________Timer Driven Conversion
Exercise description:
	-
Etc:
	-
_ex14dot3______________Voice Recorder
Exercise description:
	-
Etc:
	-


====================================================================================15 NewLib
_ex15dot1______________Hello World
Exercise description:
	-
Etc:
	-


====================================================================================16 Real-Time Operating Systems
_ex16dot1______________RTOS – Blinking Lights
Exercise description:
	-
Etc:
	-
_ex16dot2______________Multiple Threads
Exercise description:
	-
Etc:
	-
_ex16dot3______________Multithreaded Queues
Exercise description:
	-
Etc:
	-
_ex16dot4______________Multithreaded SPI
Exercise description:
	-
Etc:
	-
_ex16dot5______________Multithreaded FatFS
Exercise description:
	-
Etc:
	-


====================================================================================17 Next Steps




|||||||||||||||||||||||||||||||||||||List of created libraries:|||||||||||||||||||||||||||||||||||||



|||||||||||||||||||||||||||||||||||||Manual/main comands:|||||||||||||||||||||||||||||||||||||



|||||||||||||||||||||||||||||||||||||Etc:|||||||||||||||||||||||||||||||||||||
Exercice status: Untouched, WIP, Working, Completed.



|||||||||||||||||||||||||||||||||||||Task:|||||||||||||||||||||||||||||||||||||
Por este ficheiro em formato markdown.



|||||||||||||||||||||||||||||||||||||Learn:|||||||||||||||||||||||||||||||||||||
https://en.wikipedia.org/wiki/Make_(software)#Makefile
https://en.wikipedia.org/wiki/Makefile



