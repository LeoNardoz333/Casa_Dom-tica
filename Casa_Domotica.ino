//Pines usados: 33,4,2,12,14,13,25,22,26,27,5,1,3
//Librerias
#include <Arduino.h>
#include <WiFi.h>
#include <ESP32Servo.h>
#include <OneWire.h>
#include <DallasTemperature.h>
//Millis
unsigned long puertaAbierta;
unsigned long tendederoAv;
unsigned long tiempoActual;
//Ventiladores
const int oneWireBus = 22;
OneWire oneWire(oneWireBus);
DallasTemperature sensors(&oneWire);
float temp;
#define derecha1 26
#define izquierda1 27
//Tendedero
#define derecha3 1
#define izquierda3 3
#define pwm1 5
int canal1 = 0;
bool bTendedero = false;
bool detenido = false;
//Lavadora
#define derecha2 32
#define izquierda2 35
bool lavadora = false;
//Sensor de humedad ARD 365
#define pinSensorAwa 33
int humedad = 0;
//Relay
#define pinRelay 25
//Bomba de awa
const int umbral = 50;
//Sensor de sonido
#define pinSonido 4
bool state1 = false;
bool state2 = false; 
bool state3 = false;
bool stateLED = false;
bool soundDetected = false;
unsigned long tem;
//leds
#define led1 2
bool ledActivado = false;
//Puerta
#define trig 12
#define echo 14
#define servopin 13
Servo miServo;
int distancia = 0;
int duracion = 0;
bool cerrado=true;
//Wifi
const char* ssid = "RedPro";
const char* clave = "alfredito12";
bool activado = false;
WiFiServer server(80);

void setup() {
  //pinMode(pinBombaAwa,OUTPUT);
  pinMode(pinRelay,OUTPUT);
  pinMode(pinSonido,INPUT);
  pinMode(led1,OUTPUT);
  pinMode(trig,OUTPUT);
  pinMode(echo, INPUT);
  pinMode(derecha1, OUTPUT);
  pinMode(izquierda1, OUTPUT);
  pinMode(derecha2, OUTPUT);
  pinMode(izquierda2, OUTPUT);
  pinMode(derecha3, OUTPUT);
  pinMode(izquierda3, OUTPUT);
  
  ledcSetup(canal1,5000,8);
  ledcAttachPin(pwm1,canal1);

  WiFi.mode(WIFI_AP);
  WiFi.softAP(ssid, clave);
  server.begin();

  miServo.attach(servopin);

  puertaAbierta=millis();
  tendederoAv=0;

  Serial.print("Direccion ip del esp32 en modo Soft Access: ");
  Serial.println(WiFi.softAPIP());
  Serial.begin(115200);
  analogReadResolution(10);
}

void loop() {
  tiempoActual=millis();
  //wifi();
  imprimir();
  //condicion();

  //pruebas
  //aplausos();
  //bombaAwa();
  tendedero();
}

void imprimir()
{
  if(activado==true)
    Serial.print("activado |");
  humedad = analogRead(pinSensorAwa);
  soundDetected = digitalRead(pinSonido);
  temp = sensors.getTempCByIndex(0);
  sensors.requestTemperatures();
  Serial.print("Humedad: ");
  Serial.print(humedad);
  Serial.print("| Temperatura: ");
  Serial.print(temp);
  Serial.print("| Distancia: ");
  Serial.print(distancia);
  Serial.print("cm| Sonido: ");
  Serial.println(soundDetected);
  //delay(500);
}

void condicion()
{
  if(activado == true)
  {
    bombaAwa();
    aplausos();
    tendedero();
  }
  else
  {
    apagar();
  }
}

