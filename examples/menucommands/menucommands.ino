/* 
  Menu driven control of a sound board over UART.
  Commands for playing by # or by name (full 11-char name)
  Hard reset and List files (when not playing audio)
  Vol + and - (only when not playing audio)
  Pause, unpause, quit playing (when playing audio)
  Current play time, and bytes remaining & total bytes (when playing audio)

  Connect UG to ground to have the sound board boot into UART mode
*/

#include <SoftwareSerial.h>
#include "Adafruit_Soundboard.h"


// Choose any two pins that can be used with SoftwareSerial to RX & TX
#define SFX_TX 5
#define SFX_RX 6

// Connect to the RST pin on the Sound Board
#define SFX_RST 4

// You can also monitor the ACT pin for when audio is playing!

// we'll be using software serial
SoftwareSerial ss = SoftwareSerial(SFX_TX, SFX_RX);

// pass the software serial to Adafruit_soundboard, the second
// argument is the debug port (not used really) and the third 
// arg is the reset pin
Adafruit_Soundboard sfx = Adafruit_Soundboard(&ss, NULL, SFX_RST);
// can also try hardware serial with
// Adafruit_Soundboard sfx = Adafruit_Soundboard(&Serial1, NULL, SFX_RST);

void setup() {
  Serial.begin(115200);
  Serial.println("Adafruit Sound Board!");
  
  // softwareserial at 9600 baud
  ss.begin(9600);
  // can also do Serial1.begin(9600)

  if (!sfx.reset()) {
    Serial.println("Not found");
    while (1);
  }
  Serial.println("SFX board found");
}


void loop() {

  Serial.println(F("What would you like to do?"));
  Serial.println(F("[r] - reset"));
  Serial.println(F("[+] - Vol +"));
  Serial.println(F("[-] - Vol -"));
  Serial.println(F("[L] - List files"));
  Serial.println(F("[P] - play by file name"));
  Serial.println(F("[#] - play by file number"));
  Serial.println(F("[=] - pause playing"));
  Serial.println(F("[>] - unpause playing"));
  Serial.println(F("[q] - stop playing"));
  Serial.println(F("[t] - playtime status"));
  Serial.println(F("> "));
  
  // Read the response.  We will only use the first letter, read the line to clear everything sent (like the line return).
  const uint8_t bufferSize = 20;
  char lineBuffer[bufferSize];
  readline(lineBuffer, bufferSize);

  switch (lineBuffer[0]) {
    case 'r': {
      if (!sfx.reset()) {
        Serial.println(F("Reset failed"));
      }
      break; 
    }
    
    case 'L': {
      uint8_t files = sfx.listFiles();
    
      Serial.println(F("File Listing"));
      Serial.println(F("========================"));
      Serial.println();
      Serial.print(F("Found ")); Serial.print(files); Serial.println(F(" Files"));
      for (uint8_t f=0; f<files; f++) {
        Serial.print(f); 
        Serial.print(F("\tname: ")); Serial.print(sfx.fileName(f));
        Serial.print(F("\tsize: ")); Serial.println(sfx.fileSize(f));
      }
      Serial.println(F("========================"));
      break; 
    }
    
    case '#': {
      Serial.print(F("Enter track #"));
      uint8_t n = readnumber();

      Serial.print(F("\nPlaying track #")); Serial.println(n);
      if (! sfx.playTrack((uint8_t)n) ) {
        Serial.println(F("Failed to play track?"));
      }
      break;
    }
    
    case 'P': {
      Serial.print(F("Enter track name (full 12 character name!) >"));
      readline(lineBuffer, bufferSize);

      Serial.print(F("\nPlaying track \"")); Serial.print(lineBuffer); Serial.print(F("\""));
      if (! sfx.playTrack(lineBuffer) ) {
        Serial.println(F("Failed to play track?"));
      }
      break;
   }

   case '+': {
      Serial.println(F("Vol up..."));
      uint16_t v;
      if (! (v = sfx.volUp()) ) {
        Serial.println(F("Failed to adjust"));
      } else {
        Serial.print(F("Volume: ")); Serial.println(v);
      }
      break;
   }

   case '-': {
      Serial.println(F("Vol down..."));
      uint16_t v;
      if (! (v=sfx.volDown()) ) {
        Serial.println(F("Failed to adjust"));
      } else { 
        Serial.print(F("Volume: "));
        Serial.println(v);
      }
      break;
   }
   
   case '=': {
      Serial.println(F("Pausing..."));
      if (! sfx.pause() ) Serial.println(F("Failed to pause"));
      break;
   }
   
   case '>': {
      Serial.println(F("Unpausing..."));
      if (! sfx.unpause() ) Serial.println(F("Failed to unpause"));
      break;
   }
   
   case 'q': {
      Serial.println(F("Stopping..."));
      if (! sfx.stop() ) Serial.println(F("Failed to stop"));
      break;
   }  

   case 't': {
      Serial.print(F("Track time: "));
      uint32_t current = 0;
	  uint32_t total = 0;
      if (! sfx.trackTime(&current, &total) ) Serial.println(F("Failed to query"));
      Serial.print(current); Serial.println(F(" seconds"));
      break;
   }  

   case 's': {
      Serial.print(F("Track size (bytes remaining/total): "));
      uint32_t remain = 0;
	  uint32_t total = 0;
      if (! sfx.trackSize(&remain, &total) ) 
        Serial.println(F("Failed to query"));
      Serial.print(remain); Serial.print("/"); Serial.println(total); 
      break;
   }  

  }
}






/************************ MENU HELPERS ***************************/

char readBlocking() {
  while (!Serial.available());
  return Serial.read();
}

uint16_t readnumber() {
  uint16_t x = 0;
  char c;
  while (! isdigit(c = readBlocking())) {
    //Serial.print(c);
  }
  Serial.print(c);
  x = c - '0';
  while (isdigit(c = readBlocking())) {
    Serial.print(c);
    x *= 10;
    x += c - '0';
  }
  return x;
}

uint8_t readline(char *buff, uint8_t maxbuff) {
  uint16_t buffidx = 0;
  
  while (true) {
    if (buffidx > maxbuff) {
      break;
    }

    if (Serial.available()) {
      char c =  Serial.read();
      //Serial.print(c, HEX); Serial.print("#"); Serial.println(c);

      if (c == '\r') continue;
      if (c == 0xA) {
        if (buffidx == 0) {  // the first 0x0A is ignored
          continue;
        }
        buff[buffidx] = 0;  // null term
        return buffidx;
      }
      buff[buffidx] = c;
      buffidx++;
    }
  }
  buff[buffidx] = 0;  // null term
  return buffidx;
}
/************************ MENU HELPERS ***************************/