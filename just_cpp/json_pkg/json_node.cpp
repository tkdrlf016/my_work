#include <iostream>
#include <string.h>
#include <json-c/json.h>

using namespace std;

int main()
{
    json_object *myObj = json_object_new_object();

    json_object_object_add(myObj, "이름", json_object_new_string("홍길동"));

    json_object_object_add(myObj, "나이", json_object_new_int(45));

    json_object_object_add(myObj, "기혼", json_object_new_boolean(1));
    
    json_object_to_file("sample.json",myObj);

    return 0;
}
