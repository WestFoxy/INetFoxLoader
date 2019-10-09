

#pragma once
#include "SDK.h"

using namespace std;

class Internet
{
private:
	const char *data;

public:
	Internet();
	~Internet();

	void GetFile(const char*);
	
	char *SendHTTPSRequest(LPCSTR hostname, LPCSTR params, DWORD *lenght);

	void Get(const char *, const char *, const char *, std::string *, std::string *);

	//const char *hostname, const char *data, const char *headers, const char *optional, string *answer, string *answerheaders
	void Post(const char *, const char *, const char *, const char *, std::string *, std::string *);
};

class FFoxInternet : public ConsoleOut
{
private:
	string host;

	string GotData;
	string OutHeaders;
	string InHeaders;
public:
	FFoxInternet(const char *hostname);
	~FFoxInternet();

	void AddHeader(const char *Row, const char *Dat);
	void ClearHeaders();
	void Get(const char *hostdata, const char *method);
	void Post(const char *hostdata);

	const char *GetRecievedData();
	const char *GetRecievedHeaders();
};

class FFoxUpdate : public ConsoleOut
{
private:
	SOCKET sConnetction;
	WSADATA wsaData;

	char *RecievedData;
	DWORD dwRecievedDataLenght;

	void connect();
	void closeConnection();
	void sendData(const char *data, DWORD lenght);

public:
	FFoxUpdate(string szAddress, UINT32 dwport);
	~FFoxUpdate();

	void CheckUpdates();


};