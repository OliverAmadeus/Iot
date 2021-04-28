#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>
//------------------VARIABLES GLOBALES-------------------------
//Nombre de la red WiFi y Contraseña
const char* ssid = "SATCO-01D0CA"; 

const char* password = "AEEAF7AD";

const char*  host= "192.168.0.158";

 void setup() {
 
  // Inicia Serial
  Serial.begin(115200);
  delay(10);

  Serial.println();
  Serial.println();
  Serial.print("Conectando con "); 
  Serial.println(ssid);
  
   // Conexión WIFI
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
      
      Serial.println("");
      Serial.println("WiFi conectado");
      Serial.println("IP Address: ");
      Serial.println(WiFi.localIP());
  
      
  }
 }
int value = 0;
//-------------------------LOOP-------------------------------
void loop() {
  delay(2000);
  value = random(-15,50);

  Serial.println("Conectando con: ");
  Serial.println(host);

//------Función para Enviar Datos a la Base de Datos SQL-------
  WiFiClient client;
  const int httpPort =80;
  if (!client.connect(host, httpPort)) {
    Serial.println("Fallo de conexion");
    return;
  }

  String url = "http://192.168.0.158/entrada_datos.php";
  String data = "serie=777&temp=" + String(value);
  
//imrimir url de la solicitud
  Serial.println("Requesting URL: ");
  Serial.println(url);

  client.print(String("POST ") + url + " HTTP/1.1" + "\r\n" + 
               "Host: " + host + "\r\n" +
               "Accept: */*" + "*\r\n" +
               "Content-Length: " + data.length() + "\r\n" +
               "Content-Type: application/x-www-form-urlencoded" + "\r\n" +
               "\r\n" + data);           
  delay(10);             
  
  Serial.print("Enviando datos a SQL...");
  
  unsigned long timeout = millis();
  while (client.available() == 0) {
    if (millis() - timeout > 5000) {
      Serial.println("Cliente fuera de tiempo!");
      client.stop();
      
    }
  }
  // Lee todas las lineas que recibe del servidro y las imprime por la terminal serial
  while(client.available()){
    String line = client.readStringUntil('\r');
    Serial.println(line);
  }  
  Serial.println();


  Serial.println("Cerrando Conexión");
}
