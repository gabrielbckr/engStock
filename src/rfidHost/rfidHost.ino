
#include <SPI.h>
#include <MFRC522.h>
 
#define RST_PIN 5 
#define SS_PIN 53

MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance.
 
void setup() 
{
  Serial.begin(9600);   // Initiate a serial communication
  SPI.begin();      // Initiate  SPI bus
  mfrc522.PCD_Init();   // Initiate MFRC522
  Serial.println("Approximate your card to the reader...");
  Serial.println();
  // Serial.println("Writing it");
  // writeMacAddressIntoTag("");
  // Serial.println("Done");
  
}
void loop() 
{
  Serial.println(readMacAddressFromTag());
  // Serial.println("Nein");
  delay(700);

  mfrc522.PICC_HaltA(); // Halt PICC
  mfrc522.PCD_StopCrypto1();  // Stop encryption on PCD
} 

String readMacAddressFromTag()
{

  MFRC522::MIFARE_Key key;
  for (byte i = 0; i < 6; i++) key.keyByte[i] = 0xFF;


  byte block = 4;
  byte contentLength = 18*2;
  byte content[18*2];
  String charContent = String("");

  if ( ! mfrc522.PICC_IsNewCardPresent()) {    
    return String("Card Unavailable");
  }

  if ( ! mfrc522.PICC_ReadCardSerial()) {
    return String("cannot read Card");
  }

  MFRC522::StatusCode status;
  
  status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, block, &key, &(mfrc522.uid));
  if (status != MFRC522::STATUS_OK) {
    Serial.print(F("PCD_Authenticate() failed: "));
    Serial.println(mfrc522.GetStatusCodeName(status));
    return String("PCD_Authenticate() failed");
  }

  status = mfrc522.MIFARE_Read(block, content, &contentLength);
  if (status != MFRC522::STATUS_OK) {
    Serial.println(mfrc522.GetStatusCodeName(status));
    return String("Cound not read");
  }
  
	// mfrc522.PICC_DumpToSerial(&(mfrc522.uid));

  for(int ii=0; ii<18*2; ii++)
  {
    charContent+= (char)content[ii];
    // Serial.write(content[ii]);
  }


  mfrc522.PICC_HaltA();
  mfrc522.PCD_StopCrypto1();

  return charContent;
}

void writeMacAddressIntoTag(String macAddress)
{
  MFRC522::MIFARE_Key key;
  for (byte i = 0; i < 6; i++) key.keyByte[i] = 0xFF;
  // 123456789012
  // 0macaddress0

  char* content = "0macaddress0";
  byte buffer[18] ;
  byte block = 4;
  byte len = 18;
  MFRC522::StatusCode status;

  for(int ii = 0; ii<18; ii++)
  {
    buffer[ii] = (byte)content[ii];
  Serial.write(buffer[ii]);
  }


  while ( ! mfrc522.PICC_IsNewCardPresent()) {
    // return;
  }

  if ( ! mfrc522.PICC_ReadCardSerial()) {
    return;
  }


  status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, block, &key, &(mfrc522.uid));
  if (status != MFRC522::STATUS_OK) {
    Serial.print(F("PCD_Authenticate() failed: "));
    Serial.println(mfrc522.GetStatusCodeName(status));
    return;
  }
  else Serial.println(F("PCD_Authenticate() success: "));

  // Write block
  status = mfrc522.MIFARE_Write(block, buffer, 16);
  if (status != MFRC522::STATUS_OK) {
    Serial.print(F("MIFARE_Write() failed: "));
    Serial.println(mfrc522.GetStatusCodeName(status));
    return;
  }

  mfrc522.PICC_HaltA(); // Halt PICC
  mfrc522.PCD_StopCrypto1();  // Stop encryption on PCD

}