#include "wifi_connection.h"
//DateTime variables
const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = 2*3600;
const int   daylightOffset_sec = 3600;
//server variables
const uint16_t port = PORT;
const char * host = HOST;

WiFiMulti wifiMulti;


void WIFIInit() {
  wifiMulti.addAP(SSID, PASSWORD);
  Serial.println("Connecting Wifi...");
  if (wifiMulti.run() == WL_CONNECTED) {
    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
  }
   configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
}
String getLocalTime(){
  String s = "";
  struct tm timeinfo;
  if(!getLocalTime(&timeinfo)){
    Serial.println("Failed to obtain time");
    return s;
  }
  s = asctime(&timeinfo);
  for(int i = 0; i<s.length(); i++){
    if(s[i]==' ') s.setCharAt(i, '_');
  }
  s.setCharAt(s.length()-1, '_');
  return s;
}
bool is_header = false;
static const size_t bufferSize = 2048;
static uint8_t buffer[bufferSize] = {0xFF};
char str[8];
byte buf[256];
static int i = 0;
static int k = 0;
uint8_t temp = 0, temp_last = 0;

int postImage()
{
  //Check WiFi connection status
  if (WiFi.status() == WL_CONNECTED) {
    WiFiClient client;
    Serial.print("Connecting to ");
    Serial.println(host);
    if (!client.connect(host, port)) {
      Serial.println("Connection failed.");
      Serial.println("Waiting 5 seconds before retrying...");
      delay(5000);
      return 0;
    }
    
    start_capture();
    while (!(*getCamera()).get_bit(ARDUCHIP_TRIG, CAP_DONE_MASK));
    String head = "--Stefan\r\nContent-Disposition: form-data; name=\"file\"; filename=\"";
    String formattedDate = getLocalTime();
    String devId = "dev ";
    devId[devId.length()-1] = DEVICE_ID+'0';
    String head2 = ".jpg\"\r\nContent-Type: image/jpeg\r\n\r\n";
    head.concat(formattedDate);
    head.concat(devId);
    head.concat(head2);
    String tail = "\r\n--Stefan--\r\n";
    uint32_t len = (*getCamera()).read_fifo_length();
    Serial.print(F("The fifo length is :"));
    Serial.println(len, DEC);
    if (len >= MAX_FIFO_SIZE) {
      Serial.println("Over size.");
      return 0;
    } else if (len == 0 ) {
      Serial.println("Size is 0.");
      return 0;
    }
    uint32_t imageLen = len;
    uint32_t extraLen = head.length() + tail.length();
    uint32_t totalLen = imageLen + extraLen;
    client.println("POST /uploads HTTP/1.1");//POST /uploads HTTP/1.1
    client.println("Host: "+String(host)+":"+String(port));
    client.println("Content-Length: " + String(totalLen));
    client.println("Content-Type: multipart/form-data; boundary=Stefan");
    client.println();
    client.print(head);
    Serial.println(head);
    (*getCamera()).CS_LOW();
    (*getCamera()).set_fifo_burst();
    i = 0;
    while (len) {
      size_t will_copy = (len < bufferSize) ? len : bufferSize;
      SPI.transferBytes(&buffer[0], &buffer[0], will_copy);
      if (!client.connected()) break;
      client.write(&buffer[0], will_copy);
      len -= will_copy;
    }
    (*getCamera()).CS_HIGH();
    client.print(tail);
    // Free resources
    client.stop();
    return 1;
  }
  else {
    Serial.println("WiFi Disconnected");
    return 0;
  }
}