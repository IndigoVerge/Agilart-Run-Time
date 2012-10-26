# Agilart
Agilart makes it easy to controll simple devices that you connect to a microcontroller. To get started, fork this repository and continue reading to learn how you can change it to suit your needs.

[See it in action](http://www.agilart.com/community-and-support/video-tutorials/agilart-getting-started)

## Background

After speniding an enormous amount of time on trying to set up a small DIY Home Automation system I thought that it should deffinitely be easier to define simple dependencies between the devices at home. Then we decide to make our own platform [Agilart](https://lockitron.com) that would not only help with Home Automation but would make all sorts of art project possible using all kind of electronics and sensors that you can think of. We've been absolutely blown away by the response. As a first step in what will hopefully be a long history of giving back, we have decided to open source and grow the platform even more. Please send questions, comments, or concerns to [team@agilart.com](mailto:team@agilart.com)!

Agilart is a starting point. We continue working on it and really want to see it grow on it's own. Here are the core elements:

* Devices
We use device widgets to represent real devices that you want to connect together and control. The device widgets are represented by visual blocks that you can drag and drop on a canvas in the Visual Designer. We have hardware and software related devices
* Drivers
For each of the software related devices you need to have a driver. This driver would be your connection with the low level peripherals of the micocontroller that you'll use to connect and later control the specific device.
* AgilartRT
The Runtime that runs on the microcontroller and makes sure that the programs created with the Visual Designer run as expected. 

## Getting Started

First you'll need to fork and clone this repo

```bash
git clone https://github.com/Agilart/Agilart-Run-Time.git
```

### Customizing

### Deploying to Production

All you need to run the platform on your microcontroller would be the folling:

```bash
./agilart-runtime <IP> <Port Number>
```
