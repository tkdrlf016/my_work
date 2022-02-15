#include <iostream>
#include <string.h>
#include <json-c/json.h>

using namespace std;

int main()
{
    json_object *myObj = json_object_new_object();

    json_object_object_add(myObj, "ctrl_mode", json_object_new_int(0));

    json_object_object_add(myObj, "go_or_stop", json_object_new_int(0));

    json_object_object_add(myObj, "emergency", json_object_new_int(0));
    json_object_object_add(myObj, "chk_sum", json_object_new_int(0));

    json_object_object_add(myObj, "scenario", json_object_new_string("a scenario"));

    json_object_object_add(myObj, "pos_x", json_object_new_int(0));
    json_object_object_add(myObj, "pos_y", json_object_new_int(0));

    json_object_object_add(myObj, "ang", json_object_new_int(0));

    json_object_object_add(myObj, "spd", json_object_new_int(0));
    json_object_object_add(myObj, "acc", json_object_new_int(0));
    json_object_object_add(myObj, "yaw", json_object_new_int(0));
    json_object_object_add(myObj, "ahigh_level_errorcc", json_object_new_int(0));
    json_object_object_add(myObj, "high_level_error", json_object_new_int(0));
    json_object_to_file("sample.json",myObj);

    return 0;
}
