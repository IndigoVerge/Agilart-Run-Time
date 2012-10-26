# Agilart
Agilart makes it easy to controll simple devices that you connect to a microcontroller. To get started, fork this repository and continue reading to learn how you can change it to suit your needs.

[See it in action](http://www.agilart.com/community-and-support/video-tutorials/agilart-getting-started)

## Background

After speniding an enormous amount of time on trying to set up a small DIY Home Automation system I thought that it should deffinitely be easier to define simple dependencies between the devices at home. Then we decide to make our own platform [Agilart](https://lockitron.com) that would not only help with Home Automation but would make all sorts of art project possible using all kind of electronics and sensors that you can think of. We've been absolutely blown away by the response. As a first step in what will hopefully be a long history of giving back, we have decided to open source and grow the platform even more. Please send questions, comments, or concerns to [team@agilart.com](mailto:team@agilart.com)!

Agilart is a starting point. We continue working on it and really want to see it grow on it's own. Here are the core elements:

* Devices - We use device widgets to represent real devices that you want to connect together and control. The device widgets are represented by visual blocks that you can drag and drop on a canvas in the Visual Designer. We have hardware and software related devices
* Drivers - For each of the software related devices you need to have a driver. This driver would be your connection with the low level peripherals of the micocontroller that you'll use to connect and later control the specific device.
* AgilartRT - The Runtime that runs on the microcontroller and makes sure that the programs created with the Visual Designer run as expected. 

You also have a Visual Designer that we started to give people with not technical background a way to easily work with microcontrollers, sensors and simple devices. Have a look at the Visual Designer in the [video](http://www.agilart.com/community-and-support/video-tutorials/agilart-getting-started).

## Getting Started

First you'll need to fork and clone this repo

```bash
git clone https://github.com/Agilart/Agilart-Run-Time.git
```

### Customizing

Once you have the code, you can start customizing it to fit your own case. Let's say that I have an LED and a push-down button and I want to implement the most basic scenario where I want the LED to turn on when you press the button.
In the Agilart Visual designer we will have one GPIO Input Device connected to one GPIO Output Device. But let's dig into what happens in the Agilart run time and how we can customize it. Both of these widgets are hardware related. The Digital Output Device will be the LED and the Digital Input Device will be the Button. 
You can see how they are implemented if you go to: 'Devices/GPInputDevice' and 'Devices/GPOutputDevice'. You'll see that they both implement the 'BaseDevice' interface. 
If you want to create a new device all you need to do would be to implement the same 'BaseDevice' class. Each device has a number of connection points that allow it to interact with other devices.The hardware devices will also have 0 or more pinouts. thi pinouts are the hardware pins that would allow your device to interact with its driver. 
For example the GPInput and Output devices both have one pinout that is the actual pin to which they are attached on the microcontroller.

You can see the implementation of the 'Common/BaseDevice' - there you'll see that the whole device has 
* a collection of Connection Points
* a collection of Pinouts
* Some Logic
* a collection of properties
The people would have the chance to set these properties from the side menu in the visual designer. So if you need them you can add as many of them as you want in this collection, when you initialize your device.

So for our devices we'll also need drivers that would controll the GPIOs on the microcontroller where the LED and the Button are connected. You can see them if you go to 'Drivers/RaspiGPIO/'.The both implement a 'BasicDriver' interface.

The communication between the Devices and the Drivers happens in the Agilart Runtime. There we match devices and driver together and define the communication protocol. 
* the Devices send Commands to Drivers and 
* the Drivers raise events that reach the specific device that needs to be notified

You can extend the system by addining additional devices and drivers that suit your case.

### Deploying to Production

All you need to run the platform on your microcontroller would be the folling:

```bash
./agilart-runtime <IP> <Port Number>
```
