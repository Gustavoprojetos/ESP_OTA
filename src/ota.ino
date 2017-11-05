#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>


int Subir(int val);
WiFiClientSecure client;//Cria um cliente seguro (para ter acesso ao HTTPS)
//String textFix = "GET /forms/d/e/1FAIpQLSdm6M_0mTVx_LKHLB1J3u_hjaag_hBtMfDHQlTIKe0EoatfsQ/formResponse?ifq&entry.717212213=";
//2
//String textFix = "GET /forms/d/e/1FAIpQLSe_NPbrA1Hke7zBn2vH45rn8WI2DuS-ZRw2gupkyisq1kURXw/formResponse?ifq&entry.1129360522=";
//Essa String sera uma auxiliar contendo o link utilizado pelo GET, para nao precisar ficar re-escrevendo toda hora
WiFiServer server(80);


void setup() {
Serial.begin(115200);//Inicia a comunicacao serial
Serial.println("Booting");
WiFi.mode(WIFI_AP_STA);//Habilita o modo estaçao
WiFi.begin("Kamuy", "cgla48123456789");//Conecta na rede
//WiFi.begin("asusk", "12345678");//Conecta na rede
while (WiFi.waitForConnectResult() != WL_CONNECTED) {
  Serial.println("Connection Failed! Rebooting...");
  delay(5000);
  ESP.restart();
}
ArduinoOTA.onStart([]() {
  String type;
  if (ArduinoOTA.getCommand() == U_FLASH)
    type = "sketch";
  else // U_SPIFFS
    type = "filesystem";

  // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
  Serial.println("Start updating " + type);
});
ArduinoOTA.onEnd([]() {
  Serial.println("\nEnd");
});
ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
  Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
});
ArduinoOTA.onError([](ota_error_t error) {
  Serial.printf("Error[%u]: ", error);
  if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
  else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
  else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
  else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
  else if (error == OTA_END_ERROR) Serial.println("End Failed");
});
ArduinoOTA.begin();
Serial.println("Ready");
Serial.print("IP address: ");
Serial.println(WiFi.localIP());
Serial.print("Atualizado: ");

// Iniciar servidor
 server.begin();

pinMode(14, OUTPUT);
}


void loop() {

  ArduinoOTA.handle();

  //--------------------------------------------------------------------------------------------------------------------
    // Check if a client has connected
    WiFiClient client = server.available();
    if (!client) {
      return;
    }

    // Wait until the client sends some data

    Serial.println("new client");
    while(!client.available()){
      delay(1);
    }

    // Read the first line of the request
    String req = client.readStringUntil('\r');
    Serial.println(req);
    client.flush();


    // Liga desliga
    int val;
    int op;
    if (req.indexOf("/gpio/1") != -1){
      val = 1;
      op = 1;
      Subir(val, op);
    }

    else if (req.indexOf("/gpio/0") != -1 ){
      val = 0;
      op = 2;
      Subir(val, op);
    }
    else {
      Serial.println("invalid request");
      client.stop();
      return;
    }

    // Manda request GPIO14
    digitalWrite(14, val);

    client.flush();

   // Preparando response
    String s = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<!DOCTYPE HTML>\r\n<html>\r\nGPIO is now ";
    s += (val)?"high":"low";
    s += "</html>\n";

    // Send the response to the client
    client.print(s);
    delay(1);
    Serial.println("Client disonnected");


delay(2000);

}
int Subir(int val, int op){
  String Ligar = "GET /forms/d/e/1FAIpQLSe_NPbrA1Hke7zBn2vH45rn8WI2DuS-ZRw2gupkyisq1kURXw/formResponse?ifq&entry.1129360522=";
  String Desligar = "GET /forms/d/e/1FAIpQLSe_NPbrA1Hke7zBn2vH45rn8WI2DuS-ZRw2gupkyisq1kURXw/formResponse?ifq&entry.381708124=";

  switch (op) {
    case 1:
      if (client.connect("docs.google.com", 443) == 1)//Tenta se conectar ao servidor do Google docs na porta 443 (HTTPS)
      {
        String toSend = Ligar;//Atribuimos a String auxiliar na nova String que sera enviada
        toSend += val;//Adicionamos um valor
        toSend += "&submit=Submit HTTP/1.1";//Completamos o metodo GET para nosso formulario.

        client.println(toSend);//Enviamos o GET ao servidor-
        client.println("Host: docs.google.com");//-
        client.println();//-
        client.stop();//Encerramos a conexao com o servidor
        Serial.println("Dados enviados.");//Mostra no monitor que foi enviado
      }
      else
        {
          Serial.println("Erro ao se conectar");//Se nao for possivel conectar no servidor, ira avisar no monitor.
        }
        break;

  case 2:
      if (client.connect("docs.google.com", 443) == 1)//Tenta se conectar ao servidor do Google docs na porta 443 (HTTPS)
      {
        String toSend = Desligar;//Atribuimos a String auxiliar na nova String que sera enviada
        toSend += val;//Adicionamos um valor
        toSend += "&submit=Submit HTTP/1.1";//Completamos o metodo GET para nosso formulario.

        client.println(toSend);//Enviamos o GET ao servidor-
        client.println("Host: docs.google.com");//-
        client.println();//-
        client.stop();//Encerramos a conexao com o servidor
        Serial.println("Dados enviados.");//Mostra no monitor que foi enviado
            }
            else
              {
                Serial.println("Erro ao se conectar");//Se nao for possivel conectar no servidor, ira avisar no monitor.
              }
              break;
            }
      }
