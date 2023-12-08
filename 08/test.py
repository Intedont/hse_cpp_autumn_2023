import json
import ujson
import cjson

def main():
    # Тест
    json_str = '{"hello": 10, "world": "value"}'
    json_dict = json.loads(json_str)
    ujson_dict = ujson.loads(json_str)
    cjson_dict = cjson.loads(json_str)

    assert json_dict == ujson_dict == cjson_dict
    assert json_str == cjson.dumps(json_dict)
    print('First test passed')
    
    # Тест на пустом словаре  
    json_str = '{}'
    json_dict = json.loads(json_str)
    ujson_dict = ujson.loads(json_str)
    cjson_dict = cjson.loads(json_str)

    assert json_dict ==  cjson_dict == ujson_dict
    assert json_str == cjson.dumps(cjson.loads(json_str))
    print('Second test passed')

    # Тест с поехавшей разметкой
    json_str = '{         "1"    :      1     ,     "2"   :        2         }'
    json_dict = json.loads(json_str)
    ujson_dict = ujson.loads(json_str)
    cjson_dict = cjson.loads(json_str)
    
    assert json_dict == cjson_dict == ujson_dict
    assert json.dumps(json_dict) == cjson.dumps(json_dict)
    print('Third test passed')
    


if __name__ == "__main__":
    main()

