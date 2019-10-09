#include "./Internet.h"

using namespace std;

Internet::Internet()
{

	return;
}

Internet::~Internet()
{
	delete[] data;

}

void Internet::GetFile(const char *)
{

	return;
}

inline wstring CharPToWstring(const char* _charP)
{
	return wstring(_charP, _charP + strlen(_charP));
}

// send https request
char *Internet::SendHTTPSRequest(LPCSTR hostname, LPCSTR params, DWORD *lenght)
{
	char szData[1024];
	string answer;

	// initialize WinInet
	HINTERNET hInternet = ::InternetOpen(TEXT("Mozilla Firefox/5.0 (Windows NT 6.1; Win64; x64; rv:47.0)"), INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
	if (hInternet != NULL)
	{
		// open HTTP session
		HINTERNET hConnect = InternetConnectA(hInternet, hostname, INTERNET_DEFAULT_HTTPS_PORT, NULL, NULL, INTERNET_SERVICE_HTTP, 0, 0);
		if (hConnect != NULL)
		{

			HINTERNET hRequest = HttpOpenRequestA(hConnect, "GET", params, 0, NULL, 0, INTERNET_FLAG_HYPERLINK | INTERNET_FLAG_IGNORE_CERT_CN_INVALID |
				INTERNET_FLAG_IGNORE_CERT_DATE_INVALID |
				INTERNET_FLAG_IGNORE_REDIRECT_TO_HTTP |
				INTERNET_FLAG_IGNORE_REDIRECT_TO_HTTPS |
				INTERNET_FLAG_NO_AUTH |
				INTERNET_FLAG_NO_CACHE_WRITE |
				INTERNET_FLAG_NO_UI |
				INTERNET_FLAG_PRAGMA_NOCACHE |
				INTERNET_FLAG_RELOAD | INTERNET_FLAG_SECURE, NULL);
			if (hRequest != NULL)
			{
				BOOL isSend = HttpSendRequestA(hRequest, NULL, 0, NULL, 0);

				if (isSend)
				{

					DWORD dwSize = 0;
					LPVOID lpOutBuffer = NULL;


				retry:
					if (!HttpQueryInfo(hRequest, HTTP_QUERY_RAW_HEADERS_CRLF,
						(LPVOID)lpOutBuffer, &dwSize, NULL))
					{

						if (GetLastError() == ERROR_HTTP_HEADER_NOT_FOUND)
							goto cyc;
						else
						{
							if (GetLastError() == ERROR_INSUFFICIENT_BUFFER)
							{
								lpOutBuffer = new char[dwSize];

								goto retry;
							}
						}
					}

					cout << (char*)lpOutBuffer << endl;

				cyc:

					for (;;)
					{
						DWORD dwByteRead;

						BOOL isRead = InternetReadFile(hRequest, szData, sizeof(szData) - 1, &dwByteRead);

						if (isRead == FALSE || dwByteRead == 0)
							break;

						szData[dwByteRead] = 0;

						answer += szData;
					}
				}
				else
					cout << GetLastError() << endl;

				::InternetCloseHandle(hRequest);
			}
			// close session
			::InternetCloseHandle(hConnect);
		}
		// close WinInet
		::InternetCloseHandle(hInternet);
	}

	//cout << answer.c_str() << endl;
	
	*lenght = (DWORD)answer.length();

	char *response = new char[answer.length()];

	strcpy(response, answer.c_str());

	return response;
}

void Internet::Get(const char *hostname,const char *data, const char *headers, string *answer, string *answerheaders)
{
	DWORD BytesRead;
	DWORD dwSize = 0;

	char *lpOutBuffer = NULL;
	char DataBuffer[1024];

	HINTERNET InSetup, InConnect, InRequest;

	InSetup = InternetOpenA("Mozilla Firefox/5.0 (Windows NT 6.1; Win64; x64; rv:47.0)", INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, NULL);
	if (!InSetup)
		throw(29);

	InConnect = InternetConnectA(InSetup, hostname, INTERNET_DEFAULT_HTTPS_PORT, NULL, NULL, INTERNET_SERVICE_HTTP, NULL, NULL);
	if (!InConnect)
		throw(30);

	InRequest = HttpOpenRequestA(InConnect, "GET", data, NULL, NULL, NULL, INTERNET_FLAG_HYPERLINK | INTERNET_FLAG_IGNORE_CERT_CN_INVALID |
		INTERNET_FLAG_IGNORE_CERT_DATE_INVALID |
		INTERNET_FLAG_IGNORE_REDIRECT_TO_HTTP |
		INTERNET_FLAG_IGNORE_REDIRECT_TO_HTTPS |
		INTERNET_FLAG_NO_AUTH |
		INTERNET_FLAG_NO_CACHE_WRITE |
		INTERNET_FLAG_NO_UI |
		INTERNET_FLAG_PRAGMA_NOCACHE |
		INTERNET_FLAG_RELOAD | INTERNET_FLAG_SECURE, NULL);
	if (!InRequest)
		throw(31);


	DWORD lnght(0);
	if (headers != NULL)
		lnght = strlen(headers);

	BOOL isSend = HttpSendRequestA(InRequest, headers, lnght, NULL, 0);

	if (!isSend)
		throw(32);

retry:
	if (!HttpQueryInfo(InRequest, HTTP_QUERY_RAW_HEADERS_CRLF,
		(LPVOID)lpOutBuffer, &dwSize, NULL))
	{

		if (GetLastError() == ERROR_HTTP_HEADER_NOT_FOUND)
			goto cyc;
		else
		{
			if (GetLastError() == ERROR_INSUFFICIENT_BUFFER)
			{
				lpOutBuffer = new char[dwSize];

				goto retry;
			}
		}
	}

	*answerheaders += lpOutBuffer;

cyc:

	while (true)
	{
		bool ReadState = InternetReadFile(InRequest, DataBuffer, 1023, &BytesRead);

		if (BytesRead == NULL || ReadState == FALSE)
			break;

		DataBuffer[BytesRead] = 0;


		*answer += DataBuffer;

	}

	InternetCloseHandle(InSetup);
	InternetCloseHandle(InConnect);
	InternetCloseHandle(InRequest);

	return;
}


void Internet::Post(const char *hostname, const char *data, const char *headers, const char *optional, string *answer, string *answerheaders)
{
	DWORD BytesRead;
	DWORD dwSize = 0;

	char *lpOutBuffer = NULL;
	char DataBuffer[1024];

	HINTERNET InSetup, InConnect, InRequest;

	InSetup = InternetOpenA("Mozilla Firefox/5.0 (Windows NT 6.1; Win64; x64; rv:47.0)", INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, NULL);
	if (!InSetup)
		throw(29);

	InConnect = InternetConnectA(InSetup, hostname, INTERNET_DEFAULT_HTTPS_PORT, NULL, NULL, INTERNET_SERVICE_HTTP, NULL, NULL);
	if (!InConnect)
		throw(30);

	InRequest = HttpOpenRequestA(InConnect, "POST", data, NULL, NULL, NULL, INTERNET_FLAG_HYPERLINK | INTERNET_FLAG_IGNORE_CERT_CN_INVALID |
		INTERNET_FLAG_IGNORE_CERT_DATE_INVALID |
		INTERNET_FLAG_IGNORE_REDIRECT_TO_HTTP |
		INTERNET_FLAG_IGNORE_REDIRECT_TO_HTTPS |
		INTERNET_FLAG_NO_AUTH |
		INTERNET_FLAG_NO_CACHE_WRITE |
		INTERNET_FLAG_NO_UI |
		INTERNET_FLAG_PRAGMA_NOCACHE |
		INTERNET_FLAG_RELOAD | INTERNET_FLAG_SECURE, NULL);
	if (!InRequest)
		throw(31);


	DWORD lnght(0), optlnght(0);
	if (headers != NULL)
		lnght = strlen(headers);

	if (optional != NULL)
		optlnght = strlen(optional);

	BOOL isSend = HttpSendRequestA(InRequest, headers, lnght, (LPVOID)optional, optlnght);

	if (!isSend)
		throw(32);

retry:
	if (!HttpQueryInfo(InRequest, HTTP_QUERY_RAW_HEADERS_CRLF,
		(LPVOID)lpOutBuffer, &dwSize, NULL))
	{

		if (GetLastError() == ERROR_HTTP_HEADER_NOT_FOUND)
			goto cyc;
		else
		{
			if (GetLastError() == ERROR_INSUFFICIENT_BUFFER)
			{
				lpOutBuffer = new char[dwSize];

				goto retry;
			}
		}
	}

	*answerheaders += lpOutBuffer;

cyc:

	while (true)
	{
		bool ReadState = InternetReadFile(InRequest, DataBuffer, 1023, &BytesRead);

		if (BytesRead == NULL || ReadState == FALSE)
			break;

		DataBuffer[BytesRead] = 0;


		*answer += DataBuffer;

	}

	InternetCloseHandle(InSetup);
	InternetCloseHandle(InConnect);
	InternetCloseHandle(InRequest);

	return;
}

FFoxInternet::FFoxInternet(const char * hostname)
{
	host = hostname;
}

FFoxInternet::~FFoxInternet()
{
}

void FFoxInternet::AddHeader(const char *Row, const char *Dat)
{
	OutHeaders += Row;
	OutHeaders += ": ";
	OutHeaders += Dat;
	OutHeaders += "\r\n\r\n";

}

void FFoxInternet::ClearHeaders()
{
	OutHeaders.clear();

}

void FFoxInternet::Get(const char * hostdata, const char *method)
{
	InHeaders.clear();
	GotData.clear();

	DWORD BytesRead;
	DWORD dwSize = 0;

	char *lpOutBuffer = NULL;
	char DataBuffer[1024];

	HINTERNET InSetup, InConnect, InRequest;

	InSetup = InternetOpenA("Mozilla Firefox/5.0 (Windows NT 6.1; Win64; x64; rv:47.0)", INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, NULL);
	if (!InSetup)
		throw(29);

	InConnect = InternetConnectA(InSetup, host.c_str(), INTERNET_DEFAULT_HTTPS_PORT, NULL, NULL, INTERNET_SERVICE_HTTP, NULL, NULL);
	if (!InConnect)
		throw(30);

	InRequest = HttpOpenRequestA(InConnect, method, hostdata, NULL, NULL, NULL, INTERNET_FLAG_HYPERLINK | INTERNET_FLAG_IGNORE_CERT_CN_INVALID |
		INTERNET_FLAG_IGNORE_CERT_DATE_INVALID |
		INTERNET_FLAG_IGNORE_REDIRECT_TO_HTTP |
		INTERNET_FLAG_IGNORE_REDIRECT_TO_HTTPS |
		INTERNET_FLAG_NO_AUTH |
		INTERNET_FLAG_NO_CACHE_WRITE |
		INTERNET_FLAG_NO_UI |
		INTERNET_FLAG_PRAGMA_NOCACHE |
		INTERNET_FLAG_RELOAD | INTERNET_FLAG_SECURE, NULL);
	if (!InRequest)
		throw(31);


	DWORD lnght(0);
	BOOL isSend;

	if(!OutHeaders.empty())
		isSend = HttpSendRequestA(InRequest, OutHeaders.c_str(), OutHeaders.length(), NULL, 0);
	else
		isSend = HttpSendRequestA(InRequest, NULL, NULL, NULL, 0);

	if (!isSend)
		throw(32);

retry:
	if (!HttpQueryInfo(InRequest, HTTP_QUERY_RAW_HEADERS_CRLF,
		(LPVOID)lpOutBuffer, &dwSize, NULL))
	{

		if (GetLastError() == ERROR_HTTP_HEADER_NOT_FOUND)
			goto cyc;
		else
		{
			if (GetLastError() == ERROR_INSUFFICIENT_BUFFER)
			{
				lpOutBuffer = new char[dwSize];

				goto retry;
			}
		}
	}

	InHeaders += lpOutBuffer;

cyc:

	while (true)
	{
		bool ReadState = InternetReadFile(InRequest, DataBuffer, 1023, &BytesRead);

		if (BytesRead == NULL || ReadState == FALSE)
			break;

		DataBuffer[BytesRead] = 0;


		GotData += DataBuffer;

	}

	InternetCloseHandle(InSetup);
	InternetCloseHandle(InConnect);
	InternetCloseHandle(InRequest);

	return;

}

void FFoxInternet::Post(const char * hostdata)
{
}

const char * FFoxInternet::GetRecievedData()
{

	return GotData.c_str();
}

const char * FFoxInternet::GetRecievedHeaders()
{
	return InHeaders.c_str();
}



/////////////////////////////////////////////////////
//class FFoxUpdate
/////////////////////////////////////////////////////


void FFoxUpdate::connect()
{
}

void FFoxUpdate::closeConnection()
{
}

void FFoxUpdate::sendData(const char *data, DWORD lenght)
{
}

FFoxUpdate::FFoxUpdate(string szAddress, UINT32 dwport)
{
	sConnetction = INVALID_SOCKET;

}

FFoxUpdate::~FFoxUpdate()
{
}

void FFoxUpdate::CheckUpdates()
{


}
