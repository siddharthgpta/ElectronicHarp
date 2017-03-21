#include "pitches.h"
int speaker = 11, rec = 7, play = 8, i, rc_state, rc_prev_state, pb_state, pb_prev_state, tune, count;
float threshold, V0, V1, V2, V3, V4;
unsigned long rc[100], startTime, elapsedTime;
int arr[] = {NOTE_A2, NOTE_C3, NOTE_E3, NOTE_G3, NOTE_A3};
void setup() {
  pinMode(speaker, OUTPUT);
  pinMode(rec, INPUT);
  pinMode(play, INPUT);
  threshold = max(max(analogRead(A0), analogRead(A1)), max(analogRead(A3), analogRead(A4)));
  i = rc_state = rc_prev_state = pb_state = pb_prev_state = 0;
  count = 0;
}
void loop() {
  rc_state = digitalRead(rec);
  pb_state = digitalRead(play);
  if(rc_state == HIGH && rc_prev_state == LOW) {
    rc_state = 1 - rc_state;
    delay(20);
  }
  rc_prev_state = rc_state;
  if(pb_state == HIGH && pb_prev_state == LOW) {
    pb_state = 1 - pb_state;
    delay(20);
  }
  pb_prev_state = pb_state;
  V0 = analogRead(A0);    //to read the first string
  V1 = analogRead(A1);    //to read the second string
  V2 = analogRead(A2);    //to read the third string
  V3 = analogRead(A3);    //to read the fourth string
  V4 = analogRead(A4);    //to read the fifth string
  tune = analogRead(A5);  //to adjust the tune of the harp
  if(pb_state == 0) {                 //to check whether playback of recorded tune is required
    if(V0 > threshold) {              //to see if first string is played
      startTime = millis();
      tone(speaker, NOTE_A2 + tune, 1);
      if(rc_state == 1 && count == 0) {               //start recording if enabled
        rc[i++] = 0;
        count = 1;
      }
    }
    if(V1 > threshold) {
      startTime = millis();
      tone(speaker, NOTE_C3 + tune, 1);  //similar to first string
      if(rc_state == 1 && count == 0) {
        rc[i++] = 1;
        count = 1;
      }
    }
    if(V2 > threshold) {
      startTime = millis();
      tone(speaker, NOTE_E3 + tune, 1);  //similar to first string
      if(rc_state == 1 && count == 0) {
        rc[i++] = 2;
        count = 1;
      }
    }
    if(V3 > threshold) {
      startTime = millis();
      tone(speaker, NOTE_G3 + tune, 1);  //similar to first string
      if(rc_state == 1 && count == 0) {
        rc[i++] = 3;
        count = 1;
      }
    }
    if(V4 > threshold) {
      startTime = millis();
      tone(speaker, NOTE_A3 + tune, 1);  //similar to first string
      if(rc_state == 1 && count == 0) {
        rc[i++] = 4;
        count = 1;
      }
    }
    if(V0 < threshold && V1 < threshold && V2 < threshold && V3 < threshold && V4 < threshold && count == 1) {
      elapsedTime = startTime - millis();
      rc[i++] = elapsedTime;
      count = 0;
    }
  }
  if(pb_state == 1) {
    for(int j = 0; j < i; j += 2) {
      tone(speaker, arr[rc[j]] + tune, rc[j + 2]);
    }
  }
}
