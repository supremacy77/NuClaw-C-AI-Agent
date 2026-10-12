#pragma once
#include <sstream>
#include "request.h"

class HttpParser {
public:
    bool parse(const std::string& data, size_t Length, Request& req) {
        buffer_ += data.substr(0, Length);
        size_t header_end = buffer_.find("\r\n\r\n");
        if (header_end == std::string::npos) {
            return false;  
        }

        std::string header_data = buffer_.substr(0, header_end);
        if (!parse_headers(header_data, req)) {
            return false;  
        }

        size_t body_start = header_end + 4;  
        auto it = req.headers.find("Content-Length");
        if (it != req.headers.end()) {
            size_t content_length = std::stoul(it->second);
            if (buffer_.size() < body_start + content_length) {
                return false;
            }

            req.body = buffer_.substr(body_start, content_length);
            buffer_.erase(0, body_start + content_length);
        } else {
            req.body.clear();
            buffer_.erase(0, body_start);
        }
        return true;
    }


private:
    bool parse_headers(const std::string& header_data, Request& req) {
        std::istringstream stream(header_data);
        std::string line;
        
        if(!std::getline(stream, line)) {
            return false;
        }

        if(!line.empty() && line.back() == '\r') {
            line.pop_back();
        }

        std::istringstream request_line(line);
        if (!(request_line >> req.method >> req.path >> req.version)) {
            return false;
        }

        size_t query_pos = req.path.find('?');
        if (query_pos != std::string::npos) {
            req.query_string = req.path.substr(query_pos + 1);
            req.path = req.path.substr(0, query_pos);
            parse_query_string(req.query_string, req.query_params);
        }

        while (std::getline(stream, line)) {
            if(!line.empty() && line.back() == '\r') {
                line.pop_back();
            }
            if (line.empty()) {
                break;  
            }
            size_t colon_pos = line.find(':');
            if (colon_pos != std::string::npos) {
                std::string key = line.substr(0, colon_pos);
                std::string value = line.substr(colon_pos + 1);
                trim(key);
                trim(value);
                req.headers[key] = value;
            }
        }
        return true;
    }




    void parse_query_string(const std::string& qs, std::map<std::string, std::string>& params) {
        std::istringstream stream(qs);
        std::string pair;
        while (std::getline(stream, pair, '&')) {
            size_t equal_pos = pair.find('=');
            if (equal_pos != std::string::npos) {
                std::string key = pair.substr(0, equal_pos);
                std::string value = pair.substr(equal_pos + 1);
                trim(key);
                trim(value);
                params[url_decoded(key)] = url_decoded(value);
            } else {
                trim(pair);
                params[url_decoded(pair)] = "";
            }
        }
    }




    std::string url_decoded(const std::string& s) {
        std::string result;
        for (size_t i = 0; i < s.length(); ++i) {
            if (s[i] == '%' && i + 2 < s.length()) {
                std::string hex = s.substr(i + 1, 2);
                char decoded_char = static_cast<char>(std::stoi(hex, nullptr, 16));
                result += decoded_char;
                i += 2;  
            } else if (s[i] == '+') {
                result += ' ';  
            } else {
                result += s[i];
            }
        }
        return result;
    }
    void trim(std::string& s) {
        size_t start = s.find_first_not_of(" \t\r\n");
        size_t end = s.find_last_not_of(" \t\r\n");
        if (start == std::string::npos || end == std::string::npos) {
            s = "";
        } else {
            s = s.substr(start, end - start + 1);  
        }
    }
    std::string buffer_;
};