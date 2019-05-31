#include <AZ3166WiFi.h>
#include "MQTTClient.h"
#include "MQTTNetwork.h"
#include "Telemetry.h"
#include <Arduino_JSON.h>
#include <OledDisplay.h>

int status = WL_IDLE_STATUS;
int arrivedcount = 0;
bool hasWifi = false;
bool isConnected = false;

int butA = 4;
int butB = 10;

int butAState;             // the current reading from the input pin
int butALastState = LOW;   // the previous reading from the input pin

int butBState;             // the current reading from the input pin
int butBLastState = LOW;   // the previous reading from the input pin

// the following variables are unsigned longs because the time, measured in
// milliseconds, will quickly become a bigger number than can be stored in an int.
unsigned long butALastDebounceTime = 0;  // the last time the output pin was toggled
unsigned long butBLastDebounceTime = 0;  // the last time the output pin was toggled

unsigned long debounceDelay = 50;

const char* mqttServer = "m10.cloudmqtt.com"; //iot.eclipse.org";   //"m2m.eclipse.org";
int port = 19839; //1883;

MQTTNetwork mqttNetwork;
MQTT::Client<MQTTNetwork, Countdown> client = MQTT::Client<MQTTNetwork, Countdown>(mqttNetwork);

void initWifi()
{
  Screen.print("IoT DevKit\r\n \r\nConnecting...\r\n");

  if (WiFi.begin() == WL_CONNECTED)
  {
    IPAddress ip = WiFi.localIP();
    Screen.print(1, ip.get_address());
    hasWifi = true;
    Screen.print(2, "Running... \r\n");
  }
  else
  {
    Screen.print(1, "No Wi-Fi\r\n ");
  }
}

void splash(){
  unsigned char BMP[] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,128,192,192,224,240,56,12,192,240,224,192,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,128,224,224,224,224,128,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,192,224,248,252,254,255,255,63,15,3,64,248,254,255,255,255,255,255,252,248,224,128,0,0,0,0,0,0,0,0,0,0,0,0,0,0,128,240,252,127,31,3,3,15,127,254,240,192,0,0,0,0,8,28,28,28,28,156,220,252,124,60,28,0,0,0,252,252,248,0,0,0,0,0,0,252,252,248,0,0,0,0,252,252,252,112,56,28,28,28,0,128,224,240,248,28,28,12,12,28,248,248,240,128,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,128,224,248,252,255,255,255,255,63,15,3,0,0,0,0,0,0,3,7,15,31,63,127,255,255,255,255,255,252,240,224,128,0,0,0,0,0,0,64,112,124,127,31,7,7,7,7,7,7,7,7,31,127,126,120,64,0,96,112,120,126,127,103,99,97,96,96,96,0,0,0,63,127,127,240,224,224,96,96,56,127,127,127,0,0,0,0,127,127,127,0,0,0,0,0,0,15,63,127,123,243,227,227,227,227,99,99,35,3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,0,8,8,8,8,8,12,12,12,12,12,14,14,14,14,14,15,15,15,15,15,15,15,15,15,15,12,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
  // draw a bitmap to the screen
    Screen.draw(0, 0, 128, 8, BMP);
    delay(1000);
    // clean screen ready for next loop
    Screen.clean();
}

void messageArrived(MQTT::MessageData& md)
{
    MQTT::Message &message = md.message;

    char msgInfo[60];
    sprintf(msgInfo, "Message arrived: qos %d, retained %d, dup %d, packetid %d", message.qos, message.retained, message.dup, message.id);
    Serial.println(msgInfo);

    sprintf(msgInfo, "Payload: %s", (char*)message.payload);
    Serial.println(msgInfo);
    ++arrivedcount;
}

void mqttConnect(){
  
  char msgBuf[100];
  sprintf(msgBuf, "Connecting to MQTT server %s:%d", mqttServer, port);
  Serial.println(msgBuf);

  int rc = mqttNetwork.connect(mqttServer, port);
  if (rc != 0) {
    Serial.println("Connected to MQTT server failed");
  } else {
    Serial.println("Connected to MQTT server successfully");
    isConnected=true;
  }

  MQTTPacket_connectData data = MQTTPacket_connectData_initializer;
  data.MQTTVersion = 3;
  data.clientID.cstring = "ambrosio";
  data.username.cstring = "tag";
  data.password.cstring = "tag";

  if ((rc = client.connect(data)) != 0) {
      Serial.println("MQTT client connect to server failed");
  } else {
    Serial.println("MQTT client connected to server successfully");
  }
  
}

void msgScreen(MQTT::MessageData& md)     //{"text":"asdasdas","iswrap":true}
{
    MQTT::Message &message = md.message;

    char msgInfo[70];
    sprintf(msgInfo, "SCREEN Message arrived: qos %d, retained %d, dup %d, packetid %d", message.qos, message.retained, message.dup, message.id);
    Serial.println(msgInfo);

    JSONVar msg = JSON.parse((char*)message.payload);

    int line = 0;
    char buf[100];
    bool isWrap = false;

    if (msg.hasOwnProperty("line")) 
      line = msg["line"];

    if (msg.hasOwnProperty("text")) 
      sprintf(buf, msg["text"]);
      //buf = msg["text"];

    if (msg.hasOwnProperty("iswrap")) 
      isWrap = msg["iswrap"];
    Screen.clean();
    Screen.print(line,buf,isWrap);
}

