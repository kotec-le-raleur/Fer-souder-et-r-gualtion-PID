/********************************************************
 * PID Basic Example
 ********************************************************/
//  version 1.0 du 7 juillet 2020
//
// Regulation fer Weller RL21
// Lib graphique   voir le site web: http://www.RinkyDinkElectronics.com/
// This program requires a Nokia 5110 LCD module.
//
// It is assumed that the LCD module is connected to
// the following pins using a levelshifter to get the
// correct voltage to the module.
//      SCK  - Pin 13    clock SPI
//      MOSI - Pin 11    Data in
//      DC   - Pin 6     data/command
//      RST  - Pin 8
//      CS   - Pin 7     chip select
//

/*
// importer les lib suivantes :
LCD5110_Graph  pour l'afficheur nokia 5110
PID_V1         pour la régulation PID
chrono         pour la mesure en temps des taches et pour  les ordonnancer
Onebutton      pour gérer l'appui long ou court sur mes boutons

*/


// voir les lib dans le fichier README.txt



#include <LCD5110_Graph.h>
#include <PID_v1.h>
#include <Chrono.h>
#include "OneButton.h"


//Define Variables we'll be connecting to
double Setpoint, Input, Output;

//Specify the links and initial tuning parameters
PID myPID(&Input, &Output, &Setpoint,4,0.2,0.1 ,P_ON_E, DIRECT);


#define  SCK       13
#define  DIN       11
#define  DC         6
#define  CE         7
#define  RST        8
#define  GATE_ON    9
#define  POTAR     A0
#define  TPFER     A1
#define  GAUCHE     4
#define  DROIT      5
#define  TEST       3

#define T_AVANT_COUPURE  20
#define NB_ACQ      4
#define SET_STDBY   100     // correspond à 80°C
#define SET_COLLE   220     // correspond à 130°C

//***********************************************
//    	LCD5110(int SCK, int MOSI, int DC, int RST, int CS);
//***********************************************
// Setup a new OneButton on pin 4.  
OneButton BtGauche(GAUCHE, true);
// Setup a new OneButton on pin 5.  
OneButton BtDroit(DROIT, true);

LCD5110 myGLCD(SCK,DIN,DC,RST,CE);

extern uint8_t SmallFont[];
extern uint8_t MediumNumbers[];
extern uint8_t BigNumbers[];


//int pot=0;
//int tfer=0;
//int OldTfer = 0;
//int Vres = 0;
//int Power = 1;
boolean STDBY = false;
boolean COLLE = false;
boolean ARRET = false;

Chrono secondsChrono(Chrono::SECONDS);
Chrono millisChrono;
Chrono Trestant(Chrono::SECONDS);

void setup()
{
	Serial.begin(57600);
    myGLCD.InitLCD();
    pinMode( GATE_ON, OUTPUT );
    pinMode( TEST, OUTPUT );
    pinMode( GAUCHE, INPUT );
    pinMode( DROIT,  INPUT );
  //initialize the variables we're linked to
    Input    = Get_Input() ;
    Setpoint = Get_Setpoint();
    Setpoint = map(Setpoint,0,1023,100,700);
    
  // link the button 1 functions.
  BtGauche.attachClick(clickG);
  //BtGauche.attachDoubleClick(doubleclick1);
  //BtGauche.attachLongPressStart(longPressStart1);
  //BtGauche.attachLongPressStop(longPressStop1);
  //BtGauche.attachDuringLongPress(longPress1);

  // link the button 2 functions.
   BtDroit.attachClick(clickD);
  //BtDroit.attachDoubleClick(doubleclickD);
  //BtDroit.attachLongPressStart(longPressStart2);
  //BtDroit.attachLongPressStop(longPressStop2);
  //BtDroit.attachDuringLongPress(longPress2);


    myPID.SetMode(AUTOMATIC);

}


void loop()
{
static int Tx=0,  Tac=T_AVANT_COUPURE;
static int mm,ss;

// tache 1  toutes les secondes                cette tache dure 8.76 millisecondes
    BtGauche.tick();
    BtDroit.tick();

  if (secondsChrono.hasPassed(1,true)) {
	  Tx++;
    mm = Tx/60;  ss = Tx -(mm*60);
    myGLCD.setFont(SmallFont);
    myGLCD.print ("Tx",  0, 0);
    myGLCD.printNumI (mm, 0, 8 ,2, '0');
    myGLCD.printNumI (ss, 0, 17,2, '0');
    Setpoint = Get_Setpoint();
    Setpoint = map(Setpoint,0,1023,110,700);
	
	Serial.print(Setpoint);
	Serial.print (" , ");  
	Serial.print(Input);
	Serial.print (" , ");  
	Serial.println(Output);

  }
 
 // tache 2 toutes les 330 millisecondes     cette tache dure 62.3 millisecondes
  if (millisChrono.hasPassed(573,true)) {
//  affichage de la température
    myGLCD.setFont(BigNumbers);
    myGLCD.printNumI(Input*0.6, CENTER, 2, 3, '0');
    myGLCD.drawRoundRect(20,0,64,27);
    
// affichage de la consigne    
    myGLCD.setFont(MediumNumbers);
    myGLCD.printNumI(Setpoint*0.6, CENTER, 30, 4, '0');

// affichage informatif provisoire    
    myGLCD.setFont(SmallFont);
    myGLCD.print ("Px",  0, 30);
    myGLCD.printNumI ((int)(Output/2.56),  0, 40,2,'0');
   
//    myGLCD.print ("%"  ,74, 30);
//    myGLCD.printNumI (pot/20,  70, 40,2), '0';
 
    myGLCD.update();
    }
//    digitalWrite(TEST,HIGH); 

	  Input = Get_Input();
	  myPID.Compute();           //  calcul du  PID
	  if (ARRET) {analogWrite(9,1); }
    else       {analogWrite(9,Output); }    // relié à la gate du mosfet
 //   digitalWrite(TEST,LOW); 

  }

//*************************************************************
int Get_Input (void)
{
	int rval = 0;
	for (int x=0; x<NB_ACQ;x++)	{ rval += analogRead(TPFER); delay(10);}
	rval = rval / NB_ACQ;
	return(rval);
}

//*************************************************************
int Get_Setpoint (void)
{
	int pval = 0;
	for (int x=0; x<NB_ACQ; x++)	{ pval += analogRead(POTAR); delay(10);}
	pval = pval / NB_ACQ;
	
	if(STDBY) pval = SET_STDBY;
	if(COLLE) pval = SET_COLLE;
	
	return(pval);

}

// callback liées aux boutons
//
void clickG() {
 COLLE = !COLLE;
}

void clickD() {
  STDBY = !STDBY;
} 


void doubleclickD() {
  ARRET = !ARRET;
} 


// This function will be called once, when the button1 is pressed for a long time.
//void longPressStart1() {
//  Serial.println("Button 1 longPress start");
//} // longPressStart1


// This function will be called often, while the button1 is pressed for a long time.
//void longPress1() {
//  Serial.println("Button 1 longPress...");
//} // longPress1


// This function will be called once, when the button1 is released after beeing pressed for a long time.
//void longPressStop1() {
//  Serial.println("Button 1 longPress stop");
//} // longPressStop1


// ... and the same for button 2:




	
