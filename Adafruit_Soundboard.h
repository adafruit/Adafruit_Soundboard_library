/*************************************************** 
  This is a library for the Adafruit Sound Boards in UART mode

  ----> http://www.adafruit.com/products/2342
  ----> http://www.adafruit.com/products/2341
  ----> http://www.adafruit.com/products/2217
  ----> http://www.adafruit.com/products/2210
  ----> http://www.adafruit.com/products/2133
  ----> http://www.adafruit.com/products/2220

  Check out the links above for our tutorials and wiring diagrams
  This sound fx driver uses TTL Serial to communicate

  Adafruit invests time and resources providing this open source code,
  please support Adafruit and open-source hardware by purchasing
  products from Adafruit!

  Written by Limor Fried/Ladyada for Adafruit Industries.
  MIT license, all text above must be included in any redistribution
 ****************************************************/

#ifndef _ADAFRUIT_SOUNDBOARD_H_
#define _ADAFRUIT_SOUNDBOARD_H_

#include "Arduino.h"

struct FileInfo
{
  char      filename[12];
  uint32_t  size;
};

typedef void (*fileListHandler)(uint8_t id, const FileInfo& fileInfo);

class Adafruit_Soundboard : public Print {
 public:
  Adafruit_Soundboard(Stream *stream, Stream *debug, int8_t r);

  boolean reset(void);

  int     readLine(void);
  /**
   *  Gets a list of files. You may pass in:
   *  - a pointer to an array of FileInfo structures, and the size
   *    (nInfoArr) of that array. listFiles() will fill in the 
   *    filenames and size of the files.
   * - a callback function to be called for each file in the list.
   * 
   * You may pass in either, both, or neither. The number of files
   * is returned.
   */
  uint8_t listFiles(FileInfo* infoArr, int nInfoArr, fileListHandler handler);

  uint8_t volUp(void);
  uint8_t volDown(void);
  uint8_t setVol(uint8_t vol);

  boolean playTrack(uint8_t n);
  boolean playTrack(const char *name);
  boolean pause(void);
  boolean unpause(void);
  boolean stop(void);

  boolean trackTime(uint32_t *current, uint32_t *total);
  boolean trackSize(uint32_t *current, uint32_t *total);

 private:
  enum { LINE_BUFFER_SIZE = 80 };

  Stream   *stream;     // -> sound board, e.g. SoftwareSerial or Serial1
  Stream    *debug;      // -> host, e.g. Serial
  
  int8_t reset_pin;
  char line_buffer[LINE_BUFFER_SIZE];
  boolean writing;

  virtual size_t write(uint8_t); // Because Print subclass
};
#endif
