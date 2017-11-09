#include <ESP8266WiFi.h>

//------------------------------------------Protótipo da função-----------------------------------------------------------
int Subir(int val, int op);

//--------------------------------------------------//--------------------------------------------------------------------
WiFiClientSecure client;//Cria um cliente seguro (para ter acesso ao HTTPS)
WiFiServer server(80);
//------------------------------------------Declaração das variáveis dos setores------------------------------------------
int primeiroandar = 5;
int garagem = 14;
int sala = 4;
int jardim = 2;
int portao = 12;

void setup() {
//-----------------------------------------Pinos como saída-----------------------------------------------------------------
pinMode(primeiroandar, OUTPUT);
pinMode(garagem, OUTPUT);
pinMode(sala, OUTPUT);
pinMode(jardim, OUTPUT);
pinMode(portao, OUTPUT);
//-------------------------------------------------------------------------------------------------------------------------
Serial.begin(115200);//Inicia a comunicacao serial
WiFi.mode(WIFI_AP_STA);//Habilita o modo estaçao
WiFi.begin("Kamuy", "cgla48123456789");//Conecta na rede
server.begin();//Iniciando servidor.
}
void loop() {
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

  // Liga desliga------------------------------------------------------------------------------------------------------
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

  // Manda request
  digitalWrite(sala, val);
  digitalWrite(garagem, val);
  digitalWrite(primeiroandar, val);
  digitalWrite(jardim, val);
  digitalWrite(portao, val);
  client.flush();
//-------------------------------------------------//-----------------------------------------------------------------------
 // Preparando response
  String s = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<!DOCTYPE HTML>\r\n<html>\r\nGPIO is now ";
  s += (val)?"high":"low";
  s += "</html>\n";

  // Send the response to the client
  client.print(s);
  delay(1);
  Serial.println("Client disonnected");
  delay(1000);
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
