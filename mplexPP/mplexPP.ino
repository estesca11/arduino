#define sensorNum 6                       //number of sensors
const int controlPin[4] = {8, 9, 10, 11}; //s0~s3 pin of MUX
const int SIG_pin = 0;
const int interval = 10;
double volts[sensorNum] = {
    0,
};
unsigned int sample[sensorNum] = {
    0,
};
void setup()
{
    //initializing MUX
    for (int i = 0; i < 4; i++)
    {
        pinMode(controlPin[i], OUTPUT);
        digitalWrite(controlPin[i], LOW);
    }
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
    //calculating min and max during the interval
    while (millis() - now < interval)
    {
        for (int channel = 0; channel < sensorNum; channel++) 
        {
            sample[channel] = readMux(channel); //storing instantaneous value from sensor
            if (sample[channel] > sigMax[channel])
            {
                sigMax[channel] = sample[channel];
            }
            else if (sample[channel] < sigMin[channel])
            {
                sigMin[channel] = sample[channel];
            }
        }
    }
    for (int channel = 0; channel < sensorNum; channel++)
    {
        peakToPeak[channel] = sigMax[channel] - sigMin[channel];
        volts[channel] = (peakToPeak[channel] * 5.0) / 1024;
        // Serial.print(channel);
        // Serial.print(" : ");
        if (peakToPeak[channel] < 1023) //filtering outlier
        {
            Serial.print(peakToPeak[channel]);
            Serial.print("   ");
        }
    }
    Serial.println("");
}
int readMux(const int channel) //returning analog value from channel
{
    //binary that deciding input channel of MUX, 0~15
    int muxChannel[16][4] = {
        {0, 0, 0, 0}, //channel 0
        {1, 0, 0, 0}, //channel 1
        {0, 1, 0, 0}, //channel 2
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