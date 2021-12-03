#include <SoftwareSerial.h>
SoftwareSerial bluetooth(2, 3);
unsigned long prev = 0;
const int interval = 100;
double threshold = 3.0;
#define sensorNum 3
double volts[sensorNum] = {
    0,
};
unsigned int sample[sensorNum] = {
    0,
};
double volts_value[sensorNum] = {
    0,
};

//Set the LPF parameter
double tau = 100;

void setup()
{
    pinMode(8, INPUT_PULLUP);
    pinMode(9, INPUT_PULLUP);
    pinMode(10, INPUT_PULLUP);
    Serial.begin(9600);
    bluetooth.begin(9600);
}
void loop()
{
    setThreshold(); //THR value from BT

    //initializing
    double af_value[sensorNum] = {
        0,
    };
    unsigned long now = millis();
    unsigned int peakToPeak[sensorNum] = {
        0,
    };
    unsigned int sigMin[sensorNum] = {
        0,
    };
    for (int i = 0; i < sensorNum; i++)
    {
        sigMin[i] = 1023;
    }
    unsigned int sigMax[sensorNum] = {
        0,
    };

    //calculating MIN and MAX of sensor value
    while (millis() - now < interval)
    {
        for (int i = 0; i < sensorNum; i++)
        {
            sample[i] = analogRead(i + 14);

            if (sample[i] > sigMax[i])
            {
                sigMax[i] = sample[i];
            }
            else if (sample[i] < sigMin[i])
            {
                sigMin[i] = sample[i];
            }
        }
    }

    //LPF
    for (int i = 0; i < sensorNum; i++)
    {
        peakToPeak[i] = sigMax[i] - sigMin[i];
        volts[i] = (peakToPeak[i] * 5.0) / 1024;
        double dt = interval / 10;
        af_value[i] = tau / (tau + dt) * volts_value[i] + dt / (tau + dt) * volts[i];
        volts_value[i] = af_value[i];
    }

    //output to serial monitor and BT
    for (int i = 0; i < sensorNum; i++)
    {
        if (af_value[i] > threshold)
        {
            bluetooth.write(i + 1);
        }
        Serial.print(af_value[i]);
        Serial.print("  ");
    }
    Serial.println("");
}
void setThreshold()
{
    if (bluetooth.available())
    {
        int inputTH = bluetooth.parseInt();
        Serial.println(inputTH);
        threshold = (double)inputTH / 10.0;
    }
}

/*
void buttonCheck()
{
    for (int i = 0; i < sensorNum; i++)
    {
        if ((digitalRead(i + 8)) == 1 && (occupation[i] == 0) && bluetooth.read() == (i + 1) * 10)
        {
            occupation[i] = 1;
        }
    }
}

void sendSeatValue()
{
    for (int i = 0; i < sensorNum; i++)
    {
        if (occupation[i] == 1)
        {
            bluetooth.write((i + 1) * 10 + 1);
        }
        else if (occupation[i] == 0)
        {
            bluetooth.write((i + 1) * 10);
        }
    }
}
*/
