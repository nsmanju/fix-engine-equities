#pragma once
#include "fix_tags.h"
#include "fix_message.h"
#include <string>
#include <string_view>
#include <chrono>
#include <cstdio>
#include <ctime>

namespace FIX {

class FixBuilder {
    std::string buffer_;
    size_t bodyLenPos_ = 0;

public:
    FixBuilder() {
        buffer_.reserve(512);
    }

    FixBuilder& begin(std::string_view beginString = "FIX.4.4") {
        buffer_.clear();
        buffer_ += "8=";
        buffer_ += beginString;
        buffer_ += SOH;
        bodyLenPos_ = buffer_.size();
        buffer_ += "9=0000";
        buffer_ += SOH;
        return *this;
    }

    FixBuilder& set(int tag, std::string_view value) {
        buffer_ += std::to_string(tag);
        buffer_ += '=';
        buffer_ += value;
        buffer_ += SOH;
        return *this;
    }

    FixBuilder& set(int tag, int value) {
        return set(tag, std::to_string(value));
    }

    FixBuilder& set(int tag, double value) {
        return set(tag, std::to_string(value));
    }

    FixBuilder& setSendingTimeNow() {
        using namespace std::chrono;
        auto now = system_clock::now();
        auto now_t = system_clock::to_time_t(now);
        auto ms = duration_cast<milliseconds>(now.time_since_epoch()) % 1000;
        std::tm utc{};
        gmtime_r(&now_t, &utc);
        char buf[64];
        std::snprintf(buf, sizeof(buf), "%04d%02d%02d-%02d:%02d:%02d.%03d",
            utc.tm_year + 1900, utc.tm_mon + 1, utc.tm_mday,
            utc.tm_hour, utc.tm_min, utc.tm_sec, static_cast<int>(ms.count()));
        return set(Tags::SendingTime, std::string_view(buf));
    }

    std::string build() {
        size_t bodyStart = bodyLenPos_ + 7;
        size_t bodyLen = buffer_.size() - bodyStart;
        std::string header = buffer_.substr(0, bodyLenPos_);
        std::string body = buffer_.substr(bodyStart);
        header += "9=" + std::to_string(bodyLen) + SOH;
        std::string withoutChecksum = header + body;
        uint8_t cs = FixMessage::computeChecksum(withoutChecksum);
        char csBuf[8];
        std::snprintf(csBuf, sizeof(csBuf), "%03d", static_cast<int>(cs));
        std::string finalMsg = withoutChecksum + "10=" + csBuf + SOH;
        return finalMsg;
    }
};

} // namespace FIX
