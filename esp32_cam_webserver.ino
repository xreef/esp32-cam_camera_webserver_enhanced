#include "esp_camera.h"
#include <WiFi.h>
#include "utils/parsebytes.h"

#include "config.h"

#define VERSION 1.0

void startCameraServer();

void setup() {
  Serial.begin(115200);
  Serial.setDebugOutput(true);
  Serial.println();

  camera_config_t config;
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;
  config.pin_d0 = Y2_GPIO_NUM;
  config.pin_d1 = Y3_GPIO_NUM;
  config.pin_d2 = Y4_GPIO_NUM;
  config.pin_d3 = Y5_GPIO_NUM;
  config.pin_d4 = Y6_GPIO_NUM;
  config.pin_d5 = Y7_GPIO_NUM;
  config.pin_d6 = Y8_GPIO_NUM;
  config.pin_d7 = Y9_GPIO_NUM;
  config.pin_xclk = XCLK_GPIO_NUM;
  config.pin_pclk = PCLK_GPIO_NUM;
  config.pin_vsync = VSYNC_GPIO_NUM;
  config.pin_href = HREF_GPIO_NUM;
  config.pin_sscb_sda = SIOD_GPIO_NUM;
  config.pin_sscb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn = PWDN_GPIO_NUM;
  config.pin_reset = RESET_GPIO_NUM;
  config.xclk_freq_hz = 20000000;
  config.pixel_format = PIXFORMAT_JPEG;

  // if PSRAM IC present, init with UXGA resolution and higher JPEG quality
  //                      for larger pre-allocated frame buffer.
  if(psramFound()){
    config.frame_size = FRAMESIZE_UXGA;
    config.jpeg_quality = 10;
    config.fb_count = 2;
  } else {
    config.frame_size = FRAMESIZE_SVGA;
    config.jpeg_quality = 12;
    config.fb_count = 1;
  }

#if defined(CAMERA_MODEL_ESP_EYE)
  pinMode(13, INPUT_PULLUP);
  pinMode(14, INPUT_PULLUP);
#endif

  // camera init
  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) {
    Serial.printf("Camera init failed with error 0x%x", err);
    return;
  }

  sensor_t * s = esp_camera_sensor_get();
  // initial sensors are flipped vertically and colors are a bit saturated
  if (s->id.PID == OV3660_PID) {
    s->set_vflip(s, 1); // flip it back
    s->set_brightness(s, 1); // up the brightness just a bit
    s->set_saturation(s, -2); // lower the saturation
  }
  // drop down frame size for higher initial frame rate
  s->set_framesize(s, INITIAL_FRAME_SIZE);

#if defined(CAMERA_MODEL_M5STACK_WIDE) || defined(CAMERA_MODEL_M5STACK_ESP32CAM)
  s->set_vflip(s, 1);
  s->set_hmirror(s, 1);
#endif
//  IPAddress ip(192,168,1,40);
//  IPAddress gateway(192,168,1,1);
//  IPAddress subnet(255,255,255,0);
//  WiFi.config(ip, gateway, subnet);
//
//  WiFi.begin(ssid, password);
//
//  while (WiFi.status() != WL_CONNECTED) {
//    delay(500);
//    Serial.print(".");
//  }
//  Serial.println("");
//  Serial.println("WiFi connected");

#ifdef WIFI_MANAGER
  WiFi.mode(WIFI_STA); // explicitly set mode, esp defaults to STA+AP
  // it is a good practice to make sure your code sets wifi mode how you want it.
  //WiFiManager, Local intialization. Once its business is done, there is no need to keep it around
  WiFiManager wm;

  // reset settings - wipe stored credentials for testing
  // these are stored by the esp library
  wm.resetSettings();

  // Automatically connect using saved credentials,
  // if connection fails, it starts an access point with the specified name ( "AutoConnectAP"),
  // if empty will auto generate SSID, if password is blank it will be anonymous AP (wm.autoConnect())
  // then goes into a blocking loop awaiting configuration and will return success result

  bool res;
  // res = wm.autoConnect(); // auto generated AP name from chipid
  res = wm.autoConnect("MischiantiCAM"); // anonymous ap
  // res = wm.autoConnect("AutoConnectAP","password"); // password protected ap

  if(!res) {
      Serial.println("Failed to connect");
      // ESP.restart();
  }
  else {
      //if you get here you have connected to the WiFi
      Serial.println("connected...yeey :)");
  }


