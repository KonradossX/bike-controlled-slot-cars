//#include <SparkFun_TB6612.h>

#define maxPredkosc 50 // [km/h]
#define minPredkosc 0  // [km/h]

#define srednicaA 0.68   // [m]
#define srednicaB 0.60   // [m]

#define impulsA 3
#define impulsB 2
#define IN1A 9
#define IN1B 7
#define IN2A 10
#define IN2B 6
#define PWMA 11
#define PWMB 5
#define STBY 8

//Do konfiguracji silników
const int offsetA = 1;           //????
const int offsetB = 1;

unsigned int czasOczekiwania = 2000;

class Autko {
  private:
    unsigned long czas;
    unsigned long delta;
    bool wykryty;
    int IN1;
    int IN2;
    int PWM;
    int offset;
    int stby;
    //Motor motor;

  public:
    float srednica;
    float dlugosc;
    float predkosc;
    int   pinSygnal;

    Autko(int P, float D, int IN1pin, int IN2pin, int PWMpin) {
      pinSygnal = P;
      srednica  = D;
      policzDlugosc();

      IN1 = IN1pin;
      IN2 = IN2pin;
      PWM = PWMpin;

      czas = millis();
      pinMode(pinSygnal, INPUT_PULLUP);
      pinMode(IN1, OUTPUT);
      pinMode(IN2, OUTPUT);
      pinMode(PWM, OUTPUT);

      digitalWrite(IN1, HIGH);
      digitalWrite(IN2, HIGH);
      analogWrite(PWM, 0);
    }

    bool testujSygnal() {
      if (digitalRead(pinSygnal) == LOW) {
        if (wykryty == false) {
          wykryty = true;
        }
      } else {
        wykryty = false;
      }

      return wykryty;
    }

//    bool sprawdzCzas() {
//      if ((millis() - czas) > czasOczekiwania) {
//        return true;
//      } else {
//        return false;
//      }
//    }
    bool sprawdzCzas() {
      return ((millis() - czas) > czasOczekiwania);
    }

    void policzDlugosc() {
      //L = 2*pi*r = pi*d
      dlugosc = PI * srednica * 1000; // [m/s]
//      dlugosc = PI * srednica * 3600; // [km/h]
    }

    int policzPredkosc() {
      delta = millis() - czas;
      czas  = millis();

      //    Serial.print("D: ");
      //    Serial.println(delta);

      predkosc = dlugosc / float(delta);
      return (int)delta;
    }

    void steruj() {
      if (predkosc < minPredkosc) {
        //Stop
        digitalWrite(IN1, HIGH);
        digitalWrite(IN2, HIGH);
        analogWrite(PWM, 0);
      } else {
        digitalWrite(IN1, HIGH);
        digitalWrite(IN2, LOW);
        int x = map(predkosc, 0, 50, 0, 255);              //------------------ wylicz X
        analogWrite(PWM, x);
      }
    }
};

Autko autkoA = Autko(impulsA, srednicaA, IN1A, IN2A, PWMA);
Autko autkoB = Autko(impulsB, srednicaB, IN1B, IN2B, PWMB);

//------------------------------------------------------//
//                        Start
//------------------------------------------------------//

void setup() {
  pinMode(STBY, OUTPUT);
  digitalWrite(STBY, HIGH);
  Serial.begin(9600);

  Serial.println("\n\n\nAutka");

//  Serial.print("L_a: ");
//  Serial.println(autkoA.dlugosc);

  //  Serial.print("L_b: ");
  //  Serial.println(autkoB.dlugosc);

//  delay(10);
//  autkoA.policzPredkosc();
//  Serial.print("V: ");
//  Serial.println(autkoA.predkosc);

  delay(czasOczekiwania);
}

int d;
void loop() {  
  if (autkoA.testujSygnal() == true) {
    d = autkoA.policzPredkosc();
    //    autkoA.steruj();
  } else {
    if (autkoA.sprawdzCzas() == true) {
      autkoA.predkosc = 0;
      //      autkoA.steruj();
    }
  }

  autkoA.steruj();
  Serial.println(autkoA.predkosc);
//    Serial.print("\t");
//    Serial.println(d);


//    delay(100);


  /*
    if (autkoB.testujSygnal() == true) {
    autkoB.policzPredkosc();
    autkoB.steruj();
    }
  */
}
