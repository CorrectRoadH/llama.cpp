#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include "common.h"  // 添加这行来引入 llama_tokens 类型

class TokenConverter {
private:
    std::unordered_map<int, int> qwen_to_ds_map;
    bool is_initialized = false;

public:
    TokenConverter(const std::string& json_path);
    bool initialize(const std::string& json_path);
    int convert(int qwen_id);
    llama_tokens convert_batch(llama_tokens qwen_ids);
};
