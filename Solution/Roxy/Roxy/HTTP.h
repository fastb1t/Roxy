#pragma once

#ifndef _HTTP_H_
#define _HTTP_H_

#include <string>
#include <winsock2.h>

#pragma comment(lib, "ws2_32.lib")

namespace HTTP {
    int Initialize();
    int Clear();

    class Request {
        typedef bool (*pfnPrint)(char*, size_t);
        pfnPrint fnPrint;
        int iWSALastError;
        std::string url;
        size_t port;

    public:
        Request(pfnPrint fnPrint = nullptr, std::string url = "", size_t port = 0) :
            fnPrint(fnPrint),
            url(url),
            port(port),
            iWSALastError(0)
        { }

        bool SetUrl(std::string url)
        {
            if (!url.empty())
            {
                this->url = url;
            }
            return !url.empty();
        }
        bool SetPort(size_t port) {
            if (port > 0 && port < 65535) {
                this->port = port;
                return true;
            }
            return false;
        }
        void SetPrintFunc(pfnPrint fnPrint) {
            this->fnPrint = fnPrint;
        }
        int GetWSALastError() {
            return iWSALastError;
        }
        bool Run();
    };

}

#endif