void msgScreenLines(MQTT::MessageData& md)     //{"L1":"aaa","L2":"bbb","L3":"ccc","L4":"ddd"}
{
    MQTT::Message &message = md.message;

    char msgInfo[70];
    sprintf(msgInfo, "SCREEN LINES Message arrived: qos %d, retained %d, dup %d, packetid %d", message.qos, message.retained, message.dup, message.id);
    Serial.println(msgInfo);

    JSONVar msg = JSON.parse((char*)message.payload);

    int line = 0;
    char buf[100];
    bool isWrap = false;

    Screen.clean();
  
    if (msg.hasOwnProperty("L1")) {
      sprintf(buf, msg["L1"]);
      Screen.print(0,buf,false);
    }
    if (msg.hasOwnProperty("L2")) {
      sprintf(buf, msg["L2"]);
      Screen.print(1,buf,false);
    }
    if (msg.hasOwnProperty("L3")) {
      sprintf(buf, msg["L3"]);
      Screen.print(2,buf,false);
    }
    if (msg.hasOwnProperty("L4")) {
      sprintf(buf, msg["L4"]);
      Screen.print(3,buf,false);
    }
}


void subcribeScreen(){

  char* topic = "/tag/screen";
  int rc = client.subscribe(topic, MQTT::QOS2, msgScreen);
  
  if ( rc != 0) {
      Serial.println("MQTT client subscribe from server failed");
  }else {
    Serial.println("MQTT client subscribed from server successfully");
  }
}

void subcribeScreenLines(){

  char* topic = "/tag/screenlines";
  int rc = client.subscribe(topic, MQTT::QOS2, msgScreenLines);
  
  if ( rc != 0) {
      Serial.println("MQTT client subscribe LINES from server failed");
  }else {
    Serial.println("MQTT client subscribed LINES from server successfully");
  }
}

int runMqttExample() {
  char* topic = "/tag/mqtt-sample";

  int rc = client.subscribe(topic, MQTT::QOS2, messageArrived);
  
  if ( rc != 0) {
      Serial.println("MQTT client subscribe from server failed");
  }
  
  MQTT::Message message;

  // QoS 0
  char buf[100];
  sprintf(buf, "QoS 0 message from AZ3166!");
  message.qos = MQTT::QOS0;
  message.retained = false;
  message.dup = false;
  message.payload = (void*)buf;
  message.payloadlen = strlen(buf)+1;
  rc = client.publish(topic, message);
  while (arrivedcount < 1) {
      client.yield(100);
  }
 
  // QoS 1
  sprintf(buf, "QoS 1 message from AZ3166!");
  message.qos = MQTT::QOS1;
  message.payloadlen = strlen(buf)+1;
  rc = client.publish(topic, message);
  
  while (arrivedcount < 2) {
      client.yield(100);
  }
  
  if ((rc = client.unsubscribe(topic)) != 0) {
      Serial.println("MQTT client unsubscribe from server failed");
  }
  
  if ((rc = client.disconnect()) != 0) {
      Serial.println("MQTT client disconnect from server failed");
  }
  
  mqttNetwork.disconnect();
  Serial.print("Finish message count: ");
  Serial.println(arrivedcount);
      
  return 0;
}

void sendButA(){
  MQTT::Message message;
  char* topic = "/tag/button";
  // QoS 0
  char buf[10];
  sprintf(buf, "A");
  message.qos = MQTT::QOS0;
  message.retained = false;
  message.dup = false;
  message.payload = (void*)buf;
  message.payloadlen = strlen(buf);
  client.publish(topic, message);
}

void sendButB(){
  MQTT::Message message;
  char* topic = "/tag/button";
  // QoS 0
  char buf[10];
  sprintf(buf, "B");
  message.qos = MQTT::QOS0;
  message.retained = false;
  message.dup = false;
  message.payload = (void*)buf;
  message.payloadlen = strlen(buf);
  client.publish(topic, message);
}

void setup() {
  //Initialize serial and Wi-Fi and connect:
  Serial.begin(115200);
  initWifi();

  pinMode(butA, INPUT);  
  pinMode(butB, INPUT);  
  
  Screen.init();
  splash();
  mqttConnect();
  subcribeScreen();
  subcribeScreenLines();
  
  

  Serial.println("\r\n>>>Enter Loop");
}

void testMsg(){

  MQTT::Message message;
  char* topic = "/tag/screen";
    // QoS 0
  char buf[100];
  sprintf(buf, "{\"text\":\"asdasdas\",\"iswrap\":true}");
  message.qos = MQTT::QOS0;
  message.retained = false;
  message.dup = false;
  message.payload = (void*)buf;
  message.payloadlen = strlen(buf)+1;
  int rc = client.publish(topic, message);

  if (rc != 0) {
      Serial.println("Message sent");
  } else {
    Serial.println("Message fail");
    
  }
  
}



void loop() {

  readButA();
  readButB();
  client.yield(50);
  //delay(1000);
}
