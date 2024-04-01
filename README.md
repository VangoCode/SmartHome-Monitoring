# SmartHome Monitoring with AWS on an STM32 Board

## Ron Varshavsky, Elsie (Muhan) Zhu

### Requirements

- An STM32L4 B-L475E-IOT01A board
- An AWS account
- A valid WiFi connection without additional connection requirements aside from username/password (ex; hotspot, home WiFi etc.)

### Technical Features

This project has a number of additional technical features, mostly to preserve power consumption.

- All routines in the board outside of the initial setup and monitoring are handled through interrupts. This conserves power by allowing the board to go into sleep/deep sleep for almost all of its running time
- Although not done through interrupts, Monitoring is done through the global Mbed Event Queue, allowing the program to again sleep when not in use, and only awaken once every 10 seconds to read from the sensors, conserving power consumption
- The message queue sends a keep alive message every 15 seconds through the Mbed Event Queue. This is because AWS automatically closes MQTT connections after 30 seconds of inactivity. This keeps the connection alive indefinitely, while reducing power consumption by allowing the board to sleep when not sending this message
- The board publishes and subscribes using the same MQTT connection; you only need to connect once and can then do everything!
- Messages are only read when you press the user button. This conserves power and acts as an additional feature, that the settings can only be changed with a physical push; a malicious actor cannot randomly send a message and remove monitoring

### Setup

### Project Flow

### Usage

To start the program, simply build the project and flash your board with the compiled program using Mbed studio. When the program first loads, it will attempt to connect to WiFi, then form an MQTT connection with AWS, and then subscribe to an AWS topic. If any part of this fails, please see the `Setup` section and make sure you are following the instructions correctly. It should look as follows;
![image](./readme-images/initialization.png)

When running the program, if you wish to change which sensors to monitor, you should go to your AWS IoT console, and send a message over MQTT to the topic set as `MQTT_TOPIC_SUB` in the project setup. It should be of the format `nnnn` where each `n` corresponds to a bit. Each bit refers to whether or not you want to monitor this sensor, in the order of `[humidity][temp][pressure][distance]`. For example, if you want to monitor everything except humidity, you would send the message below:
![image](./readme-images/message-payload.png)

Now, once you have sent the message it is important to note that your settings are NOT applied until you press the blue `user` button on your board. Once you press the button, there should be some feedback letting you know the message was read successfully:

![image](./readme-images/message-received.png)

Note that this feedback also means that your settings were loaded successfully. From now on, every sensor specified is monitored every 10 seconds, and if an abnormal value is detected, it will send a message over MQTT. If you have SMS message routing set up properly, as specified in the `Setup`, you should also now be getting texts any time there is an abnormal value.

You can also change which sensors to monitor in realtime; to do this, simply send another message over MQTT to your `MQTT_TOPIC_SUB` topic, and then press the blue `user` button again. It will cancel the previous monitoring, and start monitoring these new sensors!
