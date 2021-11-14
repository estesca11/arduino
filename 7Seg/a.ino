#include <SoftwareSerial.h>
#define interval 100
unsigned long prev = 0;
int pins[] = {2, 3, 4, 5, 6, 7, 8};
int num_of_pins = 7;
bool segment[11][7] = {
    {true, true, true, true, true, true, false},     //0
    {false, true, true, false, false, false, false}, //1
    {true, true, false, true, true, false, true},    //2
    {true, true, true, true, false, false, true},    //3
    {false, true, true, false, false, true, true},   //4
    {true, false, true, true, false, true, true},    //5
    {true, false, true, true, true, true, true},     //6
    {true, true, true, false, false, false, false},  //7
    {true, true, true, true, true, true, true},      //8
    {true, true, true, true, false, true, true},     //9
    {false, false, false, false, false, false, true} //err

};
int s0 = 9;
int s1 = 10;
int s2 = 11;
int s3 = 12; //Mux in “SIG” pin
int SIG_pin = 0;
void setup()
{
    for (int i = 0; i < num_of_pins; ++i)
    {
        pinMode(pins[i], OUTPUT);
        digitalWrite(pins[i], HIGH);
    }
    pinMode(s0, OUTPUT);
    pinMode(s1, OUTPUT);
    pinMode(s2, OUTPUT);
    pinMode(s3, OUTPUT);
    digitalWrite(s0, LOW);
    digitalWrite(s1, LOW);
    digitalWrite(s2, LOW);
    digitalWrite(s3, LOW);
    Serial.begin(9600);
}
void loop()
{
    int refresh = 0;
    unsigned long now = millis();
    if (now - prev >= interval)
    {
        prev = now;
        if (readMux(0) > readMux(1))
        {
            seven_segment(segment[0]);
        }
        else if (readMux(1) > readMux(0))
        {
            seven_segment(segment[1]);
        }
        else
        {
            seven_segment(segment[11]);
        }
    }
}

int readMux(int channel)
{
    int controlPin[] = {s0, s1, s2, s3};
    int muxChannel[16][4] = {
        {0, 0, 0, 0},
        {1, 0, 0, 0}, //channel 1
                      /* {0, 1, 0, 0}, //channel 2
      {1, 1, 0, 0}, //channel 3
      {0, 0, 1, 0}, //channel 4
      {1, 0, 1, 0}, //channel 5
      {0, 1, 1, 0}, //channel 6
      {1, 1, 1, 0}, //channel 7
      {0, 0, 0, 1}, //channel 8
      {1, 0, 0, 1}, //channel 9
      {0, 1, 0, 1}, //channel 10
      {1, 1, 0, 1}, //channel 11
      {0, 0, 1, 1}, //channel 12
      {1, 0, 1, 1}, //channel 13
      {0, 1, 1, 1}, //channel 14
      {1, 1, 1, 1}  //channel 15*/
    };
    for (int i = 0; i < 4; i++)
    {
        digitalWrite(controlPin[i], muxChannel[channel][i]);
    }                              //read the value at the SIG pin
    int val = analogRead(SIG_pin); //return the value
    return val;
}
void seven_segment(bool arr[])
{
    for (int i = 0; i < num_of_pins; ++i)
    {
        arr[i] ? digitalWrite(pins[i], LOW) : digitalWrite(pins[i], HIGH);
    }
}