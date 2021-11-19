unsigned long prev = 0;
const int interval = 100;
const int threshold = 9.0;
const int ledPin[6] = {8, 9, 10, 11, 12, 13};
int Sensor1 = 0;
#define sensorNum 6
double volts[sensorNum] = {
    0,
};
unsigned int sample[sensorNum] = {
    0,
};
void setup()
{
    Serial.begin(9600);
    for (int i = 0; i < sensorNum ; i++)
    {
        pinMode(ledPin[i], OUTPUT);
    }
}
void loop()
{
    unsigned long now = millis();
    unsigned int peakToPeak[sensorNum] = {
        0,
    };
    unsigned int sigMin[sensorNum] = {
        0,
    },
                 sigMax[sensorNum] = {
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
        volts[i] = 3 * (peakToPeak[i] * 5.0) / 1024;
        if (volts[i] > threshold)
        {
            digitalWrite(ledPin[i], HIGH);
        }
        else
            digitalWrite(ledPin[i], LOW);
        Serial.print(volts[i]);
        Serial.print("   ");
    }
    Serial.println("");
}