#include "DHT.h"
#define DHTPIN D1
#define DHTTYPE DHT11
// #include <MQ2.h>
#include <ESPAsyncWebServer.h>
#include <ESPAsyncTCP.h>
#include <Arduino.h>
#if defined(ESP32)
  #include <WiFi.h>
#elif defined(ESP8266)
  #include <ESP8266WiFi.h>
#endif
#include <Firebase_ESP_Client.h>

DHT dht(DHTPIN, DHTTYPE);
#define MQ2pin 0

float t = 0.0;
float tf = 0.0;
float h = 0.0;
float gs = 0.0;
// MQ2 mq2(A0);
//thresholds
float temperatureThreshold=28;
float humidityThreshold=80;
float smokeThreshold=0.05;
float temperatureThresholdForBuzzer=29;
float smokeThresholdForBuzzer=1000;
bool DHTsensorworking = false;
bool smokeSensorWorking = false;
unsigned long previousMillis = 0; 
long frequency = 1000; 
AsyncWebServer server(80);

//Provide the token generation process info.
#include "addons/TokenHelper.h"
//Provide the RTDB payload printing info and other helper functions.
#include "addons/RTDBHelper.h"

// Insert your network credentials
#define WIFI_SSID "ROG"
#define WIFI_PASSWORD "12345678"

// Insert Firebase project API Key
#define API_KEY "AIzaSyB7e6KxtDMZw5cgIQf9CselofOcSPfQn6o"

// Insert RTDB URLefine the RTDB URL */
#define DATABASE_URL "iitj-data-center-iot-default-rtdb.firebaseio.com/"

//Defined Firebase Data object
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;
bool signupOK = false;

// //web page
const char index_html[] PROGMEM = R"webpage(
<!DOCTYPE HTML>
<html>

<head>
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <script src="https://code.highcharts.com/8.0/highcharts.js"></script>
    <style>
        body {
            min-width: 300px;
            max-width: 800px;
            height: 400px;
            margin: 0 auto;
        }

        h2 {
            font-family: Arial;
            font-size: 2.5rem;
            text-align: center;
        }
    </style>
</head>

<body>
    <h2>Data Center</h2>
    <div id="temperature-chart" class="container"></div>
    <div id="fahrenheit-chart" class="container"></div>
    <div id="humidity-chart" class="container"></div>
    <div id="smoke-chart" class="container"></div>