void wifi()
{
  WiFiClient cliente = server.available();
  if (cliente) {
    String lineaactual = "";
    while (cliente.connected()) {
      if (cliente.available()) {
        char c = cliente.read();
        if (c == '\n') {
          if (lineaactual.length() == 0) {
            cliente.println("Content-type:text/html");
            cliente.println();
            cliente.println("<html>");
            cliente.println("<head>");
            cliente.println("<meta charset=\"UTF-8\">");
            cliente.println("<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">");
            cliente.println("<title>Control de casa</title>");
            cliente.println("<style>");
            cliente.println("body {");
            cliente.println("background-color: rgb(39, 39, 39);");
            cliente.println("background-size: auto;");
            cliente.println("background-position: center;");
            cliente.println("background-repeat: inherit;");
            cliente.println("margin: 0;");
            cliente.println("padding: 0;");
            cliente.println("}");
            cliente.println(".titulo{");
            cliente.println("margin-top: 50px;");
            cliente.println("text-align: center;");
            cliente.println("color: white;");
            cliente.println("font-size: 50px;");
            cliente.println("font-family:'Franklin Gothic Medium', 'Arial Narrow', Arial, sans-serif;");
            cliente.println("}");
            cliente.println(".titulo2{");
            cliente.println("margin-top: 50px;");
            cliente.println("text-align: center;");
            cliente.println("color: white;");
            cliente.println("font-size: 30px;");
            cliente.println("font-family:'Franklin Gothic Medium', 'Arial Narrow', Arial, sans-serif;");
            cliente.println("}");
            cliente.println(".btn{");
            cliente.println("font-size: 25px;");
            cliente.println("font-family: 'Franklin Gothic Medium', 'Arial Narrow', Arial, sans-serif;");
            cliente.println("color: aliceblue;");
            cliente.println("border-radius: 5px;");
            cliente.println("border: 3px solid;");
            cliente.println("width: 100%;");
            cliente.println("padding-bottom: 20px;");
            cliente.println("padding-top: 20px;");
            cliente.println("margin: auto;");
            cliente.println("cursor: pointer;");
            cliente.println("}");
            cliente.println(".btn-verde{");
            cliente.println("background-color: rgb(81, 207, 115);");
            cliente.println("border-color: rgb(22, 126, 70);");
            cliente.println("}");
            cliente.println(".btn-verde:hover{");
            cliente.println("background-color: rgb(132, 255, 160);");
            cliente.println("}");
            cliente.println(".btn-magenta{");
            cliente.println("background-color: rgb(207, 81, 148);");
            cliente.println("border-color: rgb(126, 22, 76);");
            cliente.println("}");
            cliente.println(".btn-magenta:hover{");
            cliente.println("background-color: rgb(255, 132, 218);");
            cliente.println("}");
            cliente.println(".btn-azul{");
            cliente.println("background-color: rgb(92, 182, 247);");
            cliente.println("border-color: rgb(22, 64, 126);");
            cliente.println("}");
            cliente.println(".btn-azul:hover{");
            cliente.println("background-color: rgb(181, 215, 255);");
            cliente.println("}");
            cliente.println(".btn-amarillo{");
            cliente.println("background-color: rgb(213, 247, 92)");
            cliente.println("border-color: rgb(119, 126, 22);");
            cliente.println("}");
            cliente.println(".btn-amarillo:hover{");
            cliente.println("background-color: rgb(235, 255, 181);");
            cliente.println("}");
            cliente.println("div{");
            cliente.println("display: flex;");
            cliente.println("width: 100%;");
            cliente.println("justify-content: center;");
            cliente.println("}");
            cliente.println("p{");
            cliente.println("width: 250px;");
            cliente.println("}");
            cliente.println("span{");
            cliente.println("font-size: 250px;");
            cliente.println("}");
            cliente.println("</style>");
            cliente.println("</head>");
            cliente.println("<body>");
            cliente.println("<h1 class=\"titulo\">Casa Domótica</h1>");
            cliente.println("<div>");
            cliente.println("<span>🏡</span>");
            cliente.println("</div>");
            cliente.println("<div id=\"status-container\">");
            cliente.println("</div>");
            cliente.println("<div>");
            cliente.println("<p><button class=\"btn btn-verde\" onclick=\"mostrarEstado('Sistema activado')\">ACTIVAR</button></p>");
            cliente.println("</div>");
            cliente.println("<div>");
            cliente.println("<p><button class=\"btn btn-magenta\" onclick=\"mostrarEstado('Sistema desactivado')\">DESACTIVAR</button></p>");
            cliente.println("</div>");
            cliente.println("<h1 class=\"titulo\">Tendederos</h1>");
            cliente.println("<div>");
            cliente.println("<p><a href=\"/tOn\"><button class=\"btn btn-azul\">ACTIVAR</button></a></p>");
            cliente.println("</div>");
            cliente.println("<div>");
            cliente.println("<p><a href=\"/tOff\"><button class=\"btn btn-magenta\">DESACTIVAR</button></a></p>");
            cliente.println("</div>");
            cliente.println("<div>");
            cliente.println("<p><a href=\"/lOn\"><button class=\"btn btn-amarillo\">ACTIVAR</button></a></p>");
            cliente.println("</div>");
            cliente.println("<div>");
            cliente.println("<p><a href=\"/lOff\"><button class=\"btn btn-magenta\">DESACTIVAR</button></a></p>");
            cliente.println("</div>");
            cliente.println("<script>");
            cliente.println("function mostrarEstado(estado) {");
            cliente.println("var statusContainer = document.getElementById('status-container');");
            cliente.println("statusContainer.innerHTML = \"<p class=\\\"titulo2\\\">\" + estado + \"</p>\";");
            cliente.println("}");
            cliente.println("</script>");
            cliente.println("</body>");
            cliente.println("</html>");            
          break;
          } else {
            lineaactual = "";
          }
        } else if (c != '\r')
          lineaactual += c;
      }
      if (lineaactual.endsWith("GET /on")) {
        activado = true;
      }
      if (lineaactual.endsWith("GET /off")) {
        activado = false;
      }
      if (lineaactual.endsWith("GET /lOn")) {
        lavadora = true;
      }
      if (lineaactual.endsWith("GET /lOff")) {
        lavadora = false;
      }
      if (lineaactual.endsWith("GET /tOn")) {
        bTendedero = true;
      }
      if (lineaactual.endsWith("GET /tOff")) {
        bTendedero = false;
      }
    }
    cliente.stop();
  }
}

void bombaAwa()
{
  if(humedad<umbral)
  {
    digitalWrite(pinRelay,HIGH);
  }
  else
  {
    digitalWrite(pinRelay,LOW);
    Serial.print("en el else |");
  }
}

void aplausos()
{
  if ((soundDetected==true) && (state1==false) && (state2==false) && (state3==false)) {
  state1 = true;
  Serial.println("Estado1 true");
  tem = millis();
  delay (100);
  } else if ((soundDetected == true) && (state1==true) && (state2==false) && (state3==false) ) {
  state2 = true; 
  Serial.println("Estado2 true");
  delay (100);
  } else if ((soundDetected == true) && (state1==true) && (state2==true) && (state3==false) ) {
  state3 = true; 
  Serial.println("Estado3 true");
  delay (100);  
  }
//activa y desactiva sonidos
  if ((state1==true) && (state2==true) && (state3==true)){
  stateLED = !stateLED;
  digitalWrite(led1 , stateLED);
  Serial.println("Reset");
  state1 = false; 
  state2 = false;
  state3 = false; 
  }
// se hace reset de los estados
  if (((millis()-tem) >= 500) && (state1==true) && (state2==false || state3==false)){ // Reseteamos estados al pasar 500 ms
  Serial.println("Reset Time");
  Serial.println((millis()-tem));
  state1 = false; 
  state2 = false;
  state3 = false;  
  }
}

void tendedero()
{
  if(bTendedero==true)
  {
    if(tendederoAv==0 && detenido == false)
    {
      tendederoAv=tiempoActual+4000;
      motores(izquierda3,derecha3,pwm1,canal1,6,10,true);
    }
    else if(tiempoActual>tendederoAv && detenido==false)
    {
      detenido=true;
      motores(izquierda3,derecha3,pwm1,canal1,6,10,false);
      tendederoAv=tiempoActual+4000;
    }
    else if(tiempoActual>tendederoAv && detenido==true)
    {
      detenido=false;
      tendederoAv=0;
    }
  }
  else
  {
    motores(izquierda3,derecha3,pwm1,canal1,6,10,false);
    tendederoAv=0;
    detenido=false;
  }
}

void puerta()
{
  digitalWrite(trig,LOW);
  delayMicroseconds(2);
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);
  duracion=pulseIn(echo, HIGH);
  distancia=(duracion/2)*0.0343; //Distancia en cm
  if(distancia<=5 && cerrado==true)
  {
    miServo.write(180);
    cerrado=false;
    puertaAbierta=0;
  }
  else if(distancia>5)
  {
    if(puertaAbierta==0)
    {
      puertaAbierta=tiempoActual+6000;
    }
    else if(tiempoActual>puertaAbierta)
    {
      cerrado=true;
      miServo.write(0);
    }
  }
}

void ventiladores()
{
  if(temp>35)
  {
    motores(izquierda1,derecha1,0,0,1,7777,true);
  }
  else{
    motores(izquierda1,derecha1,0,0,1,7777,false);
  }
}

void motores(int izquierda, int derecha, int pwm, int canal, int valor, int mayor, bool encender)
{
  if(encender==true)
  {
    if(mayor!=7777)
    {
      int velocidad=map(valor,0,mayor,0,255);
      digitalWrite(derecha,LOW);
      digitalWrite(izquierda,HIGH);
      ledcWrite(canal,velocidad);
    }
    else{
      digitalWrite(derecha,LOW);
      digitalWrite(izquierda,HIGH);
    }
  }
  else{
    digitalWrite(derecha,LOW);
    digitalWrite(izquierda,LOW);
  }
}

void lavadoraowo()
{
  if(lavadora==true)
  {
    motores(izquierda2,derecha2,0,0,1,7777,true);
  }
  else{
    motores(izquierda2,derecha2,0,0,1,7777,false);
  }
}

void apagar()
{
  digitalWrite(pinRelay,LOW);
  digitalWrite(led1,LOW);
}