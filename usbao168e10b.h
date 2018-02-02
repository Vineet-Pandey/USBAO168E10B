#ifndef USBAO168E10B_H
#define USBAO168E10B_H

#include<iostream>
#define __aiousb_cplusplus
#include <aiousb.h>
#include <stdio.h>
#include<vector>


/*
This class is defined to communicate with USB-AO16-8E-10B.User can use the class to
perform Digital to Analog conversions through the desired USB-AO16-8E-10B
board by using its serial number. Use- USBAO168E10B(SerialNumber);
*/


using namespace AIOUSB;
using namespace std;



class USBAO168E10B
{

private:
    DeviceProperties properties;
    int deviceIndex;
    double minValue;
    double range;
    uint64_t deviceSerialNumber;
    uint64_t serialNumber;

    char name[22];
    unsigned long productID, nameSize, numDIOBytes, numCounters;
    unsigned long deviceMask;
    int TEST_CHANNEL;
    int Sngl_Voltage;
    unsigned counts;

    vector<float> outmask;
    vector<float> digitalinput;
    float readbytes[16];

    const unsigned int MAX_COUNTS = 0xffff;
    const int MAX_CHANNELS = 8;
    const int MAX_NAME_SIZE = 20;






public:

    USBAO168E10B(uint64_t  deviceSerialNumber);// Receives the details of the board, initializes it with -10 to 10 V operation range with minValue=-10 and range=20
   ~USBAO168E10B();// exits the setup, Destroys the object
    void DACwrite(vector<float> UsrDefVoltage);// writes voltages to all the channels of user fed device
    void Sngl_Chnl_DACwrite(int TEST_CHANNEL,int Sngl_Voltage ); // writes the voltage to the user fed device and channels
    void digitalconfig(vector<int> outmask);
    void digitalwrite(vector<int> digitalinput);
    void digitalread();


};



#endif // USBAO168E10B_H
