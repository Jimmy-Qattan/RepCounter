#include <SR04.h>
#include <IRremote.h>
#include <LiquidCrystal.h>

IRrecv irrecv(6);
decode_results results;

int reps = 0;
int buzzer = 5;
bool repPHASE = false;
int repsDONE = 0;
SR04 sr04 = SR04(3, 4);

long barPlateTrackers;

LiquidCrystal lcd(7, 8, 9, 10, 11, 12);

int mode = 0;
bool cyclingThroughWorkouts = true;

void setup() {
  lcd.begin(16, 2);
  irrecv.enableIRIn();
  Serial.begin(9600);
  pinMode(buzzer, OUTPUT);

  digitalWrite(buzzer, HIGH);

  if (cyclingThroughWorkouts) {
    lcd.print("7 - Squat");
    delay(3000);
    lcd.clear();
    lcd.print("8 - Bench Press");
    delay(3000);
    lcd.clear();
    lcd.print("9 - Deadlift");
  }

  
  
}

void loop() {
  lcd.setCursor(0, 1);

  barPlateTrackers = sr04.Distance();
  Serial.print(barPlateTrackers);

  if (irrecv.decode(&results)) {
    if (results.value == 0xFF42BD) {
      mode = 7;
      cyclingThroughWorkouts = false;
      lcd.clear();
      lcd.print("Squat");
      delay(2000);
      lcd.clear();
      lcd.print("How Many Reps?");
      repPHASE = true;
      lcd.setCursor(0, 1);
      
      
    } else if (results.value == 0xFF4AB5) {
      mode = 8;
      cyclingThroughWorkouts = false;
      lcd.clear();
      lcd.print("Bench Press");
      delay(2000);
      lcd.clear();
      lcd.print("How Many Reps?");
      repPHASE = true;


      
    } else if (results.value == 0xFF52AD) {
      mode = 9;
      cyclingThroughWorkouts = false;
      lcd.clear();
      lcd.print("Deadlift");
      delay(2000);
      lcd.clear();
      lcd.print("How Many Reps?");
      repPHASE = true;
    } else if (results.value == 0xFF906F) {
      if (repPHASE == true) {
        reps += 1;
        lcd.clear();
        lcd.setCursor(1, 0);
        lcd.print("How Many Reps?");
        lcd.setCursor(0, 1);
        lcd.print(reps);
      }
    } else if (results.value == 0xFFE01F) {
      if (repPHASE == true) {
        if (reps > 1) {
          reps -= 1;
          lcd.clear();
          lcd.setCursor(1, 0);
          lcd.print("How Many Reps?");
          lcd.setCursor(0, 1);
          lcd.print(reps);
        }
      }
    } else if (results.value == 0xFF02FD) {
      lcd.clear();
      lcd.setCursor(1, 0);
      lcd.print("Beginning in");
      delay(3000);
      for (int i = 0; i < 5; i++) {
        lcd.clear();
        lcd.setCursor(1, 0);
        lcd.print("Beginning in");
        lcd.setCursor(0, 1);
        lcd.print((-1 * i) + 5);
        digitalWrite(buzzer, LOW);
        delay(1000);
        digitalWrite(buzzer, HIGH);
      }
      lcd.clear();
      lcd.setCursor(1, 0);
      lcd.print("LET'S GOOOOO!!!!!");
      digitalWrite(buzzer, LOW);
      delay(1500);
      digitalWrite(buzzer, HIGH);
      lcd.clear();
      checkForRep();
      lcd.print(barPlateTrackers);
    } else if (results.value == REPEAT) {
      irrecv.resume();
    }

    irrecv.resume();
  }
}

void checkForRep() {
  for (int i = 0; i < 1000; i++) {
    barPlateTrackers = sr04.Distance();
    

    if (mode == 7) {
      
      if (barPlateTrackers > 45) {

        lcd.clear();
        lcd.print("Closer to Butt");
        
        
      } else if (barPlateTrackers < 30) {


        lcd.clear();
        lcd.print("Farther");

        
      } else if ((barPlateTrackers > 30) && (barPlateTrackers < 45)) {

        lcd.clear();
        lcd.print("Perfect");

        runDeadLiftPart1();
        delay(1000);
        lcd.clear();
        return 0;
      }

      

    } else if (mode == 8) {
      
      if (barPlateTrackers > 45) {

        lcd.clear();
        lcd.print("Closer to Plates");
        
        
      } else if (barPlateTrackers < 30) {


        lcd.clear();
        lcd.print("Farther");

        
      } else if ((barPlateTrackers > 30) && (barPlateTrackers < 45)) {

        lcd.clear();
        lcd.print("Perfect");

        runDeadLiftPart1();
        delay(1000);
        lcd.clear();
        return 0;
      }

      
      
    } else if (mode == 9) {

      if (barPlateTrackers > 45) {

        lcd.clear();
        lcd.print("Closer to Plates");
        
        
      } else if (barPlateTrackers < 30) {


        lcd.clear();
        lcd.print("Farther");

        
      } else if ((barPlateTrackers > 30) && (barPlateTrackers < 45)) {

        lcd.clear();
        lcd.print("Perfect");

        runDeadLiftPart1();
        delay(1000);
        lcd.clear();
        return 0;
      }

      
      
    }


    
  }
}

void runDeadLiftPart1() {
  for (int j = 0; j < 1000; j++) {
    barPlateTrackers = sr04.Distance();

    if ((barPlateTrackers >= 45) && (repsDONE <= reps)) {
      delay(2000);
      runDeadLiftPart2();
    } else if (repsDONE >= reps) {
      lcd.clear();
      lcd.print("Great Job!");
    }
  }
}

void runDeadLiftPart2() {
  for (int k = 0; k < 1000; k++) {
    barPlateTrackers = sr04.Distance();

    if ((barPlateTrackers > 30) && (barPlateTrackers < 45)) {
      repsDONE += 1;
      lcd.clear();
      lcd.print(repsDONE);
      digitalWrite(buzzer, LOW);
      delay(1000);
      digitalWrite(buzzer, HIGH);
      runDeadLiftPart1();
    }
  }
}
