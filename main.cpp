//
// With code generated from Simulink model 'Controlador'.
//
// Model version                  : 1.18
// Simulink Coder version         : 8.6 (R2014a) 27-Dec-2013
// C/C++ source code generated on : Wed Jun 14 19:41:45 2017
//
// Target selection: ert.tlc
// Embedded hardware selection: 32-bit Generic
// Code generation objectives: Unspecified
// Validation result: Not run
//

#include <stddef.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include "Controlador.h"
#include "Planta.h"
#include "rtwtypes.h"

#include <dirent.h>
#include <string.h>
#include <fcntl.h>
#include <stdlib.h>
#include <iostream>
#include <ctime>
#include <fstream>

#include <errno.h>
#include <signal.h>
#include <cmath>
#include "GPIOClass.h"

#include <sstream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include "OpenTsdb_client.h"
#define   OpenTSDB_Srv  "201.196.72.74"
#define   cte_testab 80
#define   cte_tinicio 360 //6 minutos

using namespace std;

///INICIALIZACION DE HARDWARE: IR, GPIO Y TEMPERATURA
///+++++++++++++++++++++++++++++++++++++++++++++++++++

//Lectura de temperatura
DIR *dir;
struct dirent *dirent;
char dev[16];      // Dev ID
char devPath[128]; // Path de dispositivo
char buf[256];     // Datos desde el dispositivo
char tmpData[6];   // Temp C * 1000 reportada por el dispositivo
char path[] = "/sys/bus/w1/devices";
ssize_t numRead;
int fd;

float tempActual;
time_t now;
tm *treal;

//GPIO
string inputstate1, inputstate2;
GPIOClass* gpio21 = new GPIOClass("21");
GPIOClass* gpio22 = new GPIOClass("22");

static real_T consignaTemporal;

//Mutex de diversas variables
pthread_mutex_t mutex_consigna,mutex_tsdb,mutex_numpulsos,mutex_leertemp;

//Variable ingresadas a OpenTSDB
static real_T v_Value;
static real_T v_Consigna;

//Conversion a numero de pulsos para aire acondicionado
real_T t_refresc,diffU,antU;
int numPulsos = 0;
bool firstRun = true;

//Tiempo de normalizacion de temp para inicio
int cuenta_inicio = cte_tinicio;

//Captura de Cntrl+C
bool ctrl_c_pressed = false;

void sig_handler(int sig)
{
    write(0,"nCtrl^C pressed in sig handlern",32);
    ctrl_c_pressed = true;
}


///METODO DE LECTURA DE TEMPERATURA
///+++++++++++++++++++++++++++++++++

real_T read_temperature()
{
    fd = open(devPath, O_RDONLY);

    if(fd == -1) {
        perror ("Couldn't open the w1 device.");
        return 1;
    }

    float tempC;
    real_T response;

    while((numRead = read(fd, buf, 256)) > 0) {
        strncpy(tmpData, strstr(buf, "t=") + 2, 5);
        tempC = strtof(tmpData, NULL) / 1000;
    }
    close(fd);

    response = tempC;

    return response;
}


///OpenTSDB
///+++++++++

void* thOpenTSDB_Client(void*)
{
  //Inicializacion de conexion con OpenTSDB
  OpenTsdb_client TagServer(OpenTSDB_Srv);
  TagServer.SetMetric("controlador.alonso.ucr");
  while(true)
  {
    //Thread principal de actualizacion de datos para historizador OpenTSDB
    pthread_mutex_lock(&mutex_tsdb);
    TagServer.TagValue("Tagname=TEMPERATURA",v_Value);
    TagServer.TagValue("Tagname=CONSIGNA",v_Consigna);
    pthread_mutex_unlock(&mutex_tsdb);

    usleep(1000000);
  }
  return NULL;
}


///CONTROLADOR
///++++++++++++

static ControladorModelClass Controlador_Obj; //Instancia de controlador

// '<Root>/Ref_in'
static real_T arg_Ref_in;
// '<Root>/Retro_in'
static real_T arg_Out1;
// '<Root>/Kp'
static real_T arg_Kp = 3.5;
// '<Root>/Ki'
static real_T arg_Ki = 0.0814;
// '<Root>/U'
static real_T arg_U;

//Escalon del controlador
void rt_OneStepController(void)
{
  static boolean_T OverrunFlag = false;

  if (OverrunFlag) {
    rtmSetErrorStatus(Controlador_Obj.getRTM(), "Overrun");
    return;
  }
  OverrunFlag = true;
  Controlador_Obj.step(&arg_Ref_in, &arg_Out1, &arg_Kp, &arg_Ki, &arg_U);
  OverrunFlag = false;
}

