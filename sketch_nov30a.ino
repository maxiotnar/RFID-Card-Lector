#include <SPI.h>
#include <MFRC522.h>
#include <avr/wdt.h> // libreria perro guardian/watch dog, cada un cierto tiempo reinicia el arduino automaticamente //
#define RST_PIN 9
#define SS_PIN 10

MFRC522 mfrc522 (SS_PIN, RST_PIN);  // Le decimos al arduino que vamos a usar el lector de tarjetas y entre parentesis //
// le pasamos los parametros //

byte lecturaUID[4]; // almacenar los datos del codigo de nuestras tarjetas ( arreglo tipo byte ) es de 4 posiciones ya que es //
// de 4 bytes //

// Creamos los usuarios que van a tener el acceso concedido //
byte Usuario1[4] = {0xD0, 0x9F, 0x75, 0x32}; // codigo tarjeta // datos ( codigos ) sacados de las tarjetas y llavero
byte Usuario2[4] = {0xE9, 0x22, 0xB0, 0xB9}; // codigo llavero // cada codigo es unico

int ledv=3;
int ledr=2;
int buzzer=4;


void setup() {
  // put your setup code here, to run once:
  wdt_disable();
 Serial.begin (9600);

 // Funciones para habilitar nuestro lector para que se comunique con el arduino y nos pueda pasar los datos de sus tarjetas //
 // e interactuar con ellas //
 SPI.begin();
 mfrc522.PCD_Init();

  Serial.println ("Ingrese tarjeta");
  pinMode (ledv , OUTPUT);
  pinMode (ledr , OUTPUT);
  pinMode (buzzer , OUTPUT);
  
  wdt_enable(WDTO_8S); // si pasan 8 segundos y el loop todavia no termino de dar una vuelta, se va a reiniciar automaticamente el 
  // arduino // si no reiniciamos el contador de perro guardian, el arduino se va a reiniciar siempre entonces en el loop al 
  // final  ponemos wdt_reset(); //
}

void loop() {
  // put your main code here, to run repeatedly:
 // Ahora para averiguar y mostrar el valor distinto de cada una de nuestras tarjetas //
 //El arduino nos va a devolver el codigo que almacena una tarjeta //
 // Al acercar la tarjeta al lector se activa el codigo //

 if (! mfrc522.PICC_IsNewCardPresent()){  // Esta funcion identifica cuando una tarjeta es acercada para poder leerla //
 return; // va a volver al loop cada vez que la funcion no encuentre una tarjeta /
 }
 if (! mfrc522.PICC_ReadCardSerial()){ // Para ver si el contenido de la tarjeta es accesible o no // No todas las tarjetas son legibles //
 return; // Nos avisa si el codigo de la tarjeta y el contenido de ella es legible o no // Evitamos lecturas invalidas //
 }

 for (byte i =0; i<mfrc522.uid.size;i++){ // la funcion mfrc522.uid.size nos dice el tamaño de bytes del codigo //
 // de cada tarjeta ( longitud de codigo de la tarjeta ) //

 if (mfrc522.uid.uidByte[i]<0x10){ // Para que los datos que nos muestre queden totalmente legibles y no amontonados //
 //  0x10 es igual a 10 en hexagesimal //

    Serial.print (" 0"); // imprima un espacio y un cero //
 }
 else {
      Serial.print(" ");
 }

    Serial.print( mfrc522.uid.uidByte[i], HEX); // Nos permite leer cual es el codigo de nuestra tarjeta en hexagesimal //
    // guardar los datos que ingresamos de nuestra tarjeta/llavero // con el array lecturaUID[4] // y los otros arrays sirven para comparar//
    // si la tarjeta tiene acceso //
    lecturaUID[i]=mfrc522.uid.uidByte[i]; // guarda de uno en uno los valores de nuestra tarjeta
 }

  Serial.print("\t"); // salto de renglon cada vez que se lee una tarjeta // y \t espacio de tabulacion //

// Ahora comparamos para ver si el codigo leido es alguno de los codigos habilitados que vamos a necesitar //
if(comparaUID(lecturaUID, Usuario1)){
  
    Serial.println ("Acceso concedido");
    digitalWrite (ledv , HIGH);
    digitalWrite (buzzer , HIGH);
    delay (500);
    digitalWrite (ledv , LOW);
    digitalWrite (buzzer , LOW);
  }
 else  if(comparaUID(lecturaUID, Usuario2)){
  
    Serial.println ("Acceso concedido");
    digitalWrite (ledv , HIGH);
    digitalWrite (buzzer , HIGH);
    delay (500);
    digitalWrite (ledv , LOW);
    digitalWrite (buzzer , LOW);
  }
  else {
    Serial.println ("Error, acceso denegado");
    for (byte i=0; i<4; i++){

    digitalWrite (ledr , HIGH);
    digitalWrite (buzzer , HIGH);
    delay (200);
    digitalWrite (ledr , LOW);
    digitalWrite (buzzer , LOW);
    delay(200);
    }
  }

  mfrc522.PICC_HaltA(); //Funcion para cortar la comunicacion una vez que se lee una tarjeta //
  // evitar que el lector siga leyendo la tarjeta una y otra vez //
  wdt_reset();
}

// Necesitamos declarar la funcion comparaUID, nos vamos fuera del loop para poder escribir el contenido de la funcion //
boolean comparaUID(byte lectura[], byte usuario[]){ // de tipo booleano pq nos devuelve true ( acceso concedido ) o false
// ( acceso denegado) ( 1 o 0) //

// recorre uno por uno de los datos de la tarjeta de ambos, del array tarjeta y el array comparador y va diciendo true o false //
for (byte i=0; i<mfrc522.uid.size; i++){

  if (lectura[i] != usuario[i]){
    return(false);
  }
  return(true);
}
}