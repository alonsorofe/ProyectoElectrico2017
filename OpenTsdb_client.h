#ifndef OPENTSDB_CLIENT_H
#define OPENTSDB_CLIENT_H
class OpenTsdb_client
{
private:
  string Host,Metric;
  int    Port;
  int sockfd, n;
  struct sockaddr_in serv_addr;
  struct hostent *server;
  bool   connected;
  long long ms_timestamp(void);


protected:
    bool Connection(void);

public:
    OpenTsdb_client(string host,int port=4242);
    virtual ~OpenTsdb_client();
    bool SetMetric(string metric);
    bool TagValue(string TagRef,float Val);
};///_________________________________________________

#endif // OPENTSDB_CLIENT_H
