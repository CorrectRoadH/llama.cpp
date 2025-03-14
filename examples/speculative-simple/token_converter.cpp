#include "token_converter.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_map>
#include "../common/json.hpp"
#include "common.h"

using json = nlohmann::json;

TokenConverter::TokenConverter(const std::string& json_path) {
    initialize(json_path);
}

bool TokenConverter::initialize(const std::string& json_path) {
    try {
        // 读取JSON文件
        std::ifstream file(json_path);
        if (!file.is_open()) {
            std::cerr << "无法打开文件: " << json_path << std::endl;
            return false;
        }

        json data;
        file >> data;
        file.close();

        // 解析JSON并构建映射
        for (const auto& token : data["tokens"]) {
            int qwen_id = token["qwen_id"];

            // 如果ds_id为null，则使用1作为默认值
            int ds_id = token["ds_id"].is_null() ? 1 : token["ds_id"].get<int>();

            qwen_to_ds_map[qwen_id] = ds_id;
        }

        is_initialized = true;
        std::cout << "成功加载转换表，共 " << qwen_to_ds_map.size() << " 个映射" << std::endl;
        return true;
    } catch (const std::exception& e) {
        std::cerr << "初始化转换器时出错: " << e.what() << std::endl;
        return false;
    }
}

int TokenConverter::convert(int qwen_id) {
    if (!is_initialized) {
        std::cerr << "转换器未初始化" << std::endl;
        return -1;
    }

    auto it = qwen_to_ds_map.find(qwen_id);
    if (it != qwen_to_ds_map.end()) {
        return it->second;
    } else {
        std::cerr << "未找到Qwen ID: " << qwen_id << "，返回默认值1" << std::endl;
        return 1; // 默认返回1
    }
}

llama_tokens TokenConverter::convert_batch(llama_tokens qwen_ids) {
    llama_tokens ds_ids;
    ds_ids.reserve(qwen_ids.size());

    // std::cout << "Qwen输入: [";
    // for (size_t i = 0; i < qwen_ids.size(); i++) {
    //     if (i > 0) std::cout << ", ";
    //     std::cout << qwen_ids[i];
    // }
    // std::cout << "]" << std::endl;

    // for (int qwen_id : qwen_ids) {
    //     ds_ids.push_back(convert(qwen_id));
    // }

    // std::cout << "DS输出: [";
    // for (size_t i = 0; i < ds_ids.size(); i++) {
    //     if (i > 0) std::cout << ", ";
    //     std::cout << ds_ids[i];
    // }
    // std::cout << "]" << std::endl;

    return ds_ids;
}
