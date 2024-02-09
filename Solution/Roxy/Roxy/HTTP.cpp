#include "HTTP.h"

static bool bHttpIsInit = false;

// [HTTP::Init]:
int HTTP::Initialize()
{
    WSADATA wd;
    if (WSAStartup(MAKEWORD(2, 2), &wd) != 0 || LOBYTE(wd.wVersion) != 2 || HIBYTE(wd.wVersion) != 2)
    {
        return WSAGetLastError();
    }
    bHttpIsInit = true;
    return 0;
}
// [/HTTP::Init]


// [HTTP::Clear]:
int HTTP::Clear()
{
    if (bHttpIsInit)
    {
        if (WSACleanup() != 0)
        {
            return WSAGetLastError();
        }
        bHttpIsInit = false;
    }
    return 0;
}
// [/HTTP::Clear]


// [HTTP::Request::Run]:
bool HTTP::Request::Run()
{
    if (!bHttpIsInit)
    {
        return false;
    }

    SOCKET sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sock == INVALID_SOCKET)
    {
        iWSALastError = WSAGetLastError();
        return false;
    }

    char host[2048] = "localhost";
    char get[4096] = "/";

    char* p = strchr(__url, '/');
    if (p)
    {
        int _l = strlen(&(*p));
        if (_l < 4096 && _l > 0)
            strcpy(get, &(*p));
        _l = strlen(__url) - _l;
        if (_l > 0 && _l < 2048)
            strncpy(host, __url, _l);
    }
    else
    {
        int _l = strlen(__url);
        if (_l > 0 && _l < 2048)
            strcpy(host, __url);
    }
    char tmp[] =
        "GET %s HTTP/1.0\r\n"
        "User-Agent: Mozilla/5.0 (Windows NT 6.1; WOW64; rv:54.0) Gecko/20100101 Firefox/54.0\r\n"
        "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8\r\n"
        "Accept-Language: ru-RU,ru;q=0.8,en-US;q=0.5,en;q=0.3\r\n"
        "Connection: close\r\n"
        "Host: www.%s\r\n"
        "\r\n";
    int request_length = strlen(tmp) + strlen(__url) + 10;
    char request[request_length];
    memset(request, '\0', request_length);
    wsprintf(request, tmp, get, host);

    hostent* ph = gethostbyname(host);
    if (ph == NULL)
    {
        closesocket(sock);
        this->__last_error = WSAGetLastError();
        this->__error_in_func = 2;
        return false;
    }

    sockaddr_in sa;
    sa.sin_family = AF_INET;
    sa.sin_port = htons(this->__port);
    sa.sin_addr.s_addr = ((unsigned long**)ph->h_addr_list)[0][0];

    if (connect(sock, (const sockaddr*)&sa, sizeof(sa)))
    {
        closesocket(sock);
        this->__last_error = WSAGetLastError();
        this->__error_in_func = 3;
        return false;
    }

    if (send(sock, request, strlen(request), 0) == SOCKET_ERROR)
    {
        closesocket(sock);
        this->__last_error = WSAGetLastError();
        this->__error_in_func = 4;
        return false;
    }

    char response[4096] = { '\0' };
    int bytes_recv;

    while ((bytes_recv = recv(sock, response, 4096, 0)) > 0)
    {
        if (__print)
            __print(response);
        memset(response, '\0', 4096);
    }

    if (bytes_recv == SOCKET_ERROR)
    {
        closesocket(sock);
        this->__last_error = WSAGetLastError();
        this->__error_in_func = 5;
        return false;
    }

    shutdown(sock, SD_BOTH);
    closesocket(sock);
    return true;
}
// [/Http::HttpRequest::Run]
