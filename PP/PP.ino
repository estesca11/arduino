unsigned long prev = 0;
const int interval = 100;
#define sensorNum 1  
double volts[sensorNum] = {
    0,
};
unsigned int sample[sensorNum] = {
    0,
};
void setup()
{
    Serial.begin(9600);
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
        volts[i] = (peakToPeak[i] * 5.0) / 1024;
        Serial.print(i);
        Serial.print(" : ");
        Serial.print(peakToPeak[i]);
        Serial.print("   ");
    }
    Serial.println("");
}