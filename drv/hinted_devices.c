#include <device.h>
#include <mips/malta-devhint.c>
#include <stdc.h>
#include <bus.h>


void construct_device_path(device_t *dev, char* buff, size_t buff_size) {
    // TODO: https://github.com/cahirwpz/mimiker/pull/491/ introduces name and unit fields for device
    if (dev == NULL) return;
    snprintf((buff), buff_size, "%s%d/%s", dev->name, dev->unit, buff);
    construct_device_path(dev->parent, buff, buff_size);
}

bool starts_with(const char* str, const char* prefix) {
    size_t prefix_len = strlen(prefix);
    return strncmp(prefix, str, prefix_len);
}

void bus_enumerate_hinted_children(device_t *bus) {
    char bus_path_buff[256]; // TODO: Is the size good?
    construct_device_path(bus, bus_path_buff, 256);
    size_t hints_len = sizeof(malta_hints) / sizeof(devhint_t);

    for (int i=0; i<hints_len; i++)
        if (starts_with(malta_hints[i].path, bus_path_buff))
            bus_hinted_child(bus, &malta_hints[i]);
}

static void generic_hinted_child(device_t *bus, devhint_t *devhint) {
    // TODO: include devhint_t
    // TODO: probably a wrong file for that
    device_t child;
    int start, count;
    int order;

    /* skeleton of a FreeBSD solution for isa_hintedd_child; needs to be adjusted */

    // TODO: do we have BUS_ADD_CHILD equivalent?
    child = BUS_ADD_CHILD(parent, order, name, unit);
    if (child == 0)
        return;

    resource_int_value(name, unit, "port", &start);
    resource_int_value(name, unit, "portsize", &count);
    if (start > 0 || count > 0)
        bus_set_resource(child, SYS_RES_IOPORT, 0, start, count);

    start = 0;
    count = 0;
    resource_int_value(name, unit, "maddr", &start);
    resource_int_value(name, unit, "msize", &count);

    if (start > 0 || count > 0)
        bus_set_resource(child, SYS_RES_MEMORY, 0, start, count);
    if (resource_int_value(name, unit, "irq", &start) == 0 && start > 0)
        bus_set_resource(child, SYS_RES_IRQ, 0, start, 1);
    if (resource_int_value(name, unit, "drq", &start) == 0 && start >= 0)
        bus_set_resource(child, SYS_RES_DRQ, 0, start, 1);

    return;
}