///PROCESO
///++++++++

static PlantaModelClass Planta_Obj; //Instancia de planta

// '<Root>/Inicial'
static real_T arg_Inicial;

//Escalon de la planta
void rt_OneStepProcess(void)
{
  static boolean_T OverrunFlag = false;
  if (OverrunFlag) {
    rtmSetErrorStatus(Planta_Obj.getRTM(), "Overrun");
    return;
  }

  OverrunFlag = true;
  Planta_Obj.step(arg_Out1, &arg_U, &arg_Inicial);
  OverrunFlag = false;
}

///THREADS
///++++++++

//Threads y banderas de activacion de threads
static pthread_t pthread_id,pthread_id2,pthread_gpio,pthread_tsdb,pthread_pulsos;
static volatile bool enable_th=false,enable_plan_th=false,enable_gpio_th=false,enable_pulsos=false;

//Corrida de un escalon en la instancia del controlador
void* ControlThread(void*)
{
  while(true){
    while (!enable_th);
    rt_OneStepController();

    sleep(1);
  }
  return NULL;
}

//Corrida de un escalon en la instancia de la planta
void* ProcessThread(void*)
{
  while(true){
    while (!enable_plan_th);
    rt_OneStepProcess();

    sleep(1);
  }
  return NULL;
}

//Establecimiento del numero de pulsos para orden al aire acondicionado
void* PulsosThread(void*)
{
  while(true){

    while (!enable_pulsos);

    usleep(500000);

    pthread_mutex_lock(&mutex_numpulsos);

    if (numPulsos != 0){

        if (numPulsos > -1 && numPulsos < 1){
            numPulsos = 0;
        } else if (numPulsos >= 1){
            --numPulsos;
            pthread_mutex_lock(&mutex_leertemp);
            system("irsend SEND_ONCE aire1up KEY_UP");
            pthread_mutex_unlock(&mutex_leertemp);
        }else if (numPulsos <= -1){
            ++numPulsos;
            pthread_mutex_lock(&mutex_leertemp);
            system("irsend SEND_ONCE aire1down KEY_DOWN");
            pthread_mutex_unlock(&mutex_leertemp);
        }

    }

    pthread_mutex_unlock(&mutex_numpulsos);

  }
  return NULL;
}

//Captura de pulsacion de teclas
void* GPIOThread(void*)
{
  while(true){

    while (!enable_gpio_th);

    usleep(1000);

    //Se leen los valores de las entradas GPIO
    gpio21->getval_gpio(inputstate1);
    gpio22->getval_gpio(inputstate2);

    if(inputstate1 == "1" || inputstate2 == "1"){

        usleep(10000);

        //Se verifica que se detecta la senal nuevamente
	    gpio21->getval_gpio(inputstate1);
        gpio22->getval_gpio(inputstate2);

        if(inputstate1 == "1")
        {
            //Tecla esta presionada definitivamente
            pthread_mutex_lock(&mutex_consigna);
            consignaTemporal=consignaTemporal+0.5;
            pthread_mutex_unlock(&mutex_consigna);

            //Se espera hasta que la tecla se libere
            while (inputstate1 == "1"){
                gpio21->getval_gpio(inputstate1);
            };

        }

	    if(inputstate2 == "1")
	    {
            //Tecla esta presionada definitivamente
            pthread_mutex_lock(&mutex_consigna);
            consignaTemporal=consignaTemporal-0.5;
            pthread_mutex_unlock(&mutex_consigna);

             //Se espera hasta que la tecla se libere
            while (inputstate2 == "1"){
                gpio22->getval_gpio(inputstate2);
            };
	    }

    }
  }
  return NULL;
}

///MAIN
///+++++

