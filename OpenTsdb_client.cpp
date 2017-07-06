/**++++++++++++++++++++++++++++++++++++++++++++++++
 OpenTsdb_client.cpp
 Clase para la conexión con OpenTSDB remoto
+++++++++++++++++++++++++++++++++++++++++++++++++++*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <math.h>

#include <string>
#include <sstream>
#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

using namespace std;
#include "OpenTsdb_client.h"

long long OpenTsdb_client::ms_timestamp(void)
{
  long long miliseconds;
  struct  timespec current_time;

  clock_gettime(CLOCK_REALTIME,&current_time);
  long long seconds=current_time.tv_sec;
  miliseconds= seconds*1000+current_time.tv_nsec/1000000L;
  return miliseconds;
}//_________________________________________________________________________

bool OpenTsdb_client::Connection(void)
{
  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd < 0)
  {
    cerr<< "ERROR opening socket... wating 3 seconds."<< endl;
    sleep(3);
    close(sockfd);
    return false;
  }

  const char *hostname=Host.c_str();
  server = gethostbyname(hostname);

  if (server == NULL) {
      fprintf(stderr,"ERROR, no such host\n");
      close(sockfd);
      return false;
  }

  bzero((char *) &serv_addr, sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  bcopy((char *)server->h_addr,
       (char *)&serv_addr.sin_addr.s_addr,
       server->h_length);

  serv_addr.sin_port = htons(Port);
  cout << "Collector connecting to OpenTSDB..." << endl;
  if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0)
  {
      cerr<< "ERROR connecting" << endl;
      sleep(3);
      close(sockfd);
      return false;
  }
  return true;
}//____________________________________________________

OpenTsdb_client::OpenTsdb_client(string host,int port) : Host(host),Port(port),connected(false)
{
  Metric="";
  connected=Connection();
}//_________________________________________

OpenTsdb_client::~OpenTsdb_client()
{
  if(connected) close(sockfd);
}//_________________________________________

bool OpenTsdb_client::SetMetric(string metric)
{
  Metric=metric;
  return true;
}//_________________________________________

bool OpenTsdb_client::TagValue(string TagRef,float v_VALUE)
{
  if (!connected) connected=Connection();
  if (!connected) return false;

  long long  epoc_time=ms_timestamp();
  stringstream ss;
  ss.precision(30);
  ss << "put " << Metric << " " << epoc_time <<" " << v_VALUE << " "<< TagRef << endl;
  string str(ss.str());
  n = write(sockfd,str.c_str(),str.length());
  if (n < 0)
  {
     cerr <<" Communications error # "<< n << endl;
     connected =false;
     return false;
  }
  //cout << "TAG: " << str << endl;
  return true;
}//_________________________________________

