//#define f_pwm 4200 // Fsw = 3200 Hz = 3.2Khz
#define f_pwm 3180 // Fsw = 3180 Hz = 3.18Khz
#define pi 3.1416 // constante pi 

#define retardo  8/// 1us de tiempo muerto

unsigned int conv = 0 ;

unsigned char f = 60;
unsigned int n =  f_pwm / f;

unsigned int i =0 ;
unsigned int R=0; //valor amplitud SPWM

float paso =0;


void setup() {
  // put your setup code here, to run once:
  pinMode(9,OUTPUT);//////OC1A
  pinMode(10,OUTPUT);/////OC1B

  timer_1_init(); ///inicializar timer 1

  sei();           //Habilitar las interrupciones globales 

}

void loop() {
 //conv = analogRead(A0); // leer potenciometro  si se va a utilizar para variar la frecuencia 
  //f = map(conv,0,1023,10,100); /// mapeo la conversion de 10 a 100
  f = 60; // frecuencia fija a 60 hz
  n= f_pwm/f; // numero de cuantas pwms tenemos entran en la sinusoidal 
  paso = 2*pi / n; // calcula en angulos rad. la distancia de cada pwm para poder tener la senoidal de f Hz
  _delay_ms(200);

  

}

void timer_1_init(void)
{
  TCCR1A=0b10110000;/// OC1A activo y OC1B  / primeros dos bits,10 polaridad no inverutda, oc1b 11 polaridad invertida. y 0000modo fase y frecuencia corregida 
  TCCR1B = 0b00010001; // timer modo 8, y el reloj, preescalador de 1 a 16Mhz


  ICR1= 2500; // frecuencia  f pwm a 4200 hz debido a 16Mhz/2*f-1
  OCR1A = 0; /// ancho de pulso 0
  OCR1B = 0; /// ancho de pulso 0
  TIMSK1= 0b00100000;// habilitamos la interrupcion modo captura 
  
  }
ISR(TIMER1_CAPT_vect)
  {
    signed int deltaA = 0;
    signed int deltaB = 0 ;

    R = 1250*sin(i*paso)+1250 ; //senoidal que vaya 0 a 2500
    i++;
    if( i>n)
      i=0;
    deltaA = R - retardo ;  
    deltaB = R + retardo ; 

    deltaA= constrain(deltaA,0,2500); ////////limitar el maximo 
    deltaB= constrain(deltaB,0,2500);

    OCR1A = (unsigned int) deltaA; /// comparacion triangular del trimer con sinusoidal creada 
    OCR1B = (unsigned int) deltaB;
    
    
    
    }
