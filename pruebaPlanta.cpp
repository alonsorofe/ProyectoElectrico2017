#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <iostream>
#include <ctime>
#include <fstream>

//#include <errno.h>
//#include <signal.h>
//#include "GPIOClass.h"

using namespace std;

DIR *dir;
struct dirent *dirent;
char dev[16];      // Dev ID
char devPath[128]; // Path to device
char buf[256];     // Data from device
char tmpData[6];   // Temp C * 1000 reported by device 
char path[] = "/sys/bus/w1/devices"; 
ssize_t numRead;
int fd;
float tempActual;
time_t now;
tm *treal;
ofstream salida1;
ofstream salida2;
int datos = 0;
int hora,minutos,seg;

float read_temperature();

//void sig_handler(int sig);
//bool ctrl_c_pressed = false;
     
int main (int argc, char** argv) {

  //esto es configurar correctamente el dispositivo /dev/lirc0
  system("sudo /etc/init.d/lirc restart");
  system("sudo lircd --device /dev/lirc0");

  enum valor {inicio=1,calibracion,medicion,cierre};
  valor estado = inicio;

  while(true){

	switch (estado)
	{
	    case inicio:

		dir = opendir (path);
		if (dir != NULL) {
		    while ((dirent = readdir (dir)))
			// 1-wire devices are links beginning with 28-
			if (dirent->d_type == DT_LNK && strstr(dirent->d_name, "28-") != NULL) { 
			    strcpy(dev, dirent->d_name);	
			    //printf("\nDevice: %s\n", dev);
			}
		        (void) closedir (dir);
		} else {
		    perror ("Couldn't open the w1 devices directory");
  		    return 1;
		}

		// Assemble path to OneWire device
		sprintf(devPath, "%s/%s/w1_slave", path, dev);
		// Read temp continuously
		// Opening the device's file triggers new reading

		estado = calibracion; 
		//salida1.open("plantaEscalonFull.txt"); 
		//salida2.open("plantaEscalon.txt"); 
		//estado = medicion; 

		break;

	    case calibracion:

		cout << "estado: calibracion" << endl;

		usleep(50000000); //tiempo estimado para que la temperatura se estabilice con el aire acondicionado (50s)

		tempActual = read_temperature();

		if (tempActual > 26){
		    cout << "Temperatura sobre el rango de 25, temp: " << tempActual << endl << endl;
		    system("irsend SEND_ONCE aire1 KEY_DOWN");
		    estado = calibracion;
		} else {

		    if (tempActual < 24){
		        cout << "Temperatura bajo el rango de 25, temp: " << tempActual << endl << endl;
		        system("irsend SEND_ONCE aire1 KEY_UP");
		        estado = calibracion;
		    } else {
			cout << "Temperatura en el rango escogido para iniciar la prueba de planta" << endl << endl;
			salida1.open("plantaEscalonFull.txt"); //se abre el archivo con la respuesta de la planta al escalon
		        salida2.open("plantaEscalon.txt");
			estado = medicion;
		    }
		}

		break;

	    case medicion:

		//cout << "Hora: " << hora << " : " << minutos << " : " << seg << endl << endl;

		if (datos == 0){

		    tempActual = read_temperature();

		    now = time(0);
		    treal = localtime(&now);

		    hora = treal->tm_hour;
		    minutos = treal->tm_min;
		    seg = treal->tm_sec;

		    salida1 << "Prueba de planta, respuesta al escalon, escalon de 3 ordenes" << endl;
		    salida1 << "Temperatura inicial: " << tempActual << ", Hora actual: " << hora;
		    salida1 << ":" << minutos << ":" << seg << endl << endl;
		    
		    for (int j = 0; j < 10;j++){ //este es el escalon 
		        system("irsend SEND_ONCE aire1up KEY_UP"); 
		        usleep(500000); //medio segundo
		    }
		    

		    salida1 << datos << " " << tempActual << " (Hora: " << hora << ":" << minutos << ":";
		    salida1 << seg << ")" << endl;

		    salida2 << tempActual << endl;

		    ++datos;
		    estado = medicion;
		} else {

		    if (datos > 1200) //si es >59 se guardan datos aproximadamente 1 por segundo, por 1 min
			estado = cierre;
		    else {
			tempActual = read_temperature();

			now = time(0);
			treal = localtime(&now);

			hora = treal->tm_hour;
			minutos = treal->tm_min;
			seg = treal->tm_sec;

			//salida << hora << minutos << seg << " " << tempActual << endl;
			salida1 << datos << " " << tempActual << " (Hora: " << hora << ":" << minutos << ":" << seg;
			salida1 << ")" << endl;

			salida2 << tempActual << endl;

			++datos;
			estado = medicion;
		    }

		}

		//estado = medicion;

		break;

	    case cierre:

		//cout << "estado: cierre" << endl;

		salida1.close();
		salida2.close();
		return 0;

		break; //este break esta de mas

	    default:

		cout << "error de estado" << endl;

		return 1;
	
	}

  }

  return 1;
}


float read_temperature()
{
		fd = open(devPath, O_RDONLY);

		if(fd == -1) {
		    perror ("Couldn't open the w1 device.");
		    return 1;   
		}

		float tempC;

		while((numRead = read(fd, buf, 256)) > 0) {
		    strncpy(tmpData, strstr(buf, "t=") + 2, 5); 
		    tempC = strtof(tmpData, NULL) / 1000;
		    //printf("Device: %s  - ", dev); 
		    //printf("Temp: %.3f C  ", tempC / 1000);
		    //printf("%.3f F\n\n", (tempC / 1000) * 9 / 5 + 32);
		    //cout << tempC << endl;
		}
		close(fd);

		return tempC;
}


/*
void sig_handler(int sig)
{
    write(0,"nCtrl^C pressed in sig handlern",32);
    ctrl_c_pressed = true;
}
*/
