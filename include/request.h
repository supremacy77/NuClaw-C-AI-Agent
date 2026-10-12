#include<string>
#include<map>

struct Request {
    std::string method;//方法
    std::string path;//路径
    std::string query_string;//查询字符串
    std::string version;//版本
    std::map<std::string, std::string> headers;//请求头
    std::map<std::string, std::string> query_params;//查询参数
    std::string body;//请求体


    //获取请求头的便捷方法
    std::string get_header(const std::string& key) const {
        auto it = headers.find(key);
        if (it != headers.end()) {
            return it->second;
        }
        return "";
    }

    //获取查询参数的便捷方法
    std::string get_query_param(const std::string& key) const {
        auto it = query_params.find(key);
        if (it != query_params.end()) {
            return it->second;
        }
        return "";
    }

};