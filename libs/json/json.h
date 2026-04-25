enum type_kind {
  string,
  number,
  boolean,
  list,
  object,
};

struct type {
  enum type_kind kind;
  void *data;
};

typedef struct type JSONObject;
typedef struct type JSONList;
typedef struct type JSONNumber;
typedef struct type JSONString;
typedef struct type JSONBoolean;

JSONObject json_object(void *data);
JSONList json_list(void *data);
JSONNumber json_number(void *data);
JSONString json_string(void *data);
JSONBoolean json_boolean(void *data);

void foo() {
	struct user {
		char * name;
	};

	struct user u;

	JSONObject obj = json_object(&u);
}
