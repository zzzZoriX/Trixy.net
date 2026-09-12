#pragma once

enum error_code {
    SUCCESS = 0,
    SEND_ABORT = 100,
    READ_ABORT = 101,
    TIMEOUT_ABORT = 102,
};