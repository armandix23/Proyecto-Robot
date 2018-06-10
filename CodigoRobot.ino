// Libreria para el control del servo.
#include <Servo.h>
/*
 * Los parametros que tenemos comentados en el void setup sirven para poder asignar el pin conectado a la placa.
 * Lo que tenemos que hacer antes de nada es poner nuestros pines AT de la placa en ON. 
 * Conectamos el robot y abrimos un nuevo IDE arduino, en herramientas abrimos un monitor serie y ponemos los baudios en 19200 y en modo retorno de carro. 
 * Para sacar los comandos ponemos AT+HELP y nos aparecera los comandos que podemos utilizar con la placa.
 * Cuando hayamos terminado de modificar nuestra placa quitamos el cable y dejamos el pin 1 de nuestro AT en ON y el 2 y 3 en OFF.
 */
int levantar_0 = 0;
int levantar_1 = 1;

/* Definición de los pines de la placa que se van a utilizar,
   hemos dejado sin activar los pines del sensor ultrasonido(pines 4-TRI y 5-ECH) 
   y del sensor infrarrojo(pines 2-IZQ y 3-DCH).
 */

#define pinRuedaIzq                		  8
#define pinRuedaDrc               		  9
#define pinMotorCabeza                		 11
#define pinSonido                  	  	 12
#define pinLuzIzq           		         A2
#define pinLuzDrc          			 A3

// Angulos para los movimientos del servo(Ruedas).
#define PararRuedas         		         90
#define RuedaIzqDelan      		          0
#define RuedaIzqAtras   		        180
#define RuedaDrcDelan   	 	        180
#define RuedaDrcAtras  		       	          0

// Definiciones para nuestros menus.
#define MenuPrincipal			          0
#define MenuEvitaLuz             		  1
#define MenuSigueLuz		                  2
#define MenuAutomatico			          3

// Angulos para la cabeza.
#define Centro                    		 80  
#define mIzquierda                    		110
#define mDerecha                     	 	 50  


// Tamaño del búfer.
#define CantidadBuffer            		  5

// Delay por defecto.
#define TiempoRetardo             	   	 10

// Variable del estado actual.
int EstadoActual = 0;


// Creacion de objetos servo para los motores de las ruedas.
Servo MotoIzq, MotoDrc;

// Creacion de objeto servo para el motor de la cabeza.
Servo cabeza;
int anguloCabeza = 0;

// Búfer para almacenar los datos recibidos de Bluetooth.
char Datos[CantidadBuffer];

// Tamaño fijo para el búfer .
int i = 0;

// Número de caracteres para el serial.
int numChar = 0;

// Variables para controlar la funcion Evita luz.
int DrcLuz = 0, IzqLuz = 0, MinLuz = 0, MaxLuz = 0;

// Variable para el automatico.
int numeroRandom = 0;

void automatico(int numeroRandom) {
    switch (numeroRandom) {

        case 0:
            Avanzar();
            break;

        case 1:
            Izquierda();
            break;

        case 2:
            Derecha();
            break;

        case 3:
            Parar();
            break;
    }
}

void movimientoCabeza(int angulo, int retardo) {
    cabeza.write(angulo);
    delay(retardo);
    anguloCabeza = angulo;
}

void Parar() {

    MotoIzq.write(PararRuedas);
    delay(TiempoRetardo);

    MotoDrc.write(PararRuedas);
    delay(TiempoRetardo);
}

void Avanzar() {

    if (anguloCabeza != Centro) {
        movimientoCabeza(Centro, TiempoRetardo);
    }

    MotoIzq.write(RuedaIzqDelan);
    delay(TiempoRetardo);

    MotoDrc.write(RuedaDrcDelan);
    delay(TiempoRetardo);
}

void Retroceder() {

    if (anguloCabeza != Centro) {
        movimientoCabeza(Centro, TiempoRetardo);
    }

    MotoIzq.write(RuedaIzqAtras);
    delay(TiempoRetardo);

    MotoDrc.write(RuedaDrcAtras);
    delay(TiempoRetardo);
}

void Izquierda() {

    if (anguloCabeza != mIzquierda) {
        movimientoCabeza(mIzquierda, TiempoRetardo);
    }

    MotoIzq.write(PararRuedas);
    delay(TiempoRetardo);

    MotoDrc.write(RuedaDrcDelan);
    delay(TiempoRetardo);
}

void Derecha() {

    if (anguloCabeza != mDerecha) {
        movimientoCabeza(mDerecha, TiempoRetardo);
    }

    MotoIzq.write(RuedaIzqDelan);
    delay(TiempoRetardo);

    MotoDrc.write(PararRuedas);
    delay(TiempoRetardo);
}

void Cuadrado() {

    Avanzar();
    delay(2500);

    Derecha();
    delay(850);

    Avanzar();
    delay(2500);

    Derecha();
    delay(860);

    Avanzar();
    delay(2500);

    Derecha();
    delay(860);

    Avanzar();
    delay(2500);

    Derecha();
    delay(860);

    Parar();
}