</body>
<script>
    var chartT = new Highcharts.Chart({
        chart: { renderTo: 'temperature-chart' },
        title: { text: 'Temperature in Degree Celsius' },
        series: [{
            showInLegend: false,
            data: []
        }],
        plotOptions: {
            line: {
                animation: false,
                dataLabels: { enabled: true }
            },
            series: { color: '#059e8a' }
        },
        xAxis: {
            type: 'datetime',
            dateTimeLabelFormats: { second: '%H:%M:%S' }
        },
        yAxis: {
            title: { text: 'Temperature (Celsius)' }
        },
        credits: { enabled: false }
    });
    setInterval(function () {
        var xhttp = new XMLHttpRequest();
        xhttp.onreadystatechange = function () {
            if (this.readyState == 4 && this.status == 200) {
                var x = (new Date()).getTime(),
                    y = parseFloat(this.responseText);
                if (chartT.series[0].data.length > 50) {
                    chartT.series[0].addPoint([x, y], true, true, true);
                } else {
                    chartT.series[0].addPoint([x, y], true, false, true);
                }
            }
        };
        xhttp.open("GET", "/temperature", true);
        xhttp.send();
    }, 1000);

    var chartF = new Highcharts.Chart({
        chart: { renderTo: 'fahrenheit-chart' },
        title: { text: 'Temperature in Fahrenheit' },
        series: [{
            showInLegend: false,
            data: []
        }],
        plotOptions: {
            line: {
                animation: false,
                dataLabels: { enabled: true }
            }
        },
        xAxis: {
            type: 'datetime',
            dateTimeLabelFormats: { second: '%H:%M:%S' }
        },
        yAxis: {
            title: { text: 'fahrenheit (F)' }
        },
        credits: { enabled: false }
    });
    setInterval(function () {
        var xhttp = new XMLHttpRequest();
        xhttp.onreadystatechange = function () {
            if (this.readyState == 4 && this.status == 200) {
                var x = (new Date()).getTime(),
                    y = parseFloat(this.responseText);
                if (chartF.series[0].data.length > 50) {
                    chartF.series[0].addPoint([x, y], true, true, true);
                } else {
                    chartF.series[0].addPoint([x, y], true, false, true);
                }
            }
        };
        xhttp.open("GET", "/fahrenheit", true);
        xhttp.send();
    }, 1000);

    var chartH = new Highcharts.Chart({
        chart: { renderTo: 'humidity-chart' },
        title: { text: 'Humidity (%)' },
        series: [{
            showInLegend: false,
            data: []
        }],
        plotOptions: {
            line: {
                animation: false,
                dataLabels: { enabled: true }
            },
            series: { color: '#18009c' }
        },
        xAxis: {
            type: 'datetime',
            dateTimeLabelFormats: { second: '%H:%M:%S' }
        },
        yAxis: {
            title: { text: 'Humidity (%)' }
        },
        credits: { enabled: false }
    });
    setInterval(function () {
        var xhttp = new XMLHttpRequest();
        xhttp.onreadystatechange = function () {
            if (this.readyState == 4 && this.status == 200) {
                var x = (new Date()).getTime(),
                    y = parseFloat(this.responseText);
                if (chartH.series[0].data.length > 50) {
                    chartH.series[0].addPoint([x, y], true, true, true);
                } else {
                    chartH.series[0].addPoint([x, y], true, false, true);
                }
            }
        };
        xhttp.open("GET", "/humidity", true);
        xhttp.send();
    }, 1000);

    var chartS = new Highcharts.Chart({
        chart: { renderTo: 'smoke-chart' },
        title: { text: 'SMOKE-CO' },
        series: [{
            showInLegend: false,
            data: []
        }],
        plotOptions: {
            line: {
                animation: false,
                dataLabels: { enabled: true }
            },
            series: { color: '#18009c' }
        },
        xAxis: {
            type: 'datetime',
            dateTimeLabelFormats: { second: '%H:%M:%S' }
        },
        yAxis: {
            title: { text: 'Smoke' }
        },
        credits: { enabled: false }
    });
    setInterval(function () {
        var xhttp = new XMLHttpRequest();
        xhttp.onreadystatechange = function () {
            if (this.readyState == 4 && this.status == 200) {
                var x = (new Date()).getTime(),
                    y = parseFloat(this.responseText);
                if (chartS.series[0].data.length > 50) {
                    chartS.series[0].addPoint([x, y], true, true, true);
                } else {
                    chartS.series[0].addPoint([x, y], true, false, true);
                }
            }
        };
        xhttp.open("GET", "/smoke", true);
        xhttp.send();
    }, 1000);
</script>

</html>
)webpage";




void setup(){

  pinMode(A0, INPUT); //LDR
  pinMode(DHTPIN, INPUT); //DHT Sensor
  pinMode(D3, OUTPUT);  // DHT sensor working
  pinMode(D4, OUTPUT);  //Gas sensor working
  pinMode(D0, OUTPUT);  //RED LED
  pinMode(D6, OUTPUT);  //BLUE LED
  pinMode(D7, OUTPUT);  //YELLOW LED
  pinMode(D8, OUTPUT);  // buzzer on this
  
  dht.begin();
  // mq2.begin(); 
  Serial.begin(115200);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED){
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();
  config.api_key = API_KEY;
  config.database_url = DATABASE_URL;

  /* Sign up */
  if (Firebase.signUp(&config, &auth, "", "")){
    Serial.println("ok");
    signupOK = true;
  }
  else{
    Serial.printf("%s\n", config.signer.signupError.message.c_str());
  }

  config.token_status_callback = tokenStatusCallback; 
  
  Firebase.begin(&config, &auth);
  // FirebaseObject object=Firebase.get("/");
  Firebase.reconnectWiFi(true);
    // Route for root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html);
  });
  server.on("/temperature", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", String(t).c_str());
  });
  server.on("/fahrenheit", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", String(tf).c_str());
  });
    
  server.on("/humidity", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", String(h).c_str());
  });

  server.on("/smoke", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", String(gs).c_str());
  });

  // Start server
  server.begin();
}

