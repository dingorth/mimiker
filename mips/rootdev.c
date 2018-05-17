#define KL_LOG KL_DEV
#include <klog.h>
#include <mips/malta.h>
#include <mips/intr.h>
#include <bus.h>
#include <exception.h>
#include <pci.h>
#include <sysinit.h>
#include <rman.h>

typedef struct rootdev { void *data; } rootdev_t;

static inline rootdev_t *rootdev_of(device_t *dev) {
  return dev->instance;
}

static void rootdev_intr_setup(device_t *dev, unsigned num,
                               intr_handler_t *handler) {
  mips_intr_setup(handler, num);
}

static void rootdev_intr_teardown(device_t *dev, intr_handler_t *handler) {
  mips_intr_teardown(handler);
}

// should return resource*
static struct r_resource* rootdev_resource_alloc(device_t *dev, int type, int *rid,
  rman_res_t start, rman_res_t end, rman_res_t count, uint32_t flags){
    
  struct r_resource *r = NULL;
  return r;
}

extern pci_bus_driver_t gt_pci_bus;
device_t *gt_pci;

static struct rman rman_mem;
static struct rman rman_irq;

static int rootdev_attach(device_t *dev) {
  // rman all physical resoursec
  // rman MEM
  // rman IRQ
  rman_irq.rm_start = 0;
  rman_irq.rm_end = NUM_MIPS_IRQS - 1;
  rman_irq.desc = "Hardware IRQs";
  if(rman_init(&rman_irq) != 0 ||
    rman_manage_region(&rman_irq, 0, NUM_MIPS_IRQS - 1) != 0){
    panic("rman_irq");
  }

  rman_mem.rm_start = 0;
  rman_mem.rm_end = BUS_SPACE_MAXADDR;
  rman_mem.desc = "Memory addresses";
  if(rman_init(&rman_mem) != 0 ||
    rman_manage_region(&rman_mem, 0, BUS_SPACE_MAXADDR) != 0){
    panic("rman_mem");
  }


  gt_pci = device_add_child(dev);
  gt_pci->driver = &gt_pci_bus.driver;
  if (device_probe(gt_pci))
    device_attach(gt_pci);
  return 0;
}

static bus_driver_t rootdev_driver = {
  .driver =
    {
      .size = sizeof(rootdev_t),
      .desc = "MIPS platform root bus driver",
      .attach = rootdev_attach,
    },
  .bus =
    {
      .intr_setup = rootdev_intr_setup, 
      .intr_teardown = rootdev_intr_teardown,
      .resource_alloc = rootdev_resource_alloc
    }};

static device_t rootdev = (device_t){
  .children = TAILQ_HEAD_INITIALIZER(rootdev.children),
  .driver = (driver_t *)&rootdev_driver,
  .instance = &(rootdev_t){},
  .state = NULL,
};

static void rootdev_init(void) {
  device_attach(&rootdev);
}

SYSINIT_ADD(rootdev, rootdev_init, DEPS("mount_fs"));
