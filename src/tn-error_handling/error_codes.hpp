#pragma once

enum error_code {
    SUCCESS = 0,
    SEND_SUCCESS = 1,
    READ_SUCCESS = 2,
    PING_SUCCESS = 3,
    SEND_ERROR = 100,
    READ_ERROR = 101,
    TIMEOUT_ERROR = 102,
    SOCK_ERROR = 103,
    PING_ERROR = 104
};