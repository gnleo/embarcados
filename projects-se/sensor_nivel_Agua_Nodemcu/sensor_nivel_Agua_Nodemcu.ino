#include <ESP8266WiFi.h>

// mapeamento dos sensores
#define sensor1 5 // GPIO16 - D1
#define sensor2 4 // GPIO5 - D2
#define sensor3 14 // GPIO14 - D5
#define sensor4 12 // GPIO12 - D6
#define sensor5 13 // GPIO13 - D7

int leitura_sensor1 = -1;
int leitura_sensor2 = -1;
int leitura_sensor3 = -1;
int leitura_sensor4 = -1;
int leitura_sensor5 = -1;

// configuracao de rede wifi
const char* rede = "CAMTUC-LIVRE";
const char* senha = "";

// configuracao de requisicao web (GET)
const int httpPort = 80;
const char* host = "intnovate.org";
String url1 = "/sensor/niveis.php?id=1&nivel=1";
String url2 = "/sensor/niveis.php?id=1&nivel=2";
String url3 = "/sensor/niveis.php?id=1&nivel=3";
String url4 = "/sensor/niveis.php?id=1&nivel=4";
String url5 = "/sensor/niveis.php?id=1&nivel=5";

void setup() {
  // put your setup code here, to run once:
  
  pinMode(sensor1, INPUT);
  pinMode(sensor2, INPUT);
  pinMode(sensor3, INPUT);
  pinMode(sensor4, INPUT);
  pinMode(sensor5, INPUT);
  
  Serial.begin(115200); // inicializa velocidade de transmissao serial
  WiFi.begin(rede, senha); // inicializa modulo de rede 
  delay(2500);

  Serial.print("CONECTANDO A REDE ");
  Serial.println(rede);
  Serial.print("Carregando");
    
   while(WiFi.status() != WL_CONNECTED){ // aguarda conexao a rede wifi
    delay(500);
    Serial.print(".");
  }
  // imprime informacoes de rede
  Serial.println("\n");
  Serial.println("WiFi Conectado");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

}

void loop() {

  WiFiClient client; // inicializa cliente
  
  if(!client.connect(host, httpPort)){ // executa tentativa de conexao cliente / servidor
    Serial.println("Falha de conexao");
    return;
  }

  // executa leitura dos sensores 
  leitura_sensor1 = digitalRead(sensor1);
  leitura_sensor2 = digitalRead(sensor2);
  leitura_sensor3 = digitalRead(sensor3);
  leitura_sensor4 = digitalRead(sensor4);
  leitura_sensor5 = digitalRead(sensor5);

  // executa requisicao ao servidor de acordo com a leitura do sensor
  if(leitura_sensor1 == HIGH && leitura_sensor2 == HIGH && leitura_sensor3 == HIGH && leitura_sensor4 == HIGH && leitura_sensor5 == HIGH){
    Serial.println("Nivel 5");
    delay(1000);
    client.print(String("GET ") + url5 + " HTTP/1.1\r\n" + "Host: " + host + "\r\n" + "Connection: close\r\n\r\n");
    
    unsigned long timeout = millis(); // controle de tempo de resposta do servidor
    while(client.available() == 0){
      if(millis() - timeout > 5000){
        Serial.println(">>> Tempo de limite atingido!");
        client.stop();
        return;
      }
    }
  } else{    
    if(leitura_sensor1 == HIGH && leitura_sensor2 == HIGH && leitura_sensor3 == HIGH && leitura_sensor4 == HIGH){
      Serial.println("Nivel 4");
      delay(1000);
      client.print(String("GET ") + url4 + " HTTP/1.1\r\n" + "Host: " + host + "\r\n" + "Connection: close\r\n\r\n");
    
      unsigned long timeout = millis(); // controle de tempo de resposta do servidor
      while(client.available() == 0){
        if(millis() - timeout > 5000){
          Serial.println(">>> Tempo de limite atingido!");
          client.stop();
          return;
        }
      }      
    } else{
      if(leitura_sensor1 == HIGH && leitura_sensor2 == HIGH && leitura_sensor3 == HIGH){
        Serial.println("Nivel 3");
        delay(1000);
        client.print(String("GET ") + url3 + " HTTP/1.1\r\n" + "Host: " + host + "\r\n" + "Connection: close\r\n\r\n");
    
        unsigned long timeout = millis(); // controle de tempo de resposta do servidor
        while(client.available() == 0){
          if(millis() - timeout > 5000){
            Serial.println(">>> Tempo de limite atingido!");
            client.stop();
            return;
          }
        }        
      } else{
        if(leitura_sensor1 == HIGH && leitura_sensor2 == HIGH){
          Serial.println("Nivel 2");
          delay(1000);
          client.print(String("GET ") + url2 + " HTTP/1.1\r\n" + "Host: " + host + "\r\n" + "Connection: close\r\n\r\n");
    
          unsigned long timeout = millis(); // controle de tempo de resposta do servidor
          while(client.available() == 0){
            if(millis() - timeout > 5000){
              Serial.println(">>> Tempo de limite atingido!");
              client.stop();
              return;
            }
          }          
        } else{          
          if(leitura_sensor1 == HIGH){
            Serial.println("Nivel 1");
            delay(1000);
            client.print(String("GET ") + url1 + " HTTP/1.1\r\n" + "Host: " + host + "\r\n" + "Connection: close\r\n\r\n");
    
            unsigned long timeout = millis(); // controle de tempo de resposta do servidor
            while(client.available() == 0){
              if(millis() - timeout > 5000){
                Serial.println(">>> Tempo de limite atingido!");
                client.stop();
                return;
              }
            }
          } else {
            Serial.println("");
            Serial.println("");
            Serial.println("Nenhum sensor foi atingido pelo nivel de agua");
            delay(1000);
          } // fim 'se nao' do sensor 1                
        } // fim 'se nao' do sensor 2
      } // fim 'se nao' do sensor 3
    } // fim 'se nao' do sensor 4
  } // fim 'se nao' do sensor 5
}