void Triangulo() {

    Avanzar();
    delay(3000);

    Derecha();
    delay(1200);

    Avanzar();
    delay(3500);

    Derecha();
    delay(1000);

    Avanzar();
    delay(3500);

    Derecha();
    delay(1200);

    Parar();
}

void Circulo() {
    for (int i = 0; i <= 5; i++) {
        Avanzar();
        delay(10);
        Izquierda();
        delay(10);
    }
}

void EvitaLuz() {

    DrcLuz = analogRead(pinLuzDrc);
    IzqLuz = analogRead(pinLuzIzq);

    if (DrcLuz < MaxLuz) {
        MotoDrc.write(PararRuedas);
        delay(TiempoRetardo);

    } else {
        MotoDrc.write(RuedaDrcDelan);
        delay(TiempoRetardo);
    }

    if (IzqLuz < MaxLuz) {
        MotoIzq.write(PararRuedas);
        delay(TiempoRetardo);

    } else {
        MotoIzq.write(RuedaIzqDelan);
        delay(TiempoRetardo);
    }
}

void SigueLuz() {

    DrcLuz = analogRead(pinLuzDrc);
    IzqLuz = analogRead(pinLuzIzq);

    if (DrcLuz > MinLuz) {
        MotoIzq.write(RuedaIzqDelan);
        delay(TiempoRetardo);

    } else {
        MotoDrc.write(PararRuedas);
        delay(TiempoRetardo);
    }

    if (IzqLuz > MinLuz) {
        MotoDrc.write(RuedaDrcDelan);
        delay(TiempoRetardo);

    } else {
        MotoIzq.write(PararRuedas);
        delay(TiempoRetardo);
    }
}

void ControlDatos(char * palabra) {

    switch (palabra[0]) {
        case 'p':
            EstadoActual = MenuPrincipal;
            Parar();
            break;

        case 'e':
            EstadoActual = MenuEvitaLuz;
            break;

        case 's':
            EstadoActual = MenuSigueLuz;
            break;

        case 'x':
            EstadoActual = MenuAutomatico;
            break;

        case 'a':
            Avanzar();
            break;

        case 'r':
            Retroceder();
            break;

        case 'i':
            Izquierda();
            break;

        case 'd':
            Derecha();
            break;

        case 'c':
            Cuadrado();
            break;

        case 't':
            Triangulo();
            break;

        case 'l':
            Circulo();
            break;
    }

    // Limpiamos el serial.
    Serial.flush();
}

void setup() {

    // Establecer la velocidad de baudios y limpiamos el serial.
    Serial.begin(19200);
    Serial.flush();

    // Asignar los pines a la funcion.
    MotoIzq.attach(pinRuedaIzq);
    MotoDrc.attach(pinRuedaDrc);
    cabeza.attach(pinMotorCabeza);

    /*Descomentar cuando vayamos hacer algun cambio en la placa(Nombre, pin, baudios)
      pinMode(levantar_0, INPUT);
      pinMode(levantar_1, INPUT);
	 */

    // Colocacion de la cabeza en el Centro.
    movimientoCabeza(Centro, TiempoRetardo);

    // Iniciar estado en el menu principal.
    EstadoActual = MenuPrincipal;

    // Limites para captar la intensidad de lumenes con los sensores de luz. 
    MinLuz = 200;
    MaxLuz = 200;

    Parar();
}

void loop() {

    // Si recibe datos los empezara a leer caracter por caracter.

    if (Serial.available() > 0) {

        i = 0;
        memset(Datos, 0, sizeof(Datos));
        delay(CantidadBuffer);

        // Número de caracteres disponibles en el serial.
        numChar = Serial.available();

        // Limite de números de caracteres que se leeran desde el Serial para evitar el desbordamiento del buffer.
        if (numChar > CantidadBuffer) {
            numChar = CantidadBuffer;
        }

        // Lee lo que reciba el serial y lo va guardando en datos.
        while (numChar--) {
            Datos[i++] = Serial.read();
            delay(3);
        }

        // Llama a la funcion ControlDatos y envia lo que haya recogido del serial.
        ControlDatos(Datos);
    }

    /* Actuara dependiendo de lo que lea de la señal emitida por bluetooth, 
	   si la señal emite una accion como las nombradas abajo entrara en esos estados, por defecto el estado es 0.
	 */

    if (EstadoActual == MenuEvitaLuz) {
        EvitaLuz();

    } else if (EstadoActual == MenuSigueLuz) {
        SigueLuz();

    } else if (EstadoActual == MenuAutomatico) {

        // En este estado conseguimos que el robot se mueva automaticamente y no depende de nosotros utilizando un random y apuntado a los movimientos.
        numeroRandom = random(3);
        delay(500);

        // Funcion que no devuelve nada y le envia un numero random para luego leerlo en un switch que hara los movimientos correspondientes.
        automatico(numeroRandom);
    }
}
