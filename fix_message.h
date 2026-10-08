#pragma once
#include "fix_tags.h"
#include <string>
#include <string_view>
#include <charconv>
#include <optional>
#include <cstdint>

namespace FIX {

struct FieldView {
    int tag = 0;
    std::string_view value;

    std::optional<int> asInt() const {
        int v = 0;
        auto [ptr, ec] = std::from_chars(value.data(), value.data() + value.size(), v);
        if (ec == std::errc()) {
            return v;
        }
        return std::nullopt;
    }
};

class FixMessage {
    std::string_view raw_;
    FieldView fields_[128];
    size_t count_ = 0;
    bool valid_ = false;

public:
    static constexpr size_t MAX_FIELDS = 128;

    static uint8_t computeChecksum(std::string_view data) noexcept {
        uint32_t sum = 0;
        for (unsigned char c : data) {
            sum += c;
        }
        return static_cast<uint8_t>(sum % 256);
    }

    explicit FixMessage(std::string_view raw) : raw_(raw) {
        parse();
    }

    void parse() {
        count_ = 0;
        size_t pos = 0;
        while (pos < raw_.size() && count_ < MAX_FIELDS) {
            size_t eq = raw_.find('=', pos);
            if (eq == std::string_view::npos) {
                break;
            }
            std::string_view tagSlice = raw_.substr(pos, eq - pos);
            int tag = 0;
            auto [ptr, ec] = std::from_chars(tagSlice.data(), tagSlice.data() + tagSlice.size(), tag);
            if (ec != std::errc()) {
                break;
            }
            size_t soh = raw_.find(SOH, eq + 1);
            if (soh == std::string_view::npos) {
                break;
            }
            std::string_view value = raw_.substr(eq + 1, soh - eq - 1);
            fields_[count_++] = FieldView{tag, value};
            pos = soh + 1;
        }
        valid_ = true;
    }

    std::string_view get(int tag) const {
        for (size_t i = 0; i < count_; ++i) {
            if (fields_[i].tag == tag) {
                return fields_[i].value;
            }
        }
        return {};
    }

    bool isValid() const {
        return valid_;
    }

    std::string toHumanReadable() const {
        std::string out;
        out.reserve(raw_.size());
        for (char c : raw_) {
            if (c == SOH) {
                out.push_back('|');
            } else {
                out.push_back(c);
            }
        }
        return out;
    }

    size_t fieldCount() const {
        return count_;
    }
};

} // namespace FIX
