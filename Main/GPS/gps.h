#ifndef _GPS_H_
#define _GPS_H_

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

void read_GPS_Data(char *Gps_Buffer);
void parse_GpsDATA();
void save_GPS_Data();
void RST_Buffer(char *Buffer);
void insert_array(char *buff);



struct
{
  char GPS_DATA[80];
  char GetData_Flag; //Get GPS Data Flag
  char ParseData_Flag; //Parsing complete flag
  char UTCTime[12]; //UTC time
  char Slatitude[15]; //latitutde
  char N_S[2]; //N/S
  char Slongitude[15]; //longitude
  char E_W[2]; //E/W
  char Usefull_Flag; //Whether the positioning information is valid flag
} Save_Data;



void read_GPS_Data(char *Gps_Buffer)
{
  char* GPS_DATAHead;
  char* GPS_DATATail;
  if ((GPS_DATAHead = strstr(Gps_Buffer, "$GPRMC,")) != NULL || (GPS_DATAHead =strstr(Gps_Buffer, "$GNRMC")) != NULL ){
    if (((GPS_DATATail = strstr(GPS_DATAHead, "\n")) != NULL) && (GPS_DATATail >GPS_DATAHead)){
      memset(Save_Data.GPS_DATA, 0, strlen(Save_Data.GPS_DATA));
      memcpy(Save_Data.GPS_DATA, GPS_DATAHead, GPS_DATATail - GPS_DATAHead);
      Save_Data.GetData_Flag = 1;
    }
  }
}

void parse_GpsDATA()
{
  char *subString;
  char *subStringNext;
  if (Save_Data.GetData_Flag){
    Save_Data.GetData_Flag = 0;

    //printf("%s\n",Save_Data.GPS_DATA);

    for (int i = 0 ; i <= 6 ; i++){
      if (i == 0){
        if ((subString = strstr(Save_Data.GPS_DATA, ",")) == NULL){
          printf("ERROR\n");//Parsing error
        }
      }else{
        subString++;
        if ((subStringNext = strstr(subString, ",")) != NULL){
          char buffer[12];
          char usefullBuffer[2];
          switch(i){
            case 1:RST_Buffer(Save_Data.UTCTime);
              memcpy(Save_Data.UTCTime, subString, subStringNext - subString);break; //Obtain UTC time
            case 2:RST_Buffer(usefullBuffer);
              memcpy(usefullBuffer, subString, subStringNext - subString);break;//Get location status
            case 3:RST_Buffer(Save_Data.Slatitude);
              memcpy(Save_Data.Slatitude, subString, subStringNext - subString);
            case 4:RST_Buffer(Save_Data.N_S);
              memcpy(Save_Data.N_S, subString, subStringNext - subString);break;//Obtain N/S
            case 5:RST_Buffer(Save_Data.Slongitude);
              memcpy(Save_Data.Slongitude, subString, subStringNext - subString);
            case 6:RST_Buffer(Save_Data.E_W);
              memcpy(Save_Data.E_W, subString, subStringNext - subString);break;//Obtain E/W
            default:break;
          }
          subString = subStringNext;
          Save_Data.ParseData_Flag = 1;
          if(usefullBuffer[0] == 'A')
            Save_Data.Usefull_Flag = 1;
          else 
            if(usefullBuffer[0] == 'V')
              Save_Data.Usefull_Flag = 0;
        }else{
          printf("ERROR\n");//Parsing Error
        }
      }
    }
  }
}

void save_GPS_Data() 
{
    FILE* saveFile;
    saveFile = fopen("GPSDATA.txt", "w");
    fprintf(saveFile, strcat(Save_Data.Slatitude, "\n"));
    fprintf(saveFile, strcat(Save_Data.Slongitude, "\n"));
    fprintf(saveFile, strcat(Save_Data.N_S, "\n"));
    fprintf(saveFile, strcat(Save_Data.E_W, "\n"));
    fclose(saveFile);
}

void RST_Buffer(char *Buffer)
{
  memset(Buffer, 0, strlen(Buffer));
}

void insert_array(char *buff)
{
  char *p;
  int place = 0;
  char sbuffer[20]={0};
  p = strstr( buff , ".");
  place = p-buff;
  //printf("%d\n", place);
  for(int i=0; i<place-2; i++){
    sbuffer[i]=buff[i];
  }
  sbuffer[place-2]=' ';
  for(int i= place-1; buff[i-1]!='\0'; i++){
    sbuffer[i]=buff[i-1];
  }
  for(int i=0; sbuffer[i]!='\0'; i++){
    buff[i]=sbuffer[i];
  }
}



 


#endif
