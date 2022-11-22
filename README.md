# Title: IOT based Data center monitoring

Name of Supervisors: 1) Dr. Suman Kundu
                     2) Mr. Jayanta Borthakur
                     
                     
 ## Abstract
Over the years, the Internet of Things (IoT) has grown rapidly. What’s the reason? The inclination towards maintaining the productivity of the equipment, manpower, and building operations. Whether it’s about managing utilities or staying compliant with health and safety regulations, users want to keep track of everything. It’s possible with remote monitoring with IoT systems. This cost-effective, smart, and advanced technology helps end-users streamline their operations.
Currently, temperature and humidity are important factors for the correct operation and security of electronic devices in a data center. In this project, we have used a combination of IoT and the cloud.


## Methodology
There’s a lot that goes into making IoT devices function as they should. All of the hardware, software, services, and applications need to work together seamlessly for an IoT system to be effective — and this requires a solid intentional IoT architecture.

![image](https://user-images.githubusercontent.com/56917291/203335498-f704cd00-30c7-4b53-8d3a-e7a1f4fefc83.png)


## Circuit Diagram

![image](https://user-images.githubusercontent.com/56917291/203335685-f4f4cfdc-485c-4800-80b4-724a4521e485.png)
![image](https://user-images.githubusercontent.com/56917291/203335755-c1fd6c80-9598-4069-b413-f90c4b2c1b40.png)


## Technical Specifications
We have used two sensors, DHT 11 and MQ2.
DHT11 measures the temperature and humidity, while MQ2 measures smoke in ppm.
DHT11 temperature and humidity sensors feature dedicated NTCs for measuring temperature and 8-bit microcontrollers for outputting temperature and humidity values as serial data.

![image](https://user-images.githubusercontent.com/56917291/203335895-a2cb9d3f-1647-4a7b-a9e9-964b233a558c.png)
![image](https://user-images.githubusercontent.com/56917291/203335932-2308aafe-6943-496c-abe7-5714165c202d.png)

Among the MQ sensor series, the MQ2 is widely used. This sensor utilizes Metal Oxide Semiconductor technology. Due to the fact that metal oxide sensors sense gas by changing resistance as the gas is introduced, they are also known as chemiresistors.![image](https://user-images.githubusercontent.com/56917291/203336036-b7c0a3fc-70fa-45ca-b23b-90be628b86be.png)

![image](https://user-images.githubusercontent.com/56917291/203336074-1feefb71-d3e7-41ee-94c0-d85990b75f6b.png)
![image](https://user-images.githubusercontent.com/56917291/203336363-0f66db93-7187-4af9-8af2-e94ebf8dacf2.png)


## Results and Discussion
Link to video for demo. https://drive.google.com/drive/folders/1glWUylDJorr5kYiLkfiyp9-oM9Mv94fc
This device measures the temperature, humidity, and smoke.
Admin can change the threshold of every parameter measured.
Admin has the ability to control the frequency/interval of transferring the data to the real-time database.
The data is sent over the network using wi-fi, and everything is updated in the database, as shown below.

![image](https://user-images.githubusercontent.com/56917291/203336454-2901d07f-76ff-4699-8d3d-9e4b64130083.png)


In the firmware code, it checks if the current measured value crosses the threshold value(specified by the Admin), then warning LEDs and Buzzer will be automatically turned on. Below is the image of the circuit


If, due to any reason sensor is not working (no proper connection, defective sensor, aged sensor, etc.), then the respective FAILURE LED(those LEDs which are meant for showing failure) will turn on, and the status will be reflected in the real-time database. So in our model, Green Light will light up if a sensor fails. For Two sensors, Two Green LEDs are present.
![image](https://user-images.githubusercontent.com/56917291/203336523-7efe32bc-7070-40f5-b721-38f5e5d9f7f7.png)

We can that data is coming from different sensors

![image](https://user-images.githubusercontent.com/56917291/203337069-169022c5-5608-41a0-8ed8-79d7506133c0.png)
![image](https://user-images.githubusercontent.com/56917291/203337109-e7437f22-bc27-4569-b890-46d64076185b.png)
![image](https://user-images.githubusercontent.com/56917291/203337136-1c677410-218a-4e6d-8668-c473104a89f3.png)

## References

https://www.circuito.io/

https://www.sunbirddcim.com/what-data-center-monitoring#:~:text=Data%20center%20monitoring%20enables%20you,protocols%20for%20IP%2Dbased%20networks.

https://www.youtube.com/watch?v=YN522_npNqs&ab_channel=BINARYUPDATES

https://www.dnsstuff.com/data-center-monitoring-systems

http://www.steves-internet-guide.com/mqtt/

https://www.instructables.com/IoT-ESP8266-Series-1-Connect-to-WIFI-Router/

https://www.researchgate.net/publication/360244057_A_review_on_data_center_monitoring_system_using_smart_sensor_network



