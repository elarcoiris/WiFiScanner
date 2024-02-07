#include <Arduino.h>
#include <SPI.h>
#include <ESP8266WiFi.h>

char ssid[] = "";
char password[] = "";

void setup() {
  Serial.begin(115200);
  int status = WiFi.begin(ssid, password);
  if (status != WL_CONNECTED) {
    // wait 10 seconds for connection:
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    status = WiFi.begin(ssid, password);
    delay(10000);

    Serial.print("Currently connected to SSID: ");
    Serial.println(WiFi.SSID());
    
    IPAddress ip = WiFi.localIP();
    Serial.print("IP Address: ");
    Serial.println(ip);
    Serial.println(ip);

    IPAddress gateway = WiFi.gatewayIP();
    Serial.print("Gateway: ");
    Serial.println(gateway);
    byte mac[6];
    WiFi.macAddress(mac);
    Serial.print("MAC address: ");
    Serial.print(mac[5], HEX);
    Serial.print(":");
    Serial.print(mac[4], HEX);
    Serial.print(":");
    Serial.print(mac[3], HEX);
    Serial.print(":");
    Serial.print(mac[2], HEX);
    Serial.print(":");
    Serial.print(mac[1], HEX);
    Serial.print(":");
    Serial.println(mac[0], HEX);

    IPAddress subnet = WiFi.subnetMask();
    Serial.print("SubnetMask: ");
    Serial.println(subnet);

    long rssi = WiFi.RSSI();
    Serial.print("signal strength (RSSI):");
    Serial.println(rssi);
  } 
  else {
    Serial.println(F("\n Scanning for WiFi..."));
    WiFi.mode(WIFI_STA);
  }
}

void loop() {
  String ssid;
  int32_t rssi;
  uint8_t encryptionType;
  uint8_t *bssid;
  int32_t channel;
  bool hidden;
  int scanResult;
  Serial.println(F("Starting WiFi scan..."));

  scanResult = WiFi.scanNetworks(/*async=*/false, /*hidden=*/true);

  if (scanResult == 0) {
    Serial.println(F("No networks found"));
  } else if (scanResult > 0) {
    Serial.printf(PSTR("%d networks found:\n"), scanResult);

    for (int8_t i = 0; i < scanResult; i++) {
      WiFi.getNetworkInfo(i, ssid, encryptionType, rssi, bssid, channel, hidden);

      const bss_info *bssInfo = WiFi.getScanInfoByIndex(i);
      String phyMode;
      const char *wps = "";
      if (bssInfo) {
        phyMode.reserve(12);
        phyMode = F("802.11");
        String slash;
        if (bssInfo->phy_11b) {
          phyMode += 'b';
          slash = '/';
        }
        if (bssInfo->phy_11g) {
          phyMode += slash + 'g';
          slash = '/';
        }
        if (bssInfo->phy_11n) {
          phyMode += slash + 'n';
        }
        if (bssInfo->wps) {
          wps = PSTR("WPS");
        }
      }
      Serial.printf(PSTR("  %02d: [CH %02d] [%02X:%02X:%02X:%02X:%02X:%02X] %ddBm %c %c %-11s %3S %s\n"), i, channel, bssid[0], bssid[1], bssid[2], bssid[3], bssid[4], bssid[5], rssi, (encryptionType == ENC_TYPE_NONE) ? ' ' : '*', hidden ? 'H' : 'V', phyMode.c_str(), wps, ssid.c_str());
      yield();
    }
  } else {
    Serial.printf(PSTR("WiFi scan error %d"), scanResult);
  }
  delay(20000);
}