int_T main(int_T argc, const char *argv[])
{
  //Argumentos no usados
  (void)(argc);
  (void)(argv);

  //Inicializacion de dispositivo lirc0 de LIRC
  system("sudo /etc/init.d/lirc restart");
  system("sudo lircd --device /dev/lirc0");

  //Conexion con dispositivo de lectura de temperatura
  dir = opendir (path);
  if (dir != NULL) {

    while ((dirent = readdir (dir)))

    //Los dispositivos que usan protocolo 1-wire tienen links que empiezan con 28-
    if (dirent->d_type == DT_LNK && strstr(dirent->d_name, "28-") != NULL)
        strcpy(dev, dirent->d_name);

    (void) closedir (dir);
  } else {
    perror ("Couldn't open the w1 devices directory");
    return 1;
  }
  sprintf(devPath, "%s/%s/w1_slave", path, dev);

  //Captura de senal ctrl+C
  struct sigaction sig_struct;
  sig_struct.sa_handler = sig_handler;
  sig_struct.sa_flags = 0;
  sigemptyset(&sig_struct.sa_mask);

  if (sigaction(SIGINT, &sig_struct, NULL) == -1) {
    cout << "Problem with sigaction" << endl;
    exit(1);
  }

  //Configuracion de puertos GPIO
  gpio21->export_gpio();
  gpio22->export_gpio();

  gpio21->setdir_gpio("in");
  gpio22->setdir_gpio("in");

  //Inicializacion de parametros del controlador
  arg_Inicial = read_temperature();

  //Valores iniciales para consigna del controlador y salida de la planta
  arg_Ref_in = arg_Inicial;
  consignaTemporal = arg_Inicial;
  arg_Out1 = arg_Inicial;

  //Valores iniciales para el historizador de datos
  v_Value = arg_Inicial;
  v_Consigna = arg_Inicial;

  //Inicializacion de objeto controlador
  Controlador_Obj.initialize();
  ///Planta_Obj.initialize();

  //Creacion de threads
  pthread_create(&pthread_gpio,NULL,GPIOThread,NULL);
  pthread_create(&pthread_pulsos,NULL,PulsosThread,NULL);
  pthread_create(&pthread_id,NULL,ControlThread,NULL);
  ///pthread_create(&pthread_id2,NULL,ProcessThread,NULL);

  //Thread de OpenTSDB
  cout << "Connection with OpenTSDB host= " << OpenTSDB_Srv << endl;
  if(pthread_create(&pthread_tsdb, NULL, thOpenTSDB_Client, NULL))
  {
      cerr << "Error creating thread"<< endl;
      return 1;
  }

  fflush((NULL));

  //Habilitacion de threads de uso de gpio, y esclaon de controlador
  enable_gpio_th=true;
  enable_th=true;
  ///enable_plan_th=true;

  while (
     rtmGetErrorStatus(Controlador_Obj.getRTM()) == (NULL) ///&& rtmGetErrorStatus(Planta_Obj.getRTM()) == (NULL)
        ) {

    //Deteccion de ctrl+C
    if(ctrl_c_pressed){
        cout << "Ctrl^C Pressed" << endl;
        cout << "unexporting pins" << endl;
        gpio21->unexport_gpio();
        gpio22->unexport_gpio();
        cout << "deallocating GPIO Objects" << endl;
        delete gpio21;
        gpio21 =0;
        delete gpio22;
        gpio22 =0;
        exit(1);
    }

    ///sleep(1);

    //Esto introduce un retraso de 1 seg, se lee la temperatura y se imprime a pantalla
    pthread_mutex_lock(&mutex_leertemp);
    arg_Out1 = read_temperature();
    pthread_mutex_unlock(&mutex_leertemp); //////////////////////////////

    //Establecimiento de la consigna principal
    pthread_mutex_lock(&mutex_consigna);
    arg_Ref_in = consignaTemporal;
    pthread_mutex_unlock(&mutex_consigna);

    //Valores para el historizador de datos
    pthread_mutex_lock(&mutex_tsdb);
    v_Value=arg_Out1;
    v_Consigna=arg_Ref_in;
    pthread_mutex_unlock(&mutex_tsdb);

    //Establecimiento del numero de pulsos para el aire acondicionado, ademas espera de tiempo de establecimiento inicial
    /*if (cuenta_inicio > 0){
        --cuenta_inicio;
        cout << cuenta_inicio << endl; ///prueba
    }else {*/
        if (firstRun){
            cout << "Empieza a correr" << endl; ///prueba
            antU = arg_U;
            numPulsos = 0;
            enable_pulsos=true;
            t_refresc = 0;
            firstRun = false;
        } else {

            ++t_refresc;

            diffU = arg_U - antU;

            ///cout << "tick..." << endl; ///prueba

            if (abs(diffU) > 0.5 || t_refresc > cte_testab){

                cout << "Actualiza pulsos" << endl; ///prueba
                pthread_mutex_lock(&mutex_numpulsos);
                numPulsos = numPulsos + (diffU * 20);
                cout << "Numero de pulsos: " << numPulsos << endl; ///prueba
                pthread_mutex_unlock(&mutex_numpulsos);

                t_refresc = 0;
                antU = arg_U;
            }

        }

    ///}

    cout << "Consigna: " << arg_Ref_in << ", Temperatura planta: " << arg_Out1 << ", U: " << arg_U << ", DiffU: " << diffU << endl; //"\r";

  }

  return 0;
}
