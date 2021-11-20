unsigned long prev = 0;
const int interval = 100;
const int threshold = 1.0;
const int ledPin[6] = {8, 9, 10, 11, 12, 13};
#define sensorNum 6
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
    Serial.begin(9600);
    for (int i = 0; i < sensorNum; i++)
    {
        pinMode(ledPin[i], OUTPUT);
    }
}
void loop()
{
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
    for (int i = 0; i < sensorNum; i++)
    {
        peakToPeak[i] = sigMax[i] - sigMin[i];
        volts[i] = (peakToPeak[i] * 5.0) / 1024;
        double dt = interval / 10;
        af_value[i] = tau / (tau + dt) * volts_value[i] + dt / (tau + dt) * volts[i];
        volts_value[i] = af_value[i];
        if (af_value[i] > threshold)
            digitalWrite(ledPin[i], HIGH);
        else
            digitalWrite(ledPin[i], LOW);
        volts_value[i] = af_value[i];
        if (af_value[i] < 5.0 && volts[i] < 5.0)
        {
            Serial.print(af_value[i]);
           // Serial.print(" ");
           // Serial.print(volts[i]);
            Serial.print("  ");
        }
    }
    Serial.println("");
}
