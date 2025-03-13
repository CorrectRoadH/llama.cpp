#pragma once

#include <string>
#include <vector>
#include <unordered_map>

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
