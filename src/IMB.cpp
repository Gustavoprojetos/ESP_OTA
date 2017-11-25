//Inclui as fun��es de Wifi do ESP
#include <ESP8266WiFi.h>
#include <DNSServer.h>
#include <WiFiManager.h>
#include <ESP8266WebServer.h>
//Cria um server na porta 80 (porta
//padr�o para onde os navegadores
//enviam as requisi��es http)
//------------------------------------------Prot�tipo da fun��o-----------------------------------------------------------
int Subir(int val, int op);

//--------------------------------------------------//--------------------------------------------------------------------
WiFiClientSecure client;//Cria um cliente seguro (para ter acesso ao HTTPS)
WiFiServer server(80);

void setup()
{
 //Inicializa a Serial apenas para efeito de log
 Serial.begin(115200);
 //Configura o GPIO0 e GPIO2 como output, ou seja, como
 //sa�da para podermos alterar o seu valor
 pinMode(0, OUTPUT);
 pinMode(2, OUTPUT);
 //Deixa o GPIO0 e o GPIO2 com sa�da LOW
 digitalWrite(0, LOW);
 digitalWrite(2, LOW);


//Enquanto o ESP n�o se conectar � rede
/*while (WiFi.status() != WL_CONNECTED)
{
 //Esperamos 100 milisegundos

 delay(100);

 Serial.print(".");
}
*/
//--------------------------------------------------------Autoconxe��o com a rede--------------------------------------------
  WiFiManager wifiManager;
      //set custom ip for portal
      //wifiManager.setAPConfig(IPAddress(10,0,1,1), IPAddress(10,0,1,1), IPAddress(255,255,255,0));
      wifiManager.autoConnect("Automap");
      Serial.println("Conectando:)");
  //-------------------------------------------------------------------------------------------------------------------------

//Se chegou aqui � porque conectou � rede, ent�o mostramos
//no monitor serial para termos um feedback
Serial.println("");
Serial.println("Conectou");
//Configura��es do IP fixo. Voc� pode alterar
//conforme a sua rede
IPAddress ip(192, 168, 0, 8);
IPAddress gateway(192, 168, 0, 1);
IPAddress subnet(255, 255, 255, 0);
Serial.print("Configurando IP fixo para : ");
Serial.println(ip);
//Envia a configura��o
WiFi.mode(WIFI_AP_STA);
WiFi.config(ip, gateway, subnet);
//Inicializa o server que criamos na porta 80
server.begin();
//Mostramos no monitor serial o IP que o ESP possui para verificarmos se
//� o mesmo que configuramos
Serial.print("Server em: ");
Serial.println(WiFi.localIP());
}
void loop()
{

 //Verifica se algum cliente est� tentando se conectar

 WiFiClient client = server.available();

 if (!client) {
 //Se n�o houver nenhum cliente podemos retornar pois n�o h� nada a fazer
  return;
 }
 Serial.println("Novo cliente conectou");
 //Esperamos at� que o cliente nos envie a requisi��o
 while (!client.available()){
   delay(100);
 }
 //Fazemos a leitura da requisi��o
 String req = client.readStringUntil('\r');
 Serial.print("Requisi��o: ");
 Serial.println(req);

 //Este � o html que iremos retornar para o cliente
//� composto basicamente de dois bot�es (ON e OFF) para o GPIO0 e dois bot�es (ON e OFF) para o GPIO2
//A parte que nos interessa � o <a href=' com a a��o vinculada a cada bot�o
//Quando clicamos em um destes bot�es essa informa��o chegar� at� o ESP para que ele verifique qual a��o deve executar
//A parte dentro de '<style>' � apenas para modificarmos o visual da p�gina que ser� exibida, voc� pode alter�-la como queira
String html =
"<html>"
"<head>"
"<meta name='viewport' content='width=device-width, initial-scale=1, user-scalable=no'/>"
"<title>ESP8266</title>"
"<style>"
"body{"
"text-align: center;"
"font-family: sans-serif;"
"font-size:14px;"
"padding: 25px;"
"}"
"p{"
"color:#444;"
"}"
"button{"
 "outline: none;"
 "border: 2px solid #1fa3ec;"
 "border-radius:18px;"
 "background-color:#FFF;"
 "color: #1fa3ec;"
 "padding: 10px 50px;"
 "}"
 "button:active{"
 "color: #fff;"
 "background-color:#1fa3ec;"
 "}"
 "</style>"
 "</head>"
 "<body>"
 "<p><h1>Controle de cargar</h1></p>"
 "<h2>Abajur</h2>"
 "<p><a href='?acao=gpio2On'><button>ON</button></a></p>"
 "<p><a href='?acao=gpio2Off'><button>OFF</button></a></p>"
 "</body>"
 "</html>";

 int val;
 int op;
 //Escreve o html no buffer que ser� enviado para o cliente
client.print(html);
//Envia os dados do buffer para o cliente
client.flush();
//Verifica se a requisi��o possui a a��o gpio0On
if (req.indexOf("acao=gpio0On") != -1)
{
 //Se possui a a��o gpio0On colocamos a sa�da do GPIO0 como alta
 val = 1;
 op = 1;
 digitalWrite(0, HIGH);
 Subir(val, op);
}
//Sen�o, verifica se a requisi��o possui a a��o gpio0Off
else if (req.indexOf("acao=gpio0Off") != -1)
{
 //Se possui a a��o gpio0Off colocamos a sa�da do GPIO0 como baixa
 val = 0;
 op = 2;
 digitalWrite(0, LOW);
 Subir(val, op);
}
//Sen�o, verifica se a requisi��o possui a a��o gpio2On
else if (req.indexOf("acao=gpio2On") != -1)
{
 //Se possui a a��o gpio2On colocamos a sa�da do GPIO2 como alta
 val = 1;
 op = 1;
 digitalWrite(2, val);
 Subir(val, op);
}
//Sen�o, verifica se a requisi��o possui a a��o gpio2Off
else if (req.indexOf("acao=gpio2Off") != -1)
{
 //Se possui a a��o gpio0Off colocamos a sa�da do GPIO2 como baixa
 val = 0;
 op = 2;
 digitalWrite(2, val);
 Subir(val, op);
}

//Fecha a conex�o com o cliente
client.stop();

Serial.println("Cliente desconectado");
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
