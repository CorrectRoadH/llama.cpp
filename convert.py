import json


# 文件路径
ds_tokenizer = "./ds_tokenizer.json"
qwen_tokenizer = "./qwen_tokenizer.json"
output_dir = "./qwen_to_ds.json"
vocab_output = "./qwen_to_ds_vocab.txt"

# 加载tokenizer
ds_tokenizer_dict = {}
with open(ds_tokenizer, "r", encoding="utf-8") as f:
    ds_tokenizer_dict = json.load(f)

qwen_tokenizer_dict = {}
with open(qwen_tokenizer, "r", encoding="utf-8") as f:
    qwen_tokenizer_dict = json.load(f)

# 提取token内容和ID的映射
ds_content_to_id = {}
ds_id_to_content = {}
qwen_content_to_id = {}
qwen_id_to_content = {}

# 处理DS tokenizer
for token in ds_tokenizer_dict.get("added_tokens", []):
    content = token["content"]
    token_id = token["id"]
    ds_content_to_id[content] = token_id
    ds_id_to_content[token_id] = content

# 处理Qwen tokenizer
for token in qwen_tokenizer_dict.get("added_tokens", []):
    content = token["content"]
    token_id = token["id"]
    qwen_content_to_id[content] = token_id
    qwen_id_to_content[token_id] = content

#"Ġiterative": 58112,
# "ĠLevi": 58113,
# "Exists": 58114,
for value, id in qwen_tokenizer_dict["model"]["vocab"].items():
    qwen_content_to_id[value] = id
    qwen_id_to_content[id] = value

for value,id in ds_tokenizer_dict["model"]["vocab"].items():
    ds_content_to_id[value] = id
    ds_id_to_content[id] = value

# 创建转换表
# 表结构
# {
#     "tokens": [{"content": "content", "qwen_id": "0", "ds_id": "1"}, ...]
# }
conversion_table = {"tokens": []}

# 遍历所有Qwen的token ID
for qwen_id, qwen_content in qwen_id_to_content.items():
    # 查找对应的DS token ID (相同数字ID对应的内容)
    ds_id = ds_content_to_id.get(qwen_content)

    # 添加到转换表
    conversion_table["tokens"].append({
        "content": qwen_content,
        "qwen_id": qwen_id,
        "ds_id": ds_id
    })

# 保存转换表
with open(output_dir, "w", encoding="utf-8") as f:
    json.dump(conversion_table, f, ensure_ascii=False, indent=4)

