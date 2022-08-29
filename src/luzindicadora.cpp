#include <Arduino.h>

void loadEffect() {
    for(int i=2; i<12; i++){
      digitalWrite(i, HIGH);
      delay(100);
    }
    
    delay(1000);
    for(int i=2; i<12; i++){
      digitalWrite(i, LOW);
    }
    
    delay(100);
    for(int i=2; i<12; i++){
      digitalWrite(i, HIGH);
    }
    delay(100);
    for(int i=2; i<12; i++){
      digitalWrite(i, LOW);
    }
}

void lightsOff() {
    for(int i=2; i<12; i++){
      digitalWrite(i, LOW);
    }
}