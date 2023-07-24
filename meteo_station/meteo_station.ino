// Libraries
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include <Wire.h>

// Definitions
# define Hall sensor 2
#define SEALEVELPRESSURE_HPA (1013.25)
 
// Variable definitions
Adafruit_BME280 bme;
float temperatura, umidade, pressao, altitude;
int pin = A0;
float valor = 0;
int Winddir = 0;
String direcao;
const float pi = 3.14159265;
int period = 5000;
int delaytime = 2000;
int radius = 147;               // Anemometer radius
unsigned int Sample = 0;
unsigned int counter = 0;       // magnet counter for sensor
unsigned int RPM = 0; 
float speedwind = 0;
float windspeed = 0;

void setup()
{
  bmp.begin(0x76)
  pinMode(2, INPUT);
  digitalWrite(2, HIGH);
  Serial.begin(9600); 
}

void loop()
{
  // Anemometer
  Sample++;
  Serial.print(Sample);
  Serial.print(": Iniciar medição...");
  windvelocity();
  Serial.println("   finalizada.");
  Serial.print("Counter: ");
  Serial.print(counter);
  Serial.print(";  RPM: ");
  RPMcalc();
  Serial.print(RPM);
  Serial.print(";  Wind speed: ");
  WindSpeed();
  Serial.print(windspeed);
  Serial.print(" [m/s] ");              
  SpeedWind();
  Serial.print(speedwind);
  Serial.print(" [km/h] ");  
  Serial.println();
  delay(delaytime);

 // Indicador de Direção do Vento
 valor = analogRead(pin)* (5.0 / 1023.0);
 
 Serial.print("leitura do sensor :");
 Serial.print(valor); 
 Serial.println(" volt");

 if (valor <= 0.27) {
  Winddir = 315;
  direcao = "Noroeste";
 }
 else if (valor <= 0.32) { 
 Winddir = 270;
 direcao = "Oeste";
 }
 else if (valor <= 0.38) { 
 Winddir = 225;
 direcao = "Sudoeste";
 }
 else if (valor <= 0.45) { 
 Winddir = 180;
 direcao = "Sul";
 }
 else if (valor <= 0.57) { 
 Winddir = 135;
 direcao = "Sudeste";
 }
 else if (valor <= 0.75) { 
 Winddir = 90;
 direcao = "Leste";
 }
 else if (valor <= 1.25) {  
 Winddir = 45;
 direcao = "Nordeste";
 }
 else {  
 Winddir = 000;
 direcao = "Norte";
 }
 Serial.print("Direção a :");
 Serial.print(Winddir);
 Serial.print(" graus");
 Serial.print(" / ");
 Serial.print(direcao);
 Serial.println("\n");
 delay (1000);

 // Indicadores Adafruit
 Serial.print("Temperatura :");
 Bme280()
 Serial.print(temperatura);
 Serial.println("\n");
 Serial.print("Umidade");
 Serial.print(umidade);
 Serial.println("\n");
 Serial.print("Pressão");
 Serial.print(pressao);
 Serial.println("\n");
 Serial.print("altitude");
 Serial.print(altitude);
 Serial.println("\n");
 delay (1000);
}


// Measure wind speed
void windvelocity(){
  speedwind = 0;
  windspeed = 0;
  
  counter = 0;  
  attachInterrupt(0, addcount, RISING);
  unsigned long millis();       
  long startTime = millis();
  while(millis() < startTime + period) {
  }
}

void RPMcalc(){
  RPM=((counter)*60)/(period/1000);  // Calculate revolutions per minute (RPM)
}

void WindSpeed(){
  windspeed = ((4 * pi * radius * RPM)/60) / 1000;  // Calculate wind speed on m/s
}

void SpeedWind(){
  speedwind = (((4 * pi * radius * RPM)/60) / 1000)*3.6;  // Calculate wind speed on km/h
}

void addcount(){
  counter++;
} 

//Adafruit_BME280 sensor
void Bme280(){
  temperatura = bme.readTemperature();
  umidade = bme.readHumidity();
  pressao = bme.readPressure() / 100.0F;
  altitude = bme.readAltitude(SEALEVELPRESSURE_HPA);
}

/*
Everything is beeing printed in serial, if an ESP module is integrated, it is possible
to turn it on a local server

 #include <WiFi.h>
 #include <WebServer.h>
 // Nome da rede e senha para conexão
 const char* ssid     = "NOME DA REDE";              // Inserir dados de Rede
 const char* senha    = "SENHA";                     // Inserir dados de Rede
 
 WebServer server(80);
 
 // Informações de acesso para rede de internet / IP Fixo
 IPAddress local_IP(192, 168, 0, 100);
 IPAddress gateway(192, 168, 0, 1);
 IPAddress subnet(255, 255, 255, 0);
 
 IPAddress primaryDNS(8, 8, 8, 8);
 IPAddress secondaryDNS(8, 8, 4, 4);
 
 void setup() {
    if (!WiFi.config(local_IP, gateway, subnet, primaryDNS, secondaryDNS)) {
      Serial.println("STA Failed to configure");
    }
 
    Serial.println("Conectando a ");
    Serial.println(ssid);
 
    //Conecta à Rede Wifi indicada anteriormente
    WiFi.begin(ssid, senha);
 
    while (WiFi.status() != WL_CONNECTED) {
      delay(1000);
      Serial.print(".");
    }
 
    Serial.println("");
    Serial.println("WiFi conectado ..!");
    Serial.print("IP obtido: ");
    Serial.println(WiFi.localIP());
 
    server.on("/", handle_OnConnect);
    server.onNotFound(handle_NotFound);
 
    server.begin();
    Serial.println("Servidor HTTP iniciado");
}
 
void loop() {
  server.handleClient();
}
 
void handle_OnConnect() {
  server.send(200, "text/html", SendHTML(temperatura, umidade, pressao, altitude, windspeed, winddir));
}
 
void handle_NotFound() {
  server.send(404, "text/plain", "Not found");
}
 
// Informações da página Web criada
String SendHTML(float temperatura, float umidade, float pressao, float altitude, float windspeed, char winddir) {
  String ptr = "<!DOCTYPE html> <html>\n";
  ptr += "<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\">\n";
 
// Título da Guia da Web.
  ptr += "<title>Meteo Station</title>\n";
 
  // Configurações de cor e padrões de exibição
  ptr += "<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}\n";
  ptr += "body{margin-top: 50px;} h1 {color: #444444;margin: 50px auto 30px;}\n";
  ptr += "p {font-size: 24px;color: #444444;margin-bottom: 10px;}\n";
  ptr += "</style>\n";
  ptr += "</head>\n";
  ptr += "<body>\n";
  ptr += "<div id=\"webpage\">\n";
 
  // Título impresso na Página Web Criada
  ptr += "<h1>Meteo Station</h1>\n";
 
  // Informações de vento
  ptr += "<p>Vento: ";
  ptr += windspeed;
  ptr += winddir;

  // Informações de Temperatura
  ptr += "<p>Temperatura: ";
  ptr += temperatura;
  ptr += "&deg;C</p>";
 
  // Informações de Umidade
  ptr += "<p>Umidade: ";
  ptr += umidade;
  ptr += "%</p>";
 
  // Informações de Pressão
  ptr += "<p>Pressao: ";
  ptr += pressao;
  ptr += "hPa</p>";
 
  // Informações de altitude
 
  ptr += "<p>Altitude: ";
  ptr += altitude;
  ptr += "m</p>";
  ptr += "</div>\n";
  ptr += "</body>\n";
  ptr += "</html>\n";
  return ptr;
}
*/
