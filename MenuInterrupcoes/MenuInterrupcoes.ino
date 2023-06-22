// MENU COM TRÊS BOTÕES 2.0
// ABRAÃO LACERDA - ACESSE O LINK https://www.youtube.com/channel/UCn_SRCloMrZ5y9XOHSIUFUQ

// === Biblioteca para o display === //
#include <Wire.h>
//#include <LCD.h>
#include <LiquidCrystal_I2C.h>
#include <AccelStepper.h>

#define col 16
#define lin 2
#define ende 0x27
#define motorPin1 8
#define motorPin2 9
#define motorPin3 10
#define motorPin4 11
#define HALFSTEP 8
#define botao_MAIS 2                  //define o botão no pino 2 (MAIS)
#define botao_ENTER 3                 //define o botão no pino 3 (ENTER/VOLTAR)
#define botao_MENOS 4                 //define o botão no pino 4 (MENOS)

LiquidCrystal_I2C lcd(ende,col,lin);
AccelStepper stepper1(HALFSTEP,motorPin1,motorPin3,motorPin2,motorPin4);

bool L_botao_MAIS = 1;                //variável para fazer a leitura do botão              (MAIS)
bool LA_botao_MAIS = 1;               //variável para verificar a leitura anterior do botão (MAIS)
bool L_botao_ENTER = 1;               //variável para fazer a leitura do botão              (ENTER/VOLTAR)
bool LA_botao_ENTER = 1;              //variável para verificar a leitura anterior do botão (ENTER/VOLTAR)
bool L_botao_MENOS = 1;               //variável para fazer a leitura do botão              (MENOS)
bool LA_botao_MENOS = 1;              //variável para verificar a leitura anterior do botão (MENOS)

bool  pausa = true ;

int velocidade_Motor = 300;

const int VALOR_MAXIMO = 900;
const int VALOR_MINIMO = 600;

int concentracao_Minima = 30;
int concentracao_Maxima = 50;

const int pino_rele_lampada = 6;
const int PINO_SENSOR = A1;

const int PINO_RELE = 5;

int leitura_sensor = 0;
int endPoint = 12288; 
int tela = 1;                         //variável para controlar a mudança de tela no menu

bool first_run=true;

void setup(){   
  //  TIMER PARA O MOTOR //
  TCCR1A = 0;                        //confira timer para operação normal pinos OC1A e OC1B desconectados
  TCCR1B = 0;                        //limpa registrador
  TCCR1B |= (1<<CS10);   // configura prescaler para 1024: CS10=1
  TCNT1 = 0xC2F7;                    // incia timer com valor para que estouro ocorra em 0,001 segundo
                                     // 65536-(16MHz/1024/1Hz) = 49911 = 0xC2F7
  TIMSK1 |= (1 << TOIE1);
 // INTERRUPÇOES PARA OS BOTOES //
  DDRD = 0x00;
  PORTD = 0xFF;
  PCICR = 1<<PCIE2;
  PCMSK2 = (1<<PCINT18)|(1<<PCINT19)|(1<<PCINT20);
  sei();
  // SETUP DISPLAY //
  lcd.init();                    //inicia o display 16x2
  lcd.backlight();                       //limpa todos os caracteres do lcd
  lcd.setCursor(0,0);                //posiciona o cursor no display (caracter, linha)      
  lcd.print(F(" SEJA BEM VINDO "));  //imprime o texto entre aspas  
  lcd.setCursor(0,1);                //posiciona o cursor no display (caracter, linha)      
  lcd.print(F(" Iniciando...   "));  //imprime o texto entre aspas 
  delay (3000);                        //pausa o programa por 3 segundos
  // SETUP MOTOR //
  stepper1.setMaxSpeed(1000.0);
  stepper1.setAcceleration(1000.0);
  stepper1.setSpeed(100);
  stepper1.moveTo(endPoint);
  // SETUP PINO //
  pinMode(PINO_SENSOR, INPUT);
  pinMode(PINO_RELE, OUTPUT);
  digitalWrite(PINO_RELE, HIGH);
  pinMode(pino_rele_lampada, OUTPUT);
  digitalWrite(pino_rele_lampada, LOW);

 }
 
