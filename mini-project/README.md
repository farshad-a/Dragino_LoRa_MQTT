# MessagePack Encoded LoRa Packets

![alt text](https://raw.githubusercontent.com/fash-aryan/EEET2371-WSNs/master/mini-project/figures/mini-project.jpg?token=AID7VE2Y5P4O2CCPGHMJJA25U3HOS)
Provide an introductory paragraph, describing:

* MessagePack or ASCII encoded JSON measurement data.
* Dockerized InfuxDB, Mosquitto MQTT Broker, Node-RED, and Grafana.
* InfluxDB as the time series database.
* Grafana as the visualization platform.
* Mosquitto MQTT Broker to subscribe and publish LoRa Node measurement and actuation data.
* Node-RED as the flow programming language for creating visual flows.

## Table of Contents

1. [About the Project](#about-the-project)
1. [Project Status](#project-status)
1. [Getting Started](#getting-started)
    1. [Docker Images and Containers](#docker-images-and-containers)
    1. [Node-RED Flows](#node-red-flows)
    1. [InfluxDB Database](#influxdb-database)
    1. [Grafana Visualization](#grafana-virtualization)
1. [Further Reading](#further-reading)
1. [Acknowledgements](#acknowledgements)

# About the Project

In this project, the DHT11 temperature and humidity sensor data is collected and sent in JSON format to LG01 LoRa gateway. The JSON Format can be encoded as ASCII or MessagePack.

* The [ArduinoJson](https://arduinojson.org/) library is used for de/serializing JSON.
* ArduinoJson library includes a MessagePack de/serializer.
* Visit [msgpack.org](https://msgpack.org) to learn more.

```
serializeJson(doc, buf_send, RH_RF95_MAX_MESSAGE_LEN);          //ASCII Encoded JSON buf_send
serializeMsgPack(doc, buf_send, RH_RF95_MAX_MESSAGE_LEN);    //MessagePack Encoded JSON buf_send
```

# Project Status

The provided code should not be used in production and can only be used for proof-of-concept validation.

# Getting Started

### Prerequisites
Before starting this project:
- [Docker](https://www.docker.com/) has to be up and running on your device.
- Installing [Dragino](http://www.dragino.com/downloads/downloads/UserManual/LG01_LoRa_Gateway_User_Manual.pdf) libraries in Arduino IDE.


## Docker Images and Containers

Run the following commands in your terminal to download the required images and then build the containers.

### Eclipse Mosquitto MQTT Broker


```
docker pull eclipse-mosquitto
docker run -it -p 1883:1883 --name <influxdb container name> eclipse-mosquitto
```

### Node-RED
```
docker pull nodered/node-red
docker run -it -p 1880:1880 --name <nodered container name> nodered/node-red
```

### InfluxDB
The '-v' option stores InfluxDB data into a docker volume in $PWD directory.
```
docker pull influxdb
docker run -p 8086:8086 --name <influxdb container name> -v $PWD:/var/lib/influxdb influxdb

```
$PWD is replaced by the current directory that your terminal run in. You can change this directory by replacing it from $PWD to your preferred directory.

### Grafana
```
docker pull grafana/grafana
docker run -d -p 3000:3000 --name=mygrafana grafana/grafana
```

## Node-RED Flows


## InfluxDB Database
| time          | id               |humidity            | temperature               |
|---------------|----------------- |--------------------|---------------------------|
| 1569988251974 |100               |26.00               |27.00                      |
| 1570594982609 |200               |24.00               |30.00                      |

## Grafana Visualization


## Dragino LoRa End Node - Arduino

## Dragino LG01 Gateway - Arduino

# Further Reading

# Acknowledgments

Provide proper credits, shoutouts, and honorable mentions here. Also provide links to relevant repositories, blog posts, or contributors worth mentioning.

Give proper credits. This could be a link to any repo which inspired you to build this project, any blogposts or links to people who contributed in this project. If you used external code, link to the original source.

**[Back to top](#table-of-contents)**