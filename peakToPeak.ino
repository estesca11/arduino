unsigned long prev = 0;
const int interval = 100;
double volts[2] = {
    0,
};
unsigned int sample[2] = {
    0,
};
void setup()
{
    Serial.begin(9600);
}
void loop()
{
    unsigned long now = millis();
    unsigned int peakToPeak[2] = {
        0,
    };
    unsigned int sigMin[2] = {
        0,
    },
                 sigMax[2] = {
                     0,
                 };
    while (millis() - now < interval)
    {
        for (int i = 0; i < 2; i++)
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
    for (int i = 0; i < 2; i++)
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