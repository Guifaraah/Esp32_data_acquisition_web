/*********************************************************************************************
 * @file    ESP_APPEND_READ_GSHEET.ino
 * @author  Diego J. Stefanello - Drive Automação e Tecnologia LTDA
 * @e-mail  drive@driveat.com.br
 * @version V1.0
 * @date    17/04/2022
 * @brief   This sample code, shows how to append new values to spreadsheet
 *           and how to read the spreadsheet's values from ranges.
 *
 * Supporting material: 
 *                      based on code by K. Suwatchai (Mobizt) 
 *                      Github: https://github.com/mobizt/ESP-Google-Sheet-Client
 *  https://developers.google.com/sheets/api/reference/rest/v4/spreadsheets.values/append
 *  https://developers.google.com/sheets/api/reference/rest/v4/spreadsheets.values/get
 ********************************************************************************************/

#include <Arduino.h>
//#if defined(ESP8266)
//#include <ESP8266WiFi.h>
//#elif defined(ESP32)
#include <WiFi.h>
//#endif
#include <ESP_Google_Sheet_Client.h>

#define WIFI_SSID "AndroidAP6241"
#define WIFI_PASSWORD "izeh7566"



#define PROJECT_ID "103851781696215215580"  // ID Service Account
#define CLIENT_EMAIL "esp32-planilhas@valiant-ocean-364605.iam.gserviceaccount.com"  // Service Account's client email

//Service Account's private key
const char PRIVATE_KEY[] PROGMEM = "-----BEGIN PRIVATE KEY-----\nMIIEvgIBADANBgkqhkiG9w0BAQEFAASCBKgwggSkAgEAAoIBAQC9SGQrFbs4vDi9\ncdYBhte+y+NHLqjG34wGGbGpCITgHXuPM0g/dZh5BPJyJAFP+jPLVGCx+979fy22\nxq0Hh6fQJeEN79QUliuNxdQC+pyho1HvuWenElIVR4zHU0z6MwRx7dY2ZILPqeRl\nYke/ND4pQ2AxIUf9iP9VOLSOcsCpeTToco0qIfmkcWIsqPLoWhicMZelRLNsjz+X\n4v96kOW+hTHhlGpPE3awkSVbzrjkvRjDdPRY7OAj/f2x5ZqiipytBuWzr2rTTyzU\nnz2GEyBkL2hs4WO4n3CYSdoBNtYMOJCTXr6qpUonDUPKsrM6FHAf5dvQ2T5jCs2B\nckWr+EMVAgMBAAECggEAAhtl/UFTI9/wxkl+UNWikMhr2CDwo10IOHdMFqet0nK2\n5zKUnCx/z0ZlsIKl9iiNlh0d38Xd7WWsKXeRpA2eECO0VuikvWWo8ttmb2C4iLRb\n2TSR4jaPZVr6egED+zdr5y3U7VnxvtVgDFxWC7lNfOg8GJ7YSfnA5eD2DMcXK63m\nFZ10cIv99OzLVAhILlzKQpI7606wGTFhODS9UA6oEVFXI33BYScN++UTJrKWiuBq\nJAdzdnTMkXDTogJxPcpXyc9K/x4ztuiCuPGpIq/YkR087XfSCbj7pYEkGLdKDrvw\nYL1Q4URvb43O5b1LV7g6KndRd2x2ykGm/Rm5VZZ04QKBgQDqTiBI+AQHqubX/15r\nqUkWMrBTCKN19UxjqxJpiWrdJGWm3ZOdkrGkpg5UzaZmC+JnCKEE2U8aEnGSQdmL\nF2bZZhuk2zDFMfQBbIy+XsnEbhC4f1l3qtSL0owpe0w0VvSFQ6hWQVn6OiAtM/e2\nzlquyf9moIHAGLECkkM/XxLKzQKBgQDOzxKXnjWVYyKg6BCZpE1boHiamutrmAI3\nnGaXxJIL4uAmJzmNUuMGLjaCx7Ref3sls6Ab7sk7KikoHt367AUB2VwmehPFpC2Q\nYQleWaMIY4gb7IUT3fHPAj37Syfo4zxy2U5kOssZv7at5VZFB94OVs50sc4MjbXg\n/OxW8mlpaQKBgAxc5vMysvO8nsWS4eaIXiTfs9INyKI2uTshEa2j5JGalAqD8oAD\n93+ai3MKDteflOie2fApjzTu+DMhalsedvzNmHPQZPhitF9G3P5GfhrxO8ygW5Ym\n22QtTdv6d4Djfa9ybHVwqVC8tGnuLgnlRX+nhORjX7VFGEHsecPkPleZAoGBAIrx\nxr0E910xqK9Qmu6O28ZmrwC4etjj3QF929O1pqXDvPMFQX54pLyfSdJd2AnxPkyb\nPUPnD13nupUQ//S4o7iVT3suY6BS6KXq+/9e29EkBes04kfxwqdhC0GNQfgOy6sh\n4rAmbBQVoOd+gJKKcZ9SpQzz9MqsXvkVFLhpz3aZAoGBAJkeNhByWdNh5K/nRaCd\naBs2TxT6vpLz8Rk5VfmZFXt7iNQkx5RyEEvPEAk2e3lZC6TrD02ke5KOD7HkUUdE\nXwHrGS3BDrPNXacGKtqBFn+ibpnzNrCz0Sp3d9IV3ARoZ5pW19GKO4EIgVkdG2RL\nrxlqIkz8VeEy0GLPn4rpU/C6\n-----END PRIVATE KEY-----\n";

