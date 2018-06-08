#include <mips/malta.h>
#include <mips/intr.h>
#include <dev/i8259.h>
#include <dev/piixreg.h>
#include <dev/isareg.h>
#include <dev/gt64120reg.h>
#include <interrupt.h>
#include <pci.h>
#include <spinlock.h>
#include <stdc.h>
#include <klog.h>
#include <sysinit.h>
#include <bus.h>
#include <stdc.h>

typedef struct gt_pci_state {


} pci_isa_state_t;

static int isa_attach(device_t *isa){
    device_t *dev;
    isa->bus = DEV_BUS_ISA;

    dev = device_add_child(isa);
    dev->desc = "pit";

  dev = device_add_child(isa);
  dev->desc = "rtc";

  dev = device_add_child(isa);
  dev->desc = "atkbdc";

    return 0;
}

static resource_t *isa_resource_alloc(device_t *isab, device_t *dev,
                                         int type, int rid, rman_addr_t start,
                                         rman_addr_t end, rman_addr_t size,
                                         unsigned flags) {
    return bus_resource_alloc(dev, type, rid, start, end, size, flags);
}

static void isa_intr_setup(device_t *isab, unsigned irq,
                              intr_handler_t *handler) {
    bus_intr_setup(isab, irq, handler);
}

static int isa_probe(device_t *dev){
    if(dev->desc && strcmp(dev->desc, "isa")){
        return 1;
    }
    return 0;
}
                              
static void isa_intr_teardown(device_t *isab, intr_handler_t *handler){
    bus_intr_teardown(isab, handler);
}                              

static bus_driver_t pci_isa_bridge = {
  .driver = {.desc = "PCI-ISA bridge",
             .size = sizeof(pci_isa_state_t),
             .attach = isa_attach,
             .probe = isa_probe},
  .bus = {.intr_setup = isa_intr_setup,
          .intr_teardown = isa_intr_teardown,
          .resource_alloc = isa_resource_alloc}
};

DRIVER_ADD(pci_isa_bridge);
