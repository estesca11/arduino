#include <SoftwareSerial.h>
SoftwareSerial bluetooth(2, 3);
unsigned long prev = 0;
const int interval = 100;
double threshold = 2.0;
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
double tau = 3;

void setup()
{
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
        delay(10);
        Serial.print(volts[i]);
        Serial.print("  ");
    }
    Serial.print(1.0);
    Serial.print("  ");
    Serial.print(2.0);
    Serial.print("  ");
    Serial.print(3.0);
    Serial.print("  ");
    Serial.println("");
}
void setThreshold()
{
    if (bluetooth.available() > 0)
    {
        int inputTH = bluetooth.read();
        threshold = (double)inputTH / 10.0;
        Serial.println(threshold);
    }
}