#else
  // Start with accesspoint mode disabled, wifi setup will activate it if
  // no known networks are found, and WIFI_AP_ENABLE has been defined
  bool accesspoint = false;

  // Disable power saving on WiFi to improve responsiveness
  // (https://github.com/espressif/arduino-esp32/issues/1484)
  WiFi.setSleep(false);

  // Number of known networks in stationList[]
  int stationCount = sizeof(stationList)/sizeof(stationList[0]);
  // If we have AP mode enabled, ignore first entry in the stationList[]
  #if defined(WIFI_AP_ENABLE)
      int firstStation = 1;
  #else
      int firstStation = 0;
  #endif

  Serial.print(F("Known external SSIDs: "));
  if (stationCount > firstStation) {
      for (int i=firstStation; i < stationCount; i++) Serial.printf(" '%s'", stationList[i].ssid);
  } else {
      Serial.print("None");
  }
  Serial.println();
  byte mac[6] = {0,0,0,0,0,0};
  WiFi.macAddress(mac);
  Serial.printf("MAC address: %02X:%02X:%02X:%02X:%02X:%02X\r\n", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);

  int bestStation = -1;
  long bestRSSI = -1024;
  char bestSSID[65] = "";
  uint8_t bestBSSID[6];
  if (stationCount > firstStation) {
      // We have a list to scan
      Serial.println(F("Scanning local Wifi Networks\r\n"));
      int stationsFound = WiFi.scanNetworks();
      Serial.printf("%i networks found\r\n", stationsFound);
      if (stationsFound > 0) {
          for (int i = 0; i < stationsFound; ++i) {
              // Print SSID and RSSI for each network found
              String thisSSID = WiFi.SSID(i);
              int thisRSSI = WiFi.RSSI(i);
              String thisBSSID = WiFi.BSSIDstr(i);
              Serial.printf("%3i : [%s] %s (%i)", i + 1, thisBSSID.c_str(), thisSSID.c_str(), thisRSSI);
              // Scan our list of known external stations
              for (int sta = firstStation; sta < stationCount; sta++) {
                  if ((strcmp(stationList[sta].ssid, thisSSID.c_str()) == 0) ||
                  (strcmp(stationList[sta].ssid, thisBSSID.c_str()) == 0)) {
                      Serial.print("  -  Known!");
                      // Chose the strongest RSSI seen
                      if (thisRSSI > bestRSSI) {
                          bestStation = sta;
                          strncpy(bestSSID, thisSSID.c_str(), 64);
                          // Convert char bssid[] to a byte array
                          parseBytes(thisBSSID.c_str(), ':', bestBSSID, 6, 16);
                          bestRSSI = thisRSSI;
                      }
                  }
              }
              Serial.println();
          }
      }
  } else {
      // No list to scan, therefore we are an accesspoint
      accesspoint = true;
  }

  if (bestStation == -1) {
      if (!accesspoint) {
          #if defined(WIFI_AP_ENABLE)
              Serial.println(F("No known networks found, entering AccessPoint fallback mode"));
              accesspoint = true;
          #else
              Serial.println(F("No known networks found"));
          #endif
      } else {
          Serial.println(F("AccessPoint mode selected in config"));
      }
  } else {
      Serial.printf("Connecting to Wifi Network %d: [%02X:%02X:%02X:%02X:%02X:%02X] %s \r\n",
                     bestStation, bestBSSID[0], bestBSSID[1], bestBSSID[2], bestBSSID[3],
                     bestBSSID[4], bestBSSID[5], bestSSID);
      // Apply static settings if necesscary
      if (stationList[bestStation].dhcp == false) {
          #if defined(ST_IP)
              Serial.println(F("Applying static IP settings"));
              #if !defined (ST_GATEWAY)  || !defined (ST_NETMASK)
                  #error "You must supply both Gateway and NetMask when specifying a static IP address"
              #endif
              IPAddress staticIP(ST_IP);
              IPAddress gateway(ST_GATEWAY);
              IPAddress subnet(ST_NETMASK);
              #if !defined(ST_DNS1)
                  WiFi.config(staticIP, gateway, subnet);
              #else
                  IPAddress dns1(ST_DNS1);
              #if !defined(ST_DNS2)
                  WiFi.config(staticIP, gateway, subnet, dns1);
              #else
                  IPAddress dns2(ST_DNS2);
                  WiFi.config(staticIP, gateway, subnet, dns1, dns2);
              #endif
              #endif
          #else
              Serial.println(F("Static IP settings requested but not defined in config, falling back to dhcp"));
          #endif
      }

      #if defined(HOSTNAME)
          WiFi.setHostname(HOSTNAME);
      #endif

      // Initiate network connection request (3rd argument, channel = 0 is 'auto')
      WiFi.begin(bestSSID, stationList[bestStation].password, 0, bestBSSID);
      while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
      }
      Serial.println("");
      Serial.println(F("WiFi connected"));
  }
#endif

  startCameraServer();

  Serial.print(F("Camera Ready! Use 'http://"));
  Serial.print(WiFi.localIP());
  Serial.println(F("' to connect"));
}

void loop() {
  // put your main code here, to run repeatedly:
  delay(10000);
}
