#include "HttpTransportEnumNames.h"

web_server::HttpTransportEnumNames::HttpTransportEnumNames() :
    protocolNamesRegex({{ Request::Protocol::HTTP_1_1, "HTTP/1.1"}, { Request::Protocol::HTTP_2, "HTTP/2" } }),
    protocolNames({{ Request::Protocol::HTTP_1_1, "HTTP/1.1"}, { Request::Protocol::HTTP_2, "HTTP/2" } }),
    requestMethodNames({ { Request::Method::GET, "GET" }, { Request::Method::POST, "POST" } }),
    statusReasonMessages(
        {
            { Response::StatusCode::Ok, "OK" },
            { Response::StatusCode::NotFound, "Not Found" },
            { Response::StatusCode::MethodNotAllowed, "Method Not Allowed" },
        }
    ) {}
