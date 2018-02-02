#include<iostream>
#define __aiousb_cplusplus
#include <aiousb.h>
#include <stdio.h>
#include"usbao168e10b.h"
#include <vector>
#include<iostream>
using namespace std;
using namespace AIOUSB;



USBAO168E10B::USBAO168E10B(uint64_t deviceSerialNumber)
{

    unsigned long result = AIOUSB_Init();

    if( result == AIOUSB_SUCCESS )
    {
        unsigned long deviceMask = GetDevices();
        if( deviceMask != 0 )
        {

            cout<<"You are trying to communicate with device having a serial number: "<<hex<<deviceSerialNumber<<endl;
            AIOUSB_ListDevices(); //Lists all the ACESS products connected to the system



            for(deviceIndex=0;deviceIndex<=MAX_USB_DEVICES;deviceIndex++) //searches all the device indices available on the system ports
            {
                unsigned long result = GetDeviceSerialNumber( deviceIndex, &serialNumber ); // gets the serialNumber for the devices connected
                if(result==AIOUSB_SUCCESS && serialNumber==deviceSerialNumber )
                {



                    cout<<"Device Index in operation is "<<deviceIndex<<endl;
                    break;

                }

                else {
                    cout<<"No device found at the index : "<< deviceIndex<<endl;
                    continue;
                }
            }



            if (deviceIndex==MAX_USB_DEVICES) //executed if the board with the user fed Serial Number s not found
            {
                cout<<"device with the serial number: "<< hex<<deviceSerialNumber<<"not found on any port"<<endl;
            }



            bool deviceFound = false;

            while( deviceMask != 0 )
            {
                if( ( deviceMask & 1 ) != 0 )
                {

                    unsigned long result = QueryDeviceInfo( deviceIndex, &productID, &nameSize, name, &numDIOBytes, &numCounters );
                    if( result == AIOUSB_SUCCESS )
                    {
                        if( productID >= USB_AO16_16A&& productID <= USB_AO12_4 )
                        {
                            deviceFound = true;
                            break;
                        }
                    } else
                        printf( "Error '%s' querying device at index %lu\n", AIOUSB_GetResultCodeAsString( result ), deviceIndex );
                }
                deviceIndex++;
                deviceMask >>= 1;
            }

            if( deviceFound )
            {
                AIOUSB_SetCommTimeout( deviceIndex, 500 );

                DeviceProperties properties;
                unsigned long result = AIOUSB_GetDeviceProperties( deviceIndex, &properties );
                if( result == AIOUSB_SUCCESS )
                    cout<< "Device properties successfully retrieved for device with serial number:"<<deviceSerialNumber<<endl ;

                else
                {
                    properties.DACChannels = MAX_CHANNELS;
                    printf( "Error '%s' getting device properties\n", AIOUSB_GetResultCodeAsString( result ) );
                }

                result = DACSetBoardRange( deviceIndex, 3 ); //sets the DAC Range to -10V to 10V by default


                if( result == AIOUSB_SUCCESS )
                {
                    cout<<"D/A output successfully set to operational range of -10 to 10V for device at index "<<deviceIndex<<endl;
                   minValue=-10;range=20;
                }

                else
                {
                    printf( "Error '%s' setting D/A ourput range\n", AIOUSB_GetResultCodeAsString( result ) );
                }

                cout<<"Please call SetDACRange(deviceIndex, Operation code) to change the range of operation \n";

            } else
                cout<< "Failed to find USB-AO16-16A device " <<endl;
        } else
            cout<< "No ACCES devices found on USB bus " <<endl;


    }

}





void USBAO168E10B::DACwrite(vector<float> UsrDefVoltage)
{

    AIOUSB_GetDeviceProperties( deviceIndex, &properties );//gets the device properties

    if (UsrDefVoltage.size()<=8)
    {

        unsigned short dacData[ MAX_CHANNELS * 2 ];        // channel/count pairs
        for( int channel = 0; channel < ( int ) properties.DACChannels; channel++ )
        {
            dacData[ channel * 2 ] = channel;
            dacData[ channel * 2 + 1 ] =  (((UsrDefVoltage[channel] - minValue)/range) * MAX_COUNTS + 0.5);

        }

        unsigned long result = DACMultiDirect( deviceIndex, dacData, properties.DACChannels); //writes the user fed voltages(Vector) to all the available channels for the particular device

        if( result == AIOUSB_SUCCESS )
        {
            printf( "D/A counts successfully output to %u channels simultaneously\n", properties.DACChannels );
            cout<<"on device index :"<<deviceIndex<<endl;
        } else
            printf( "Error '%s' attempting to output D/A counts to %u channels simultaneously\n", AIOUSB_GetResultCodeAsString( result ) , properties.DACChannels );
    }

    else {
        cout<<"Channel input has exceeded the number of Channels. Revisit the data that you want to feed the channel"<<endl;
    }
}



void USBAO168E10B:: Sngl_Chnl_DACwrite(int TEST_CHANNEL,int Sngl_Voltage) //writes user fed voltage to the desired channel
{


    unsigned counts = ((Sngl_Voltage-minValue )/range)*MAX_COUNTS+0.5;	// half of full scale ( if we work on half the counts)
    unsigned long result = DACDirect( deviceIndex, TEST_CHANNEL, counts );
    if( result == AIOUSB_SUCCESS )
        printf( "%u D/A counts successfully output to channel %d\n", counts, TEST_CHANNEL );
    else
        printf( "Error '%s' attempting to output %u D/A counts successfully to channel %d\n", AIOUSB_GetResultCodeAsString( result ), counts, TEST_CHANNEL );

}


USBAO168E10B::~USBAO168E10B()
{
   // vector<float> UsrDefVoltage(8,0);
    //DACwrite(UsrDefVoltage);
    // cout<<"Exiting the terminal"<<endl;

}



void USBAO168E10B::digitalconfig(vector<int> outmask)
{
    unsigned short initialdata[16]={1,1,0,0,1,0,0,0,0,0,0,0,0,0,0,0};
    unsigned long tristate=AIOUSB_FALSE;

    if (outmask.size()<=16)
    {
       unsigned short dioData[outmask.size()];        // channel/count pairs

        for( int channel = 0; channel < outmask.size(); channel++ )
        {
            dioData[ channel] = outmask[channel] ;
        }

    unsigned long result = DIO_Configure( deviceIndex,tristate,initialdata,dioData);
    if (result==AIOUSB_SUCCESS)
    {
        cout<<"pins have been successfully configured input and output ports"<<endl;

    }

    else
    {
        cout<< "Error "<<AIOUSB_GetResultCodeAsString( result )<<" configuring device at index"<<deviceIndex<<endl;
    }
}

}



void USBAO168E10B::digitalwrite(vector<int> digitalinput)

{

   unsigned short dioData[digitalinput.size()];        // channel/count pairs
    for( int channel = 0; channel < digitalinput.size(); channel++ )
    {
        dioData[channel] = digitalinput[channel] ;
    }


    unsigned long result = DIO_WriteAll(deviceIndex,dioData);
    if (result==AIOUSB_SUCCESS)
    {
        cout<<"Data written on all corresponding output ports"<<endl;
    }
    else
    {
        cout<<"Error in writing the data.Please try again "<<endl;
    }


}


void USBAO168E10B::digitalread()
{

    unsigned long result = DIO_ReadAll(deviceIndex,readbytes);
    if(result==AIOUSB_SUCCESS)
    cout<<"Data on all the ports read successfully"<<endl;
    else
        cout<<"Error in reading the ports.Please try again"<<endl;
}
