void loop(){
  float currentH = dht.readHumidity();
  float currentTf = dht.readTemperature(true);
  float currentT = dht.readTemperature();
  float smoke = analogRead(MQ2pin);
  
  if (Firebase.ready() && signupOK ) {
    if(Firebase.RTDB.getInt(&fbdo, "frequency",&frequency)){
      Serial.print("frequency : " );
      Serial.println(frequency);
    }
    if(Firebase.RTDB.getFloat(&fbdo, "threshold/temperature",&temperatureThreshold)){
      Serial.print("temperatureThreshold : " );
      Serial.println(temperatureThreshold);
    }
    if(Firebase.RTDB.getFloat(&fbdo, "threshold/humidity", &humidityThreshold)){
      Serial.print("humidityThreshold : " );
      Serial.println(humidityThreshold);
    }
    if(Firebase.RTDB.getFloat(&fbdo, "threshold/smoke",&smokeThreshold)){
      Serial.print("smokeThreshold : " );
      Serial.println(smokeThreshold);
    }
    if(Firebase.RTDB.getFloat(&fbdo, "threshold/temperatureThresholdForBuzzer",&temperatureThresholdForBuzzer)){
      Serial.print("temperatureThresholdForBuzzer : " );
      Serial.println(temperatureThresholdForBuzzer);
    }
    if(Firebase.RTDB.getFloat(&fbdo, "threshold/smokeThresholdForBuzzer",&smokeThresholdForBuzzer)){
      Serial.print("smokeThresholdForBuzzer : " );
      Serial.println(smokeThresholdForBuzzer);
    }
  }
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= frequency) {
    previousMillis = currentMillis;
      if (isnan(currentT)) {
        DHTsensorworking=false;
        Serial.println("Failed to read from DHT sensor!");
        currentT=t;
      }else {
        t = currentT;
        DHTsensorworking=true;
      }

      if (isnan(currentTf)) {
        DHTsensorworking=false;
        Serial.println("Failed to read from DHT sensor!");
        currentTf=tf;
      }else {
        tf = currentTf;
        DHTsensorworking=true;
      }

      if (isnan(currentH)) {
        DHTsensorworking=false;
        Serial.println("Failed to read from DHT sensor!");
        currentH=h;
      }else {
        h = currentH;
        DHTsensorworking=true;
      }

      if (smoke<=10) {
        smokeSensorWorking=false;
        Serial.println("Failed to read from Gas sensor!");
        smoke=gs;
      }else {
        gs = smoke;
        smokeSensorWorking=true;
      }


    if (Firebase.ready() && signupOK ) {
      if (Firebase.RTDB.setFloat(&fbdo, "Sensors/DHT/humidity",currentH)){
        Serial.print("Humidity: ");
        Serial.println(currentH);
        if (DHTsensorworking && Firebase.RTDB.setFloat(&fbdo, "sensorWorking/DHT",1)){

          
        }else if (Firebase.RTDB.setFloat(&fbdo, "sensorWorking/DHT", 0)){
          
        }
      }
      
      if (Firebase.RTDB.setFloat(&fbdo, "Sensors/DHT/temperature", currentT)){
        Serial.print("Temperature: ");
        Serial.println(currentT);
      }

      if (Firebase.RTDB.setFloat(&fbdo, "Sensors/MQ2/smoke", smoke)){
        Serial.print("smoke: ");
        Serial.println(smoke);
        if (smokeSensorWorking && Firebase.RTDB.setFloat(&fbdo, "sensorWorking/MQ2", 1)){

          
        }else if(Firebase.RTDB.setFloat(&fbdo, "sensorWorking/MQ2", 0)){

          
        }
      }

    }
    Serial.println("______________________________");

  } 
  //checks
  if(DHTsensorworking==true){
    //dht sensors working
    digitalWrite(D3, LOW);
    if(t>=temperatureThreshold){
      digitalWrite(D7, HIGH);
    }else{
      digitalWrite(D7, LOW); 
    }
    if(t>=temperatureThresholdForBuzzer){
      digitalWrite(D8, HIGH); 
    }else{
      digitalWrite(D8, LOW); 
    }
    if(h>=humidityThreshold){
      digitalWrite(D6, HIGH);
    }else{
      digitalWrite(D6, LOW); 
    }
  }else{
    //dht sensors not working
      digitalWrite(D3, HIGH);
  }
  
  if(smokeSensorWorking==true){
    digitalWrite(D4, LOW);
    if(gs>=smokeThreshold){
      digitalWrite(D0, HIGH);
    }else{
      digitalWrite(D0, LOW); 
    }
    if(gs>=smokeThresholdForBuzzer){
      digitalWrite(D8, HIGH); 
    }else{
      digitalWrite(D8, LOW); 
    }
  }else{
    digitalWrite(D4, HIGH);
  }

}
