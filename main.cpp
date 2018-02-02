#include "usbao168e10b.h"
#include<stdio.h>
#include<vector>
#include<iostream>
using namespace std;

int main()
{

    // create your device
    // set some voltages...


    USBAO168E10B a(0x40e4d0b06773f278);//a(0x40e4d0aff80a3801);
    //a.SetDACRange(a.PM10);
    //DACSetBoardRange(0,1);
   vector<float>pad={2,3.5,5,7,1,3,8,9};
    a.DACwrite(pad);
 //   vector<int>vin={1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
  // cout<< vin.size()<<endl;
   // a.digitalconfig(vin);
  //vector<int> vik={1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
   //a.digitalwrite(vik);





    return 0;
}
