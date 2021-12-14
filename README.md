## esp32-cam firmware to use with BeePrint
Some additional config
 - you can set wifi manager at startutp to insert wifi intercatly: #define WIFI_MANAGER
 - you can set multiple wifi setting 
 `station stationList[] = {{"SSID1","passwd1", true},{"SSID2","passwd2", true}};`
 - if you set falst the dhcp parameter
 `station stationList[] = {{"SSID1","passwd1", false},{"SSID2","passwd2", false}};`
 and add ip config
 ```cpp
 #define ST_IP      192,168,1,41
 #define ST_GATEWAY 192,168,1,1
 #define ST_NETMASK 255,255,255,0
 // One or two DNS servers can be supplied, only the NTP code currently uses them
 #define ST_DNS1 192,168,1,1
 // #define ST_DNS2 8,8,8,8
 ```
 - you can set the initial resolution of the cam
```cpp
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
```
 

https://www.mischianti.org/category/project/web-interface-beeprint-for-mks-wifi/

### ESP32-CAM Tutorial

![](https://www.mischianti.org/wp-content/uploads/2021/03/ESP32-CAM-clone-upgrade-web-interface-with-flash-light-720x340.jpg)

### 3D printed box for esp32-cam

![](https://www.mischianti.org/wp-content/uploads/2021/03/3d-printed-Modular-System-ESP32-CAM-holder-main-720x340.jpg)


### Credits

WiFi management get from easytarget project.