#ifndef PRO_PORT_H
#define PRO_PORT_H
#define SECTION(name,var,value,type) \
static type var __attribute__((used,__section__(name)))=value;
#endif
