#include <SPI.h> //Serial Port connection enable
#include <MFRC522.h> //RFID module import
#include <SoftwareSerial.h> //bluetooth module import
/*
 * Typical pin layout used:
 * -----------------------------------------------------------------------------------------
 *             MFRC522      Arduino       Arduino   Arduino    Arduino          Arduino
 *             Reader/PCD   Uno/101       Mega      Nano v3    Leonardo/Micro   Pro Micro
 * Signal      Pin          Pin           Pin       Pin        Pin              Pin
 * -----------------------------------------------------------------------------------------
 * RST/Reset   RST          9             5         D9         RESET/ICSP-5     RST
 * SPI SS      SDA(SS)      10            53        D10        10               10
 * SPI MOSI    MOSI         11 / ICSP-4   51        D11        ICSP-4           16
 * SPI MISO    MISO         12 / ICSP-1   50        D12        ICSP-1           14
 * SPI SCK     SCK          13 / ICSP-3   52        D13        ICSP-3           15
 */


#define RST_PIN         5          // Configurable, see typical pin layout above
#define SS_PIN          53         // Configurable, see typical pin layout above

MFRC522 mfrc522(SS_PIN, RST_PIN);  // Create MFRC522 instance

SoftwareSerial BTserial(15, 14); // initilizing the RX | TX

char data = 0;

//M1
//int enA = 10;
int in1 = 3;//8; 
int in2 = 4;//9;
//M2
//int enB = 11;
int in3 = 6;//10;
int in4 = 7;//11;



void setup()
{
   Serial.begin(9600);
   BTserial.begin(9600); //start the bluetooth connection

//  pinMode(enA, OUTPUT);
//  pinMode(enB, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);

  SPI.begin();      // Init SPI bus
  mfrc522.PCD_Init();   // Init MFRC522
  mfrc522.PCD_DumpVersionToSerial();  // Show details of PCD - MFRC522 Card Reader details
  Serial.println(F("Scan PICC to see UID, SAK, type, and data blocks..."));
  
}

/*
   mpower(motor number,rotation,speed);

  motor number
  1-1st motor
  2-2nd motor

  rotation
  1-Forword
  0-Stop
  -1- Backword

  speed
  0-255 PWM value

*/

void mpower(int motor, int rotation, int spd) 
{
  int pwm;
  int pA;
  int pB;
  if (motor == 1) 
  {
//        pwm = enA;
    pA = in1;
    pB = in2;
  } 
  else if (motor == 2) 
  {
//    pwm = enB;
    pA = in3;
    pB = in4;
  } 
  else 
  {
    return;
  }

  if (rotation == 0) 
  {
    digitalWrite(pA, LOW);
    digitalWrite(pB, LOW);
  } 
  else if (rotation == 1) 
  {
    digitalWrite(pA, HIGH);
    digitalWrite(pB, LOW);
  } 
  else if (rotation == -1) 
  {
    digitalWrite(pA, LOW);
    digitalWrite(pB, HIGH);
  }
  analogWrite(pwm, spd);

}

void forwardMotors()
{
    mpower(1, -1, 255); 
    mpower(2, -1, 255);
    
}

void reversMotors()
{
    mpower(1, 1, 255); 
    mpower(2, 1, 255);
}

void turnLeft()
{
    mpower(1, 0, 0); 
    mpower(2, 1, 255);
}

void turnRight()
{
    mpower(1, 1, 255); 
    mpower(2, 0,0);
}

void breakMotors()
{
  mpower(1, 0, 0);
  mpower(2, 0, 0);
}

void loop() 
{
  // Look for new cards (This Method is only to detect a tag)
  if ( (! mfrc522.PICC_IsNewCardPresent()) || ( ! mfrc522.PICC_ReadCardSerial())) 
  {   
    if(Serial.available() > 0)
    {
      data = Serial.read();

      if(data == '8')
        forwardMotors();
      else if(data == '2')
        reversMotors();
      else if(data == '4')
        turnLeft();
      else if(data == '6')
        turnRight();
      else if(data == '5')
        breakMotors();
    }
    return;
  }
  /*If any RFID tag is detected then the id of the tag will send via bluetooth to Mobile serial
  */
  else
  {
    breakMotors();

     String s = "";
    for (byte i = 0; i < mfrc522.uid.size; i++)
    {
      Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
      Serial.print(mfrc522.uid.uidByte[i], HEX);
      s += (String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
      s += (String(mfrc522.uid.uidByte[i], HEX));
    }
     BTserial.println("Piumal" + String(s));
    s = "";
//    // Read RFID tag
//     mfrc522.PICC_DumpToSerial(&(mfrc522.uid));
//     /*We need to code here to check is there any related file in the SD card for this tag
//     * If yes then file should play twice.
//     * If not then it should connect to the cloud and download the file from the cloud and save it
//     *    in the SD card before play it.
//     * This shoud send a count of detected tags to cloud.
//     */
//    Serial.println("Piumal");
//     
//     
  }
  
}



