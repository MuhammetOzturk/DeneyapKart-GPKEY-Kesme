#include <freertos/task.h>

#define P(a) Serial.print(a)
#define PN(a) Serial.println(a)

TaskHandle_t xIntrHandle = NULL;

static void IRAM_ATTR kesme_fonksiyonu(void * arg){
    vTaskGenericNotifyGiveFromISR(xIntrHandle,0,(BaseType_t *) pdFALSE);
}

void vMyTask(void * parameters)
{
    for(;;){
        if(ulTaskNotifyTake(pdTRUE, portMAX_DELAY)){
            PN("kesme alindi.");
            while(gpio_get_level(GPIO_NUM_0) == 0){
                PN("basili");
                delay(200);
            }
        }
    }
}

void setup()
{
    Serial.begin(115200);
    xTaskCreate(
        vMyTask,
        "vMyTask",
        1000,
        NULL,
        10,
        &xIntrHandle);
    gpio_set_intr_type(GPIO_NUM_0,GPIO_INTR_NEGEDGE);
    gpio_install_isr_service(0);
    gpio_isr_handler_add(GPIO_NUM_0, kesme_fonksiyonu,NULL);

}

void loop()
{

}
