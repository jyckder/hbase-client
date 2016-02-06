#include<iostream>
#include <vector>
#include <map>
#include <string>
#include "hbcli.h"


int main()
{
    HbCli hbcli = HbCli("localhost", 9090, true);
    if(!hbcli.connect()){
        cout<<"hbcli connect fail"<<endl;
        return -1;
    }
    vector<string> tables;
    vector<string>::iterator it_ve;
    if(!hbcli.getTables(tables)){
        cout<<"get tables fail"<<endl;
        return -1;
    }
    for(it_ve = tables.begin(); it_ve != tables.end(); ++it_ve){
        cout<<"table:"<<*it_ve<<endl;
    }
    vector<TRowResult> values;
    map<string, string> attrs;
    vector<string> columns;
    columns.push_back("f1");
    if(!hbcli.scan("test", "", columns, attrs, values)){
        cout<<"scan hbase fail"<<endl;
        return -1;
    }

    vector<TRowResult>::iterator it;
    map<string, TCell>::iterator it_map;
    for(it = values.begin(); it != values.end(); ++it){
        for(it_map = it->columns.begin(); it_map != it->columns.end(); ++it_map){
            cout<<"row:"<<it->row<<" column:"<<it_map->first<<" value:"<<it_map->second.value<<endl;
        }
    }
    return 0;
}













/*
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

void scan(HbaseClient *client, Text startRow, vector<Text> columns, vector<TRowResult> &values)
{
    map<Text, Text> map;
    Text table = "test";
    const int32_t nRows = 1;
    int scanner = client->scannerOpen(table, startRow, columns, map); 
    std::vector<TRowResult>::const_iterator it;
    while (true) {
        std::vector<TRowResult> value;
        client->scannerGetList(value, scanner, nRows);
        cout<<"one"<<endl;
        if (value.size() == 0)
            break;
        for (it = value.begin(); it != value.end(); ++it)
            values.push_back(*it);
    }
    client->scannerClose(scanner);
}


void getTables(HbaseClient *client)
{
    vector<string> tables;
    client->getTableNames(tables);
    vector<string>::iterator it;
    for(it = tables.begin(); it != tables.end(); it++)
    {
        cout<<"table:"<<*it<<endl;
    }
}

int main()
{
    boost::shared_ptr<TSocket> socket(new TSocket("10.108.72.107", 9090));
    boost::shared_ptr<TTransport> transport(new TBufferedTransport(socket));
    boost::shared_ptr<TProtocol> protocol(new TBinaryProtocol(transport));
    HbaseClient client(protocol);
    transport->open();
    vector<Text> columns;
    Text startRow("");


    getTables(&client);
    columns.push_back("f1");
    vector<TRowResult> values;
    scan(&client, startRow, columns, values);
    vector<TRowResult>::iterator it;
    map<Text, TCell>::iterator it_map;
    for(it = values.begin(); it != values.end(); it++)
    {
        for(it_map = it->columns.begin(); it_map != it->columns.end(); it_map++)
        {
            cout<<"row:"<<it->row<<" column:"<<it_map->first<<" value:"<<it_map->second.value<<endl;
        }
    }
    return 0;
}
*/





