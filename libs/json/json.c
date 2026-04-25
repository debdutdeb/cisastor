#include <stdio.h>

#include "json.h"
#include "macros.h"

int deserialize(void *receiver, void* data) {
	struct type* json_object = cast(struct type*, receiver);
	if (object != json_object->kind) {
		// if the passed reciever is not a json object type, we do not know how to deserialize it;
		// error:
		perror("invalid_receiver_unable_to_deserialize");
		return 1;
	}

	return 0;
}
