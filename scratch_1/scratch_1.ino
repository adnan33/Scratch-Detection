#include <SD.h> //Load SD library
#include <Wire.h>
#include <FaBo9Axis_MPU9250.h>
#include <SoftwareSerial.h>
#include <BlynkSimpleSerialBLE.h>
#define BLYNK_PRINT Serial
#define BLYNK_USE_DIRECT_CONNECT

SoftwareSerial DebugSerial(5, 6); // RX, TX
FaBo9Axis fabo_9axis;
int chipSelect = 10; //chip select pin for the MicroSD Card Adapter
File file;
float data[10];
String FileName="data_file.txt";

char auth[] = "[YOURAPPAUTH]";//authentication code from Blynk app

char ACC_x[]="V0";
char ACC_y[]="V1";
char ACC_z[]="V2";
char temp_pin[]="V3";

// file object that is used to read and write data
void setup() {
  DebugSerial.begin(115200);

  DebugSerial.println("Waiting for connections...");
  // put your setup code here, to run once:
   Serial.begin(115200); // start serial connection to print out debug messages and data
     pinMode(chipSelect, OUTPUT); // chip select pin must be set to OUTPUT mode
  if (!SD.begin(chipSelect)) { // Initialize SD card
    Serial.println("Could not initialize SD card."); // if return value is false, something went wrong.
  }
  
  if (SD.exists("file.txt")) { // if "file.txt" exists, fill will be deleted
    Serial.println("File exists.");
    
    
  }
 if (fabo_9axis.begin()) {
    Serial.println("configured FaBo 9Axis I2C Brick");
  } else {
    Serial.println("device error");
    while(1);
  }

  Blynk.begin(DebugSerial, auth);
}

void vwrite()
{
  Blynk.virtualWrite(V6, data[9]);
  Blynk.virtualWrite(V0, data[0]);
  Blynk.virtualWrite(V1, data[1]);
  Blynk.virtualWrite(V2, data[2]); 
  Blynk.virtualWrite(V3, data[3]);
  Blynk.virtualWrite(V4, data[4]);
  Blynk.virtualWrite(V5, data[5]);
  
}
void loop() {
  // put your main code here, to run repeatedly:
  
  file = SD.open("datafile.txt", FILE_WRITE); // open "file.txt" to write data
  
  
  fabo_9axis.readAccelXYZ(&data[0],&data[1],&data[2]);
  fabo_9axis.readGyroXYZ(&data[3],&data[4],&data[5]);
  fabo_9axis.readMagnetXYZ(&data[6],&data[7],&data[8]);
  fabo_9axis.readTemperature(&data[9]);

  if (file)
  { 
    String data_str="";
   for (int i=0;i<10;i++)
   {
    data_str+=String(data[i]);
    data_str+=",";
   }
    file.println(data_str); // write number to file
    file.close(); // close file
    Serial.println("Wrote number: "); // debug output: show written number in serial monitor
    Serial.println(data_str);
  } 
  else 
  {
    Serial.println("Could not open file (writing).");
  }

  delay(200);
  Blynk.run();
  vwrite();
}
