#ifndef TABLE_SERIALIZER_H
#define TABLE_SERIALIZER_H

#include "tableData.h"

void serialize_prod(Product* prod, char* data);
void deserialize_prod(char* data, Product* prod);





#endif //TABLE_SERIALIZER_H
