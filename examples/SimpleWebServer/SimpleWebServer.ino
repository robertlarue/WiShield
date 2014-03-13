/*
 * Web Server
 *
 * A simple web server example using the WiShield 2.0
 *
 * Author               Date        Comment
 * --------------------------------------------------------------
 * AsyncLabs		05/29/2009  Initial version
 *
 * P. Ciuffetti		03/12/2014  Modified web page and added
 *                                  WiShield 2.0 led control
 */

#include <WiShield.h>

#define WIRELESS_MODE_INFRA	1
#define WIRELESS_MODE_ADHOC	2

// Wireless configuration parameters ----------------------------------------
unsigned char local_ip[]       = {192,168,1,2}; // IP address of WiShield
unsigned char gateway_ip[]     = {192,168,1,1}; // router or gateway IP address
unsigned char subnet_mask[]    = {255,255,255,0}; // subnet mask for the local network
const prog_char ssid[] PROGMEM = {"ssid"};  // max 32 bytes

unsigned char security_type = 3; // 0 - open; 1 - WEP; 2 - WPA; 3 - WPA2

// WPA/WPA2 passphrase
const prog_char security_passphrase[] PROGMEM = {"wifi_password"}; // max 64 characters

// WEP 128-bit keys
// sample HEX keys
prog_uchar wep_keys[] PROGMEM = {
  0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d,	// Key 0
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,	// Key 1
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,	// Key 2
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00	// Key 3
};

// setup the wireless mode
// infrastructure - connect to AP
// adhoc - connect to another WiFi device
unsigned char wireless_mode = WIRELESS_MODE_INFRA;

unsigned char ssid_len;
unsigned char security_passphrase_len;
//---------------------------------------------------------------------------

void setup() {
  WiFi.init();
  
  // Set pin 9 as an output (this is the led on the WiShield 2.0
  pinMode(9,OUTPUT);
}

// This is the header sent to the browser before the real web page served by the webserver
const prog_char webpage[] PROGMEM = {
  "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n"
};

void loop() {
  WiFi.run();
}
