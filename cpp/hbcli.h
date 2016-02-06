#ifndef HBCLI_H_
#define HBCLI_H_

#include<iostream>
#include <vector>
#include <map>
#include <string>
#include <boost/lexical_cast.hpp>
#include <protocol/TBinaryProtocol.h>
#include <transport/TSocket.h>
#include <transport/TTransportUtils.h>
#include "Hbase.h"
using namespace std;

using namespace apache::thrift;
using namespace apache::thrift::protocol;
using namespace apache::thrift::transport;
using namespace apache::hadoop::hbase::thrift;



class HbCli{
public:
    HbCli(const char* server, const unsigned int port, bool isFrame = false);
    ~HbCli();

    bool connect();
    bool disconnect();
    bool reconnect();
    inline bool isconnect();
 
    bool getTables(vector<string> &tables);
    bool scan(const string table, const string startRow, const vector<string> columns, 
            const map<string, string> attrs, vector<TRowResult> &values);
    bool scanWithStop(const string table, const string startRow, const string stopRow,
            const vector<string> columns, const map<string, string> attrs, 
            vector<TRowResult> &values);
private:
    boost::shared_ptr<TSocket> socket;
    boost::shared_ptr<TTransport> transport;
    boost::shared_ptr<TProtocol> protocol;
    HbaseClient* client;
    bool _is_connected;
};


#endif
