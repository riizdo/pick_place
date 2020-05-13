/*  file: serial_lib.h
 *  author: riizdo
 *  date: 12/05/2020
 *  version: 0.0
 *  description: h file for serial_lib 
 */

 #ifndef serial_lib_h
 #define serial_lib_h

 #include "Arduino.h"

 class Serial_lib {
  public:
    Serial_lib(int port);//constructor

    void init();
    void listener();
    
  private:
    typedef enum {_SOH = 1, _STX = 2, _ETX = 3} _tConstProtocol;

    typedef struct {    //data list type
      int nElement;
      int *element;
    } tDataList;

    const unsigned long int _BAUDRATE = 115200;   //baudrate comunication
    const int _CHECK_READ_IN = 6;   //checksum of diferent protocol
    const int _CHECK_READ_OUT = 3;
    const int _CHECK_WRITE_IN = 5;
    const int _CHECK_WRITE_OUT = 4;
    
    int _protocolReadIn[8];   //data write input protocol
    int _protocolReadOut[5];  //write data response
    int _protocolWriteIn[7];  //data read input protocol
    int _protocolWriteOut[6]; //read data response

    int _port;            //established port of comunication
    int _estation;        //station number
    int _command;         //type of parameter
    int _nData;           //number of parameter
    int _data;            //parameter value
    int _checksum;        //comprobate the transmission
    tDataList _dataList;  //data list of data of transmission
    bool _empty;          //data list empty or half-transmitted

    void _listInit(tDataList *list);
    void _readBuffer(tDataList *list, int n);
 };

 #endif
