#include "../enclosure_controller.h"
#include "../secrets.h"

void EnclosureController::_wifi_init()
{
  WiFi.disconnect(true, true);
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  WiFi.setTxPower(WIFI_POWER_8_5dBm);
  if (WiFi.waitForConnectResult() != WL_CONNECTED)
  {
    WiFi.begin(WIFI_SSID, WIFI_PASS);

    uint8_t wifiAttempts = 0;
    while (WiFi.status() != WL_CONNECTED && wifiAttempts < 20)
    {
      Serial.print(".");
      delay(1000);
      if (wifiAttempts == 10)
      {
        WiFi.disconnect(true, true);
        WiFi.begin(WIFI_SSID, WIFI_PASS);
      }
      wifiAttempts++;
    }

    log_w("Connection Failed! Rebooting...");
    delay(5000);
    ESP.restart();
  }

  log_i("IP: %s", WiFi.localIP().toString().c_str());
  _wifi_inited = true;
}

bool EnclosureController::_wifi_check()
{
  if (!_wifi_inited)
  {
    return false;
  }

  try
  {
    if (WiFi.status() != WL_CONNECTED)
    {
      log_w("Reconnecting to WiFi...");
      WiFi.disconnect();
      WiFi.reconnect();
      //wifiDisconnects++;
      log_w("Reconnected to WiFi");
    }
  }
  catch (const std::exception &e)
  {
    log_e("Wifi error: %s", e.what());
    return false;
  }

  return true;
}