void loop(){
  // Ações da bomba e do sensor de umidade //
  leitura_sensor = analogRead(PINO_SENSOR);
  leitura_sensor = map(leitura_sensor, VALOR_MINIMO, VALOR_MAXIMO,0,100);

  if(leitura_sensor <concentracao_Minima && !first_run){ //Se sim
    digitalWrite(PINO_RELE, LOW); //Acionamos o rele
    pausa = false; //Configura a variavel "pausa" como falsa  
  } else if (leitura_sensor > concentracao_Maxima) { //Caso contrario
    digitalWrite(PINO_RELE, HIGH); //O rele se mantem desligado
    pausa = true; //Configura a variavel "pausa" como verdadeira
  }  
// === Ações no display LCD === // 
   if (tela==1){                          //se a variável tela for igual a MAIS, faça...
   lcd.setCursor(0,0);                    //posiciona o cursor no display (caracter, linha)      
   lcd.print(F("   << MENU >>                  "));      //imprime o texto entre aspas 
   lcd.setCursor(0,1);                    //posiciona o cursor no display (caracter, linha)      
   lcd.print(String("Umidade ")+String(leitura_sensor)+String("               "));            //imprime o texto entre aspas 
   }
   else if (tela==2){                            
   lcd.setCursor(0,0);                     
   lcd.print(F("   << MENU >>                  "));      
   lcd.setCursor(0,1);                     
   lcd.print(F(">Ligar Lampada                 "));     
   }
   else if (tela==3){                            
   lcd.setCursor(0,0);                     
   lcd.print(F("   << MENU >>                  "));      
   lcd.setCursor(0,1);                     
   lcd.print(F(">Desligar Lampada               "));

   }
      else if (tela==4){                            
   lcd.setCursor(0,0);                     
   lcd.print(F("   << MENU >>                  "));      
   lcd.setCursor(0,1);                     
   lcd.print(F(">Conf Umi Max                 "));     
   }
      else if (tela==5){        
                            
   lcd.setCursor(0,0);                     
   lcd.print(F("   << MENU >>                  "));      
   lcd.setCursor(0,1);                     
   lcd.print(F(">Conf Umi Min                 "));     
   }
   else if (tela==20){                            
   lcd.setCursor(0,0);                     
   lcd.print(F("Lampada:               "));      
   lcd.setCursor(0,1);
   lcd.print(F("Ligada               "));   
   digitalWrite(pino_rele_lampada, LOW);
   }
   else if (tela==30){  
   lcd.setCursor(0,0);                           
   lcd.print(F("Lampada               "));      
   lcd.setCursor(0,1);                     
   lcd.print(F("Desligada               "));   
   digitalWrite(pino_rele_lampada, HIGH);  
   }
   else if (tela==40){
   lcd.setCursor(0,0);                              
   lcd.print(F("Conf Hum Max               "));      
   lcd.setCursor(0,1);                     
   lcd.print(String(">Hum Max  ")+String(concentracao_Maxima)+String("%               "));     
   }
   else if (tela==50){                            
   lcd.setCursor(0,0);                     
   lcd.print(F("Conf Hum Min"));      
   lcd.setCursor(0,1);                     
   lcd.print(String(">Hum Min  ")+String(concentracao_Minima)+String("%               "));     
   }
   if(first_run){
     first_run=false;
     delay(500);
   }
////////////////////////////////////////////////
}
ISR(PCINT2_vect){
  //lcd.init();
  L_botao_MAIS = digitalRead(botao_MAIS);
  if (!L_botao_MAIS && LA_botao_MAIS) {
            if (tela==1)tela=2; 
       else if (tela==2)tela=3;
       else if (tela==3)tela=4;
       else if (tela==4)tela=5;
       else if (tela==5)tela=1;
       //else if (tela==6)tela=1;
       else if (tela==40)(
         concentracao_Maxima = concentracao_Maxima+1
       ); 
       else if (tela==50)(
         concentracao_Minima = concentracao_Minima+1
       );           
       delay(200);}                         
  LA_botao_MAIS = L_botao_MAIS; 


/////////////////////////////////////////////////

// === Ações do Botão MENOS === //   ////////////
  L_botao_MENOS = digitalRead(botao_MENOS);
  if (!L_botao_MENOS && LA_botao_MENOS) {
       if (tela==1)tela=5; 
       else if (tela==2)tela=1;
       else if (tela==3)tela=2;
       else if (tela==4)tela=3;  
       else if (tela==5)tela=4;
       else if (tela==40)(
         concentracao_Maxima = concentracao_Maxima-1
       ); 
       else if (tela==50)(
         concentracao_Minima = concentracao_Minima-1
       );  

       delay(200);}                         
  LA_botao_MENOS = L_botao_MENOS;

     
/////////////////////////////////////////////////

// === Ações do Botão ENTER === //   ////////////
  L_botao_ENTER = digitalRead(botao_ENTER);
  if (!L_botao_ENTER && LA_botao_ENTER) {
           // if (tela==1)tela=10; 
            if (tela==2){
            tela=20;
            digitalWrite(pino_rele_lampada, HIGH);}
       else if (tela==3){
         tela=30;
         digitalWrite(pino_rele_lampada, LOW);}
       else if (tela==4)tela=40;
       else if (tela==5)tela=50;
       else if (tela==6)tela=60;
       else if (tela==10)tela=1;
       else if (tela==20)tela=2;
       else if (tela==30)tela=3;
       else if (tela==40)tela=4;
       else if (tela==50)tela=5;        
       delay(200);}                         
  LA_botao_ENTER = L_botao_ENTER;}

  ISR(TIMER1_OVF_vect)
  { 
   TCNT1 = 0xC2F7;   
   if (stepper1.distanceToGo() == 0)
   {
     Serial.println(stepper1.currentPosition());
     stepper1.setCurrentPosition(0);
     endPoint = -endPoint;
     stepper1.moveTo(endPoint);
     Serial.println(stepper1.currentPosition());
   }
    stepper1.run();
  }