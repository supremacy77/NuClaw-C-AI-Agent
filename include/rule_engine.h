#pragma once
#include <string>
#include <vector>
#include <map>
#include <functional>
#include <optional>

struct Entity {
    std::string name;
    std::string type;
    std::string pattern;
    bool required;
};


struct Intent {
    std::string name;
    std::vector<std::string> keywords;
    std::vector<Entity> entities;
    std::function<std::string(const std::map<std::string, std::string>&)> handler;

};


struct IntentResult {
    std::string intent;
    float confidence = 0.0f;
    std::map<std::string, std::string> entities;    
};


class RuleEngine {
public:
    void register_intent(const Intent& intent);
    std::optional<IntentResult> recognize(const std::string& input);
    const Intent* find_intent(const std::string& intent_name) const;
    

private:
    float calculate_score(const std::string& input, const Intent& intent);  
    std::vector<Intent> intents;
    std::map<std::string, std::string> extract_entities(const std::string& input, const std::vector<Entity>& entities);    
};

void setup_intents(RuleEngine& engine);



