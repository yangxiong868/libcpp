#include <rapidjson/document.h>
#include <rapidjson/prettywriter.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>
#include <iostream>
#include <string>

// rapidjson-1.1.0

std::string JsonToString(const rapidjson::Value &json) {
  rapidjson::StringBuffer str_buf;
  rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(str_buf);
  json.Accept(writer);
  return str_buf.GetString();
}

bool JsonCheckField(const rapidjson::Value &json, const char *field_key,
                    bool (rapidjson::Value::*validator)() const,
                    bool is_required) {
  if (!json.HasMember(field_key)) {
    if (is_required) {
      std::cout << "Invalid json: " << JsonToString(json) << " , miss key "
                << field_key << std::endl;
    }
    return false;
  }

  if (!(json[field_key].*validator)()) {
    std::cout << "Invalid json: " << JsonToString(json) << " , invalid key "
              << field_key << std::endl;

    return false;
  }

  return true;
}

bool JsonParse(const std::string &data) {
  rapidjson::Document doc;
  if (doc.Parse(data.c_str()).HasParseError()) {
    std::cout << "Failed to parse json string: " << data
              << ", error is: " << doc.GetParseError() << std::endl;
    return false;
  }

  std::cout << "JsonParse data:" << std::endl;
  // Int64
  if (JsonCheckField(doc, "Int64", &rapidjson::Value::IsInt64, true)) {
    std::cout << "Int64 = " << doc["Int64"].GetInt64() << std::endl;
  }

  // Object
  if (JsonCheckField(doc, "Object", &rapidjson::Value::IsObject, true)) {
    const rapidjson::Value &object = doc["Object"];

    if (JsonCheckField(object, "name", &rapidjson::Value::IsString, true)) {
      std::cout << "Object.name = " << object["name"].GetString() << std::endl;
    }
    if (JsonCheckField(object, "age", &rapidjson::Value::IsInt, true)) {
      std::cout << "Object.age = " << object["age"].GetInt() << std::endl;
    }
  }
  // StringArray
  if (JsonCheckField(doc, "StringArray", &rapidjson::Value::IsArray, true)) {
    const rapidjson::Value &array = doc["StringArray"];
    size_t len = array.Size();
    for (size_t i = 0; i < len; i++) {
      std::cout << "StringArray[" << i << "] = " << array[i].GetString()
                << std::endl;
    }
  }

  // ObjectArray
  if (JsonCheckField(doc, "ObjectArray", &rapidjson::Value::IsArray, true)) {
    const rapidjson::Value &array = doc["ObjectArray"];
    size_t len = array.Size();
    for (size_t i = 0; i < len; i++) {
      const rapidjson::Value &object = array[i];
      if (object.IsObject()) {
        std::cout << "ObjectArray[" << i << "]:";
        if (JsonCheckField(object, "name", &rapidjson::Value::IsString, true)) {
          std::cout << "name = " << object["name"].GetString();
        }
        if (JsonCheckField(object, "age", &rapidjson::Value::IsInt, true)) {
          std::cout << ", age = " << object["age"].GetInt() << std::endl;
        }
      }
    }
  }

  return true;
}

void JsonSerialize(std::string *data) {
  rapidjson::StringBuffer str_buf;
  rapidjson::Writer<rapidjson::StringBuffer> writer(str_buf);

  writer.StartObject();

  // Int64
  writer.Key("Int64");
  writer.Int64(123456789);

  // Object
  writer.Key("Object");
  writer.StartObject();
  writer.Key("name");
  writer.String("zhangsan");
  writer.Key("age");
  writer.Int(28);
  writer.EndObject();

  // StringArray
  writer.Key("StringArray");
  writer.StartArray();
  writer.String("one");
  writer.String("tow");
  writer.String("three");
  writer.EndArray();

  // ObjectArray
  writer.Key("ObjectArray");
  writer.StartArray();
  for (int i = 0; i < 3; i++) {
    writer.StartObject();
    writer.Key("name");
    writer.String("wangwu");
    writer.Key("age");
    writer.Int(20 + i);
    writer.EndObject();
  }
  writer.EndArray();

  writer.EndObject();

  *data = str_buf.GetString();
  std::cout << "JsonSerialize data: " << *data << std::endl;
}

int main(int argc, char *argv[]) {
  std::string data;

  JsonSerialize(&data);

  JsonParse(data);

  return 0;
}
// result
// json_write data:
// {"Int64":123456789,"Object":{"name":"zhangsan","age":28},"StringArray":["one","tow","three"],"ObjectArray":[{"name":"wangwu","age":20},{"name":"wangwu","age":21},{"name":"wangwu","age":22}]}
