#include "hbcli.h"


HbCli::HbCli(const char* server, const unsigned int port, bool isFrame)
{
    socket.reset(new TSocket(server, port));
    if(isFrame){
        transport.reset(new TFramedTransport(socket));
    }else{
        transport.reset(new TBufferedTransport(socket));
    }
    protocol.reset(new TBinaryProtocol(transport));
    client = new HbaseClient(protocol);
    _is_connected = false;
}

HbCli::~HbCli()
{
    if(NULL != client){
        delete client;
    }
    disconnect();
}


bool HbCli::connect()
{
    _is_connected = false;
    try{
        transport->open();
        _is_connected = true;
    }catch(const TException &tx){
        std::cerr << "ERROR: " << tx.what() << std::endl;
    }
    return _is_connected;
}


bool HbCli::disconnect()
{
    try{
        transport->close();
        _is_connected = false;
        return true;
    }catch(const TException &tx){
        std::cerr << "ERROR: " << tx.what() << std::endl;
    }
    return false;
}


bool HbCli::reconnect()
{
    bool flag = disconnect();
    flag = flag & connect();
    return flag;
}


inline bool HbCli::isconnect()
{
    return _is_connected;
}


bool HbCli::getTables(vector<string> &tables)
{
    if(!isconnect())
        return false;
    try{
        client->getTableNames(tables);
    }catch (const IOError &ioe) {
        std::cerr << "FATAL: Scanner raised IOError" << std::endl;
        return false;
    }catch (const TTransportException &tte) {
        std::cerr << "ERROR: " << tte.what() << std::endl; 
        return false;
    }
    return true;
}


bool HbCli::scan(const string table, const string startRow, const vector<string> columns,
        const map<string, string> attrs, vector<TRowResult> &values)
{
    int scanner;
    const int32_t nRows = 100;
    vector<TRowResult>::const_iterator it;

    if(!isconnect())
        return false;
    scanner = client->scannerOpen(table, startRow, columns, attrs);
    try{
        while(true){
            vector<TRowResult> value;
            client->scannerGetList(value, scanner, nRows);
            if(0 == value.size())
                break;
            for(it = value.begin(); it != value.end(); ++it)
                values.push_back(*it);
        }
    }catch (const IOError &ioe) { 
        std::cerr << "FATAL: Scanner raised IOError" << std::endl;
        client->scannerClose(scanner);
        return false;
    }catch (const TTransportException &tte) { 
        std::cerr << "ERROR: " << tte.what() << std::endl; 
         _is_connected = false; 
        client->scannerClose(scanner); 
        return false;
    }
    client->scannerClose(scanner);
    return true;
}


bool HbCli::scanWithStop(const string table, const string startRow, const string stopRow,
        const vector<string> columns, const map<string, string> attrs, 
        vector<TRowResult> &values)
{
    int scanner;
    const int32_t nRows = 100;
    vector<TRowResult> value;
    vector<TRowResult>::const_iterator it;

    if(!isconnect())
        return false;
    client->scannerOpenWithStop(table, startRow, stopRow, columns, attrs);
    try{
        while(true){
            client->scannerGetList(value, scanner, nRows);
            if(0 == value.size())
                break;
            for(it = value.begin(); it != value.end(); ++it)
                values.push_back(*it);
        }
    }catch (const IOError &ioe) { 
        std::cerr << "FATAL: Scanner raised IOError" << std::endl;
        client->scannerClose(scanner);
        return false;
    }catch (const TTransportException &tte) { 
        std::cerr << "ERROR: " << tte.what() << std::endl; 
         _is_connected = false; 
        client->scannerClose(scanner); 
        return false;
    }
    client->scannerClose(scanner);
    return true;
}


