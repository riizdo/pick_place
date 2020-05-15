/*  file: serial_lib.h
 *  author: riizdo
 *  date: 12/05/2020
 *  version: 0.0
 *  description: h file for serial_lib 
 */

 #ifndef serial_lib_h
 #define serial_lib_h

 #include "Arduino.h"

 #define _BAUDRATE 115200       //speed of transmission
 #define _CHECK_READ_IN 6       //number of checksum of the read data in buffer 
 #define _CHECK_READ_OUT 3      //number of checksum of the response transmission
 #define _CHECK_WRITE_IN 5
 #define _CHECK_WRITE_OUT 4

typedef enum {_SOH = 1, _STX = 2, _ETX = 3} _tConstProtocol;

typedef struct {    //data list type
  int nElement;
  int *element;
} tDataList;

#ifndef tError
typedef enum {  OK,                           //type of error of class
                ERROR_MEMORY,
                ERROR_TRANSMISSION
} tError;
#endif

 class Serial_lib {
  public:
    Serial_lib(int port, int und = 0);//constructor

    void init();
    void listener();
    void show();
    
  private:
    int _protocolReadIn[8];   //data write input protocol
    int _protocolReadOut[5];  //write data response
    int _protocolWriteIn[7];  //data read input protocol
    int _protocolWriteOut[6]; //read data response

    int _port;            //established port of comunication
    int _station;        //station number
    int _command;         //type of parameter
    int _nData;           //number of parameter
    int _data;            //parameter value
    int _checksum;        //comprobate the transmission
    tDataList _dataList;  //data list of data of transmission
    bool _empty;          //data list empty or half-transmitted
    tError _error;        //error log

    void _listInit(tDataList *list);
    void _readBuffer(tDataList *list, int n);
 };

 #endif
