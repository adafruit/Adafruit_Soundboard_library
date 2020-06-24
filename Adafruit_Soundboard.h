/*!
 * @file Adafruit_Soundboard.h
 */

#ifndef _ADAFRUIT_SOUNDBOARD_H_
#define _ADAFRUIT_SOUNDBOARD_H_

#include "Arduino.h"

#define LINE_BUFFER_SIZE 80 //!< Size of the line buffer
#define MAXFILES 25         //!< Max number of files

/*!
 * @brief Class that stores the state and functions of the soundboard object
 */
class Adafruit_Soundboard : public Print {
public:
  /*!
   * @brief Adafruit_Soundboard constructor
   * @param s Pointer to the Serial stream
   * @param d Pointer to the debug port
   * @param r Reset pin
   */
  Adafruit_Soundboard(Stream *s, Stream *d, int8_t r);

  boolean reset(void);

  /*!
   * @brief Reads a line from the stream
   * @return Returns the line read
   */
  int readLine(void);
  uint8_t listFiles(void);

  /*!
   * @brief Returns the filename
   * @param n Id of the file
   * @return Returns the filename
   */
  char *fileName(uint8_t n);
  /*!
   * @brief Returns the size of the file
   * @param n id of the file
   * @return Returns the filesize
   */
  uint32_t fileSize(uint8_t n);

  /*!
   * @brief Raises the volume
   * @return Returns the current volume
   */
  uint8_t volUp(void);
  /*!
   * @brief Lowers the volume
   * @return Returns the current volume
   */
  uint8_t volDown(void);

  /*!
   * @brief Play the specified track
   * @param n track id
   * @return Returns true if the track was played
   */
  boolean playTrack(uint8_t n);
  /*!
   * @brief Play the specified track
   * @param name track name
   * @return Returns true if the track was played
   */
  boolean playTrack(char *name);
  /*!
   * @brief Pauses track
   * @return Returns if pausing was successful
   */
  boolean pause(void);
  /*!
   * @brief Unpauses track
   * @return Returns if unpausing was successful
   */
  boolean unpause(void);
  /*!
   * @brief Stops track
   * @return Returns if stopping was successful
   */
  boolean stop(void);

  /*!
   * @brief Returns the track time
   * @param current Buffer with the current track time
   * @param total Buffer with the total track time
   * @return Returns the current track time
   */
  boolean trackTime(uint32_t *current, uint32_t *total);
  /*!
   * @brief Returns the track size
   * @param current Buffer with the current track size
   * @param total Buffer with the total track size
   * @return Returns how many bytes are remaining over the total track size
   */
  boolean trackSize(uint32_t *current, uint32_t *total);

private:
  Stream *stream; // -> sound board, e.g. SoftwareSerial or Serial1
  Stream *debug;  // -> host, e.g. Serial

  int8_t reset_pin;
  char line_buffer[LINE_BUFFER_SIZE];
  boolean writing;

  // File name & size caching
  uint8_t files;
  char filenames[MAXFILES][12];
  uint32_t filesizes[MAXFILES];

  virtual size_t write(uint8_t); // Because Print subclass
};
#endif
