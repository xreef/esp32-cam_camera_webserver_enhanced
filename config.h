#ifndef CONFIG_H_
#define CONFIG_H_

#include "sensor.h"
//
// WARNING!!! PSRAM IC required for UXGA resolution and high JPEG quality
//            Ensure ESP32 Wrover Module or other board with PSRAM is selected
//            Partial images will be transmitted if image exceeds buffer size
//

// Select camera model
//#define CAMERA_MODEL_WROVER_KIT // Has PSRAM
//#define CAMERA_MODEL_ESP_EYE // Has PSRAM
//#define CAMERA_MODEL_M5STACK_PSRAM // Has PSRAM
//#define CAMERA_MODEL_M5STACK_V2_PSRAM // M5Camera version B Has PSRAM
//#define CAMERA_MODEL_M5STACK_WIDE // Has PSRAM
//#define CAMERA_MODEL_M5STACK_ESP32CAM // No PSRAM
#define CAMERA_MODEL_AI_THINKER // Has PSRAM
//#define CAMERA_MODEL_TTGO_T_JOURNAL // No PSRAM

#include "camera_pins.h"

/* Give the camera a name for the web interface */
#define CAM_NAME "ESP32 camera server"

//#define WIFI_MANAGER

#ifdef WIFI_MANAGER
	#include <WiFiManager.h> // https://github.com/tzapu/WiFiManager
#else
/*
 *    WiFi Settings
 *
 *    For the simplest connection to an existing network
 *    just replace your ssid and password in the line below.
 */
typedef struct
  {
	const char ssid[65];
	const char password[65];
	const bool dhcp;
  }  station;

station stationList[] = {{"YOUR-SSID","YOUR-PASSWD", true},{"YOUR-SSID2","YOUT-PASSWD2", true}};

//     #define WIFI_AP_ENABLE

/*
 * You can extend the stationList[] above with additional SSID+Password pairs

struct station stationList[] = {{"ssid1", "pass1", true},
                                {"ssid2", "pass2", true},
                                {"ssid3", "pass3", false}};

 * Note the use of nested braces '{' and '}' to group each entry, and commas ',' to seperate them.
 *
 * The first entry (ssid1, above) in the stationList is special, if WIFI_AP_ENABLE has been uncommented (below)
 * it will be used for the AccessPoint ssid and password. See the comments there for more.
 *
 * The 'dhcp' setting controls whether the station uses DHCP or static IP settings; if in doubt leave 'true'
  *
 * You can also use a BSSID (eg: "2F:67:94:F5:BB:6A", a colon seperated mac address string) in place of
 * the ssid to force connections to specific networks even when the ssid's collide,
 */

/* Extended WiFi Settings */

/*
 * Hostname. Optional, uncomment and set if desired
 * - used in DHCP request when connecting to networks, not used in AP mode
 * - Most useful when used with a static netwrk config, not all routers respect this setting
 *
 * The URL_HOSTNAME will be used in place of the IP address in internal URL's
 */

// #define HOSTNAME "esp-cam"
// #define URL_HOSTNAME "esp-cam"

/*
 * Static network settings for client mode
 *
 * Note: The same settings will be applied to all client connections where the dhcp setting is 'false'
 * You must define all three: IP, Gateway and NetMask
 */
// warning - IP addresses must be seperated with commas (,) and not decimals (.)
//#define ST_IP      192,168,1,41
//#define ST_GATEWAY 192,168,1,1
//#define ST_NETMASK 255,255,255,0
// One or two DNS servers can be supplied, only the NTP code currently uses them
//#define ST_DNS1 192,168,1,1
// #define ST_DNS2 8,8,8,8

/*
 *  AccessPoint;
 *
 *  Uncomment to enable AP mode;
 *
 */
// #define WIFI_AP_ENABLE

/*  AP Mode Notes:
 *
 *  Once enabled the AP ssid and password will be taken from the 1st entry in the stationList[] above.
 *
 *  If there are further entries listed they will be scanned at startup in the normal way and connected to
 *  if they are found. AP then works as a fallback mode for when there are no 'real' networks available.
 *
 *  Setting the 'dhcp' field to true for the AP enables a captive portal and attempts to send
 *  all visitors to the webcam page, with varying degrees of success depending on the visitors
 *  browser and other settings.
 */
// Optionally change the AccessPoint ip address (default = 192.168.4.1)
// warning - IP addresses must be seperated with commas (,) and not decimals (.)
// #define AP_ADDRESS 192,168,4,1

// Uncomment this to force the AccessPoint channel number, default = 1
// #define AP_CHAN 1
#endif

#define UXGA  FRAMESIZE_XGA               // UXGA(1600x1200)
#define SXGA  FRAMESIZE_SVGA               // SXGA(1280x1024)
#define XGA   FRAMESIZE_VGA               // XGA(1024x768)
#define SVGA  FRAMESIZE_HVGA               // SVGA(800x600)
#define VGA   FRAMESIZE_CIF               // VGA(640x480)
#define CIF   FRAMESIZE_QVGA               // CIF(400x296)
#define QVGA  FRAMESIZE_240X240               // QVGA(320x240)
#define HQVGA FRAMESIZE_HQVGA               // HQVGA(240x176)
#define QQVGA FRAMESIZE_96X96               // QQVGA(160x120)

#define INITIAL_FRAME_SIZE UXGA

#endif
