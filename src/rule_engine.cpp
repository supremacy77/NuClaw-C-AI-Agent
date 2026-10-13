#include "rule_engine.h"
#include <algorithm>
#include <cctype>
#include <regex>
#include <sstream>
#include <iomanip>
#include <chrono>
#include <iostream>

static std::string to_lower(const std::string& s) {
    std::string s_lower = s;
    std::transform(s.begin(), s.end(), s_lower.begin(), [](unsigned char c) { return std::tolower(c); });
    return s_lower;
}

static std::string escape_json(const std::string& s) {
    std::string out;
    for(char c : s) {
        switch (c) {
            case '"': out += "\\\""; break;
            case '\\': out += "\\\\"; break;
            case '\b': out += "\\b"; break;
            case '\f': out += "\\f"; break;
            case '\n': out += "\\n"; break;
            case '\r': out += "\\r"; break;
            case '\t': out += "\\t"; break;
            default:
                if ('\x00' <= c && c <= '\x1f') {
                    out += "\\u";
                    std::stringstream ss;
                    ss << std::hex << std::setw(4) << std::setfill('0') << (int)c;
                    out += ss.str();
                } else {
                    out += c;
                }
        }
    }
    return out;
}


static std::string parse_datetime(const std::string& input) {
    // This is a placeholder for a more sophisticated datetime parsing logic.
    // For now, it simply returns the input string.
    return input;
}

void RuleEngine::register_intent(const Intent& intent) {
    intents.push_back(intent);
}

const Intent* RuleEngine::find_intent(const std::string& intent_name) const {
    for (const auto& intent : intents) {
        if (intent.name == intent_name) {
            return &intent;
        }
    }
    return nullptr;
}



void setup_intents(RuleEngine& engine) {
    // 意图 1：查询天气
    Intent weather_intent{
        .name = "query_weather",
        .keywords = {"天气", "温度", "下雨", "雪", "几度", "forecast", "weather"},
        .entities = {
            {
                .name = "location",
                .type = "keyword_list", 
                .pattern = "北京 上海 广州 深圳 杭州",
                .required = true
            },
            {
                .name = "date",
                .type = "regex",
                .pattern = "(今天|明天|后天)",
                .required = false
            }
        },
        .handler = [](const std::map<std::string, std::string>& entities) {
            std::string location = entities.count("location") ? 
                                   entities.at("location") : "本地";
            std::string date = entities.count("date") ? 
                              entities.at("date") : "今天";

            // 实际应该调用天气 API
            return date + location + "的天气是晴天，25°C";
        }
    };

    // 意图 2：查询时间
    Intent time_intent{
        .name = "query_time",
        .keywords = {"时间", "几点", "现在", "time", "clock"},
        .entities = {},  // 不需要提取实体
        .handler = [](const std::map<std::string, std::string>&) {
            auto now = std::chrono::system_clock::now();
            auto time = std::chrono::system_clock::to_time_t(now);
            std::stringstream ss;
            ss << std::put_time(std::localtime(&time), "%H:%M:%S");
            return "现在是 " + ss.str();
        }
    };

    // 意图 3：问候
    Intent greeting_intent{
        .name = "greeting",
        .keywords = {"你好", "您好", "Hello", "Hi", "在吗"},
        .entities = {},
        .handler = [](const std::map<std::string, std::string>&) {
            return "你好！有什么可以帮您的吗？";
        }
    };

    // 注册到引擎
    engine.register_intent(weather_intent);
    engine.register_intent(time_intent);
    engine.register_intent(greeting_intent);
}

std::optional<IntentResult> RuleEngine::recognize(const std::string& input) {
    float best_score = 0.0f;
    const Intent* best_intent = nullptr;
    std::map<std::string, std::string> best_entities;

    for (const auto& intent : intents) {
        float score = calculate_score(input, intent);
        if (score > best_score) {
            best_score = score;
            best_intent = &intent;
            best_entities = extract_entities(input, intent.entities);
        }
    }

    if (best_intent) {
        return IntentResult{best_intent->name, best_score, best_entities};
    }
    return std::nullopt;
}


float RuleEngine::calculate_score(const std::string& input, const Intent& intent) {
    float score = 0.0f;
    std::string lower_input = to_lower(input);
    for (const auto& keyword : intent.keywords) {
        if (lower_input.find(to_lower(keyword)) != std::string::npos) {
            score += 1.0f;
            if (lower_input == to_lower(keyword)) {
                score += 0.5f;   // 完整匹配加权
            }
        }
    }
    return score;   // ✅ 直接返回命中数，不再除关键词数
}


std::map<std::string, std::string> RuleEngine::extract_entities(const std::string& input, const std::vector<Entity>& entities) {
    std::map<std::string, std::string> extracted;

    for (const auto& entity : entities) {
        if (entity.type == "keyword_list") {
            std::regex pattern("\\b(" + entity.pattern + ")\\b", std::regex_constants::icase);
            std::smatch match;
            if (std::regex_search(input, match, pattern)) {
                extracted[entity.name] = match.str(1);
            }
        } else if (entity.type == "regex") {
            std::regex pattern(entity.pattern, std::regex_constants::icase);
            std::smatch match;
            if (std::regex_search(input, match, pattern)) {
                extracted[entity.name] = match.str(1);
            }
        }
    }

    return extracted;
}