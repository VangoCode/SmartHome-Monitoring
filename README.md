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
#### Create a Thing in AWS IoT
For onboarding your discovery board to AWS IoT, you must first create an AWS account and sign in to the AWS console. Navigate to the `IoT Core` service using the search bar on the top navigation bar. Then, on the side bar, select `Manage > All devices > Things`. Here, you will register your discovery board as a *Thing*!

Choose `Create single thing`. Then, you will be prompted to specify properties for this Thing. Enter a name for your Thing. It should look like this: 

![image](./readme-images/thing-properties.png)

Next, you will be prompted to configure a certificate for your Thing, so that your board can connect to AWS IoT. Select `Auto-generate a new certificate`.

Finally, you will be prompted to attach policies to this certificate, which allow you to specify which AWS IoT resources your device has access to. Click on the `Create policy` button, which will take you to a new tab. Here, you can create the policy. Give it a name, and a policy statement. In the policy statement section, select the `JSON` mode, and copy-and-paste this into it:

```
{
  "Version": "2012-10-17",
  "Statement": [
    {
      "Effect": "Allow",
      "Action": "iot:Connect",
      "Resource": "*"
    },
    {
      "Effect": "Allow",
      "Action": "iot:Publish",
      "Resource": "*"
    },
    {
      "Effect": "Allow",
      "Action": "iot:Subscribe",
      "Resource": "*"
    },
    {
      "Effect": "Allow",
      "Action": "iot:Receive",
      "Resource": "*"
    }
  ]
}
```
Your page should look like this: 

![image](./readme-images/policy-statement.png)

Now, click the `Create` button. Your page should look like this now that you have successfully created a policy.
Next, navigate back to the Thing creation tab. You should now see your newly created policy: 

![image](./readme-images/select-policy.png)

Select your policy, and click the `Create thing` button. 

Then, a modal will pop up and it will prompt you to download the public and private key files for your certificate, and the Amazon root CA certificate. You **must** download the public and private key files, as this will be the **only** time you will be able to do so. 

![image](./readme-images/certificates-modal.png)

Now, take the certificate files you have downloaded, and insert their contents into `stm32l475_vl53l0x/cloud/CONSTANTS.h`. Insert the Amazon Root CA 1 certificate like so:
```
const char* SSL_CA_PEM = "-----BEGIN CERTIFICATE-----\n"
"INSERT HERE\n"
"-----END CERTIFICATE-----\n";
```
Insert your Certificate contents like this:
```
const char* SSL_CLIENT_CERT_PEM = "-----BEGIN CERTIFICATE-----\n"
"INSERT HERE\n"
"-----END CERTIFICATE-----\n";
```
Insert your private key contents like this:
```
const char* SSL_CLIENT_PRIVATE_KEY_PEM = "-----BEGIN RSA PRIVATE KEY-----\n"
"INSERT HERE\n"
"-----END RSA PRIVATE KEY-----\n";
```
To find your device's endpoint (in our case, we will send MQTT messages here), go to the AWS IoT Settings like this:

![image](./readme-images/device-endpoint.png)

Insert this endpoint into the `CONSTANTS.h` file like this:
```
const char MQTT_SERVER_HOST_NAME[] = "INSERT ENDPOINT HERE";
```
#### Set Up Message Routing to AWS SNS
Now, we will be routing the MQTT messages from AWS IoT to AWS SNS, which you can use to receive text messages. 
On the AWS IoT console, go to `Message Routing > Rules` and click on the `Create rule` button. Give your rule a name, and to configure a SQL statement to filter received MQTT messages, type in this with your MQTT publishing topic name:
```
SELECT * FROM 'your-pub-topic'
```
Next, attach an action to your rule. This is where you want the selected MQTT messages to be routed to. In this case, select `Simple Notification Service (SNS)`. Now, you must create an SNS topic by clicking the `Create SNS topic` button, which will take you to a new tab. Enter the details like this: ADD IMAGE HERE
and select `Create topic`. You will be able to see your newly created topic. Now, you must create a subscription to this topic using the `Create subscription` button. Select the `SMS` protocol, and register and add your phone number. Click the `Subscribe` button. 

Navigate back to the IoT rule tab. Choose the SNS topic that you created, and create a new IAM role for AWS IoT to publish to that topic like this:
ADD PICTURE HERE
Once you confirm everything and create the rule, your page should look like this:
ADD IMAGE

Woohoo! You have completed the set-up!

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
