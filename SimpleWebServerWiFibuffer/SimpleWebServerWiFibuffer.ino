/*
  WiFi Web Server LED Blink

 A simple web server that lets you blink an LED via the web.
 This sketch will print the IP address of your WiFi Shield (once connected)
 to the Serial monitor. From there, you can open that address in a web browser
 to turn on and off the LED on pin 9.

 If the IP address of your shield is yourAddress:
 http://yourAddress/H turns the LED on
 http://yourAddress/L turns it off

 This example is written for a network using WPA encryption. For
 WEP or WPA, change the Wifi.begin() call accordingly.

 Circuit:
 * WiFi shield attached
 * LED attached to pin 9

 created 25 Nov 2012
 by Tom Igoe
 */
#include <SPI.h>
#include <WiFi.h>
#include <Wire.h>

char ssid[] = "TP-LINK_60C43A";      //  your network SSID (name)
char pass[] = "99852160";            // your network password

//char ssid[] = "D-Link GO-DSL-AC750";      //  your network SSID (name)
//char pass[] = "bb255d65e2d";            // your network password

int keyIndex = 0;                    // your network key Index number (needed only for WEP)
bool firstpass = true;


// debug
bool led_status = false;
unsigned long new_millis = 0;
unsigned long old_millis = 0;
unsigned long duration = 100;

uint8_t buffer_read[302];

int status = WL_IDLE_STATUS;
WiFiServer server(80);

//uint8_t buffer_read[302];
void setup() {
  Serial.begin(115200);      // initialize serial communication
  Serial.println("Start...");
  pinMode(9, OUTPUT);      // set the LED pin mode
  while(!Serial);
  // check for the presence of the shield:
  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("WiFi shield not present");
    while (true);       // don't continue
  }

  String fv = WiFi.firmwareVersion();
  if (fv != "1.1.0") {
    Serial.println("Please upgrade the firmware");
  }

  // attempt to connect to Wifi network:
  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to Network named: ");
    Serial.println(ssid);                   // print the network name (SSID);

    // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
    status = WiFi.begin(ssid, pass);
    // wait 10 seconds for connection:
    delay(10000);
    delay(5000); //five seconds
  }
  server.begin();                           // start the web server on port 80
  printWifiStatus();                        // you're connected now, so print out the status
}


void loop() {
  WiFiClient client = server.available();   // listen for incoming clients
  // new_millis = millis();
  // if (new_millis - old_millis >= duration) {
  //   // save the last time you blinked the LED
  //   old_millis = new_millis;
  //
  //   // if the LED is off turn it on and vice-versa:
  //   if (led_status == LOW) {
  //     led_status = HIGH;
  //   } else {
  //     led_status = LOW;
  //   }
  // }
  //   digitalWrite(13, led_status);
  //   Serial.println("alive");

  if (client) {                             // if you get a client,
    Serial.println("");
    Serial.println("new client");           // print a message out the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    while (client.connected()) {            // loop while the client's connected
      int buf_size = client.available();
      int charToRecv;
      Serial.println(buf_size);
      if (buf_size) {             // if there's bytes to read from the client,
        if (firstpass) {
          //Serial.print("first: ");
          //Serial.println(firstpass);
          charToRecv = buf_size;
          firstpass = false;
        }
            //int charToRecv = buf_size;
            //for (int index =0; index <= buf_size; index++){
            //Serial.print("first: ");
            //Serial.println(firstpass);
            for (charToRecv; charToRecv >= 0; charToRecv = charToRecv-10) {
              // Serial.print("first: ");
              // Serial.println(firstpass);
              Serial.println("");
              Serial.print("--- ");
              Serial.print(charToRecv);
              Serial.println(" ---");
              //buf_size = 10;
              //Serial.println("");
              //Serial.print("buf_size: ");
              //Serial.println(buf_size);
              //Serial.println("");
              //Serial.print("blocco "); Serial.println(index);
              client.read(buffer_read, 10);
              if (charToRecv >= 10) {
                for(int index = 0; index < 10; index++) {
                  //Serial.println("");
                  //Serial.print("->");
                  //Serial.print(index);
                  Serial.write(buffer_read[index]);
                }
              } else {
                for(int index = 0; index < charToRecv; index++) {
                  //Serial.println("");
                  //Serial.print("-.");
                  //Serial.print(index);
                  Serial.write(buffer_read[index]);
                }
                firstpass = true;
              }


            }
            Serial.println("");
            Serial.println("Fine ricezione e risposta");
            client.println("HTTP/1.1 200 OK");
            //Serial.println("dopo println");
            break;
      }
    }
    // close the connection:
    client.stop();
    Serial.println("client disonnected");
  }
}

void printWifiStatus() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
  // print where to go in a browser:
  Serial.print("To see this page in action, open a browser to http://");
  Serial.println(ip);
}