const char SPREADSHEET_ID[] PROGMEM = "1mTJg_6_E-NobKLm82I4si0kvD8K6Vyi_tPN-_sCxc9Q/edit#gid=0";
void tokenStatusCallback(TokenInfo info); // Callback function for debug



/*******************************************************
* @brief : Setup 
* @param : None
* @retval: None 
******************************************************/
void setup(void){
   
   Serial.begin(115200);
   delay(1000);
   Serial.println();
   
   Serial.printf("ESP Google Sheet Client v%s\n\n", ESP_GOOGLE_SHEET_CLIENT_VERSION);
   
   WiFi.setAutoReconnect(true);
   WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
   Serial.print("Connecting to Wi-Fi");
   while(WiFi.status() != WL_CONNECTED){
      Serial.print(".");
      delay(300);
   }
   Serial.println();
   Serial.print("Connected with IP: ");
   Serial.println(WiFi.localIP());
   Serial.println();

   GSheet.setTokenCallback(tokenStatusCallback);         // Set the callback for Google API access token generation status 
   GSheet.begin(CLIENT_EMAIL, PROJECT_ID, PRIVATE_KEY);  // Begin the access token generation for Google API authentication
}


/*******************************************************
* @brief : Main loop
* @param : None
* @retval: None 
******************************************************/
void loop(void){
    
   static unsigned long ms;
   static unsigned long interval = 25000;
   bool ready = GSheet.ready();              // Call ready() repeatedly in loop for authentication checking and processing

   if(ready && ((millis() > ms))){
      ms = millis() + interval;
      
      FirebaseJson response;
      Serial.println("\nAppend spreadsheet values:");

      FirebaseJson valueRange;
      
      valueRange.add("majorDimension", "COLUMNS");
      uint8_t aux = random(20, 28);
      valueRange.set("values/[0]/[0]", aux);
      aux = random(35, 96);
      valueRange.set("values/[1]/[0]", aux);
           
      bool success = GSheet.values.append(&response /* returned response */, SPREADSHEET_ID /* spreadsheet Id to append */, "Log1!A7" /* range to append */, &valueRange /* data range to append */);
      response.toString(Serial, true);
      Serial.println();

      Serial.println("\nGet spreadsheet values from range...");
      
      success = GSheet.values.get(&response /* returned response */, SPREADSHEET_ID /* spreadsheet Id to read */, "Log1!B3" /* range to read */);
      response.toString(Serial, true);   
      
      FirebaseJsonData result;

      response.get(result, "values/[0]/[0]");
      if(result.success)
      {
         //Print its content e.g.string, int, double, bool whereas object, array and null also can access as string
         interval = result.to<String>().toInt() * 1000;
         Serial.printf("\nWaiting %ldms to next event!\n", interval);
      }else Serial.println("Read failure :(");  
      Serial.println("--------------------------------------------");  
   }
}


/*******************************************************
* @brief : Status Callback
* @param : None
* @retval: None 
******************************************************/
void tokenStatusCallback(TokenInfo info){
   
   Serial.printf("Token info: type = %s, status = %s\n", GSheet.getTokenType(info).c_str(), GSheet.getTokenStatus(info).c_str());
   if(info.status == esp_signer_token_status_error) Serial.printf("Token error: %s\n", GSheet.getTokenError(info).c_str());
}
