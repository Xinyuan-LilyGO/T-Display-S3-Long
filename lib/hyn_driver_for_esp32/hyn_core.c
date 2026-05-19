#include "hyn_core.h"

static struct hyn_ts_data *hyn_data = NULL;
static struct hyn_chip_hw_info chip_hw;
void hyn_delay_ms(int cnt)
{
    vTaskDelay(cnt/portTICK_PERIOD_MS);
}

/**gpio ctl*/
int gpio_set_value(uint32_t gpio_id,bool vlue)
{
    gpio_set_level(gpio_id,vlue);
    return 0;
}   
bool gpio_get_value(uint32_t gpio_id) 
{
 return gpio_get_level(gpio_id);
}


static xQueueHandle gpio_evt_queue = NULL;

static void IRAM_ATTR gpio_isr_handler(void* arg)
{
    uint32_t gpio_num = (uint32_t) arg;
    xQueueSendFromISR(gpio_evt_queue, &gpio_num, NULL);
}

static void touch_int_handler(void* arg)
{
    uint32_t io_num;
    for(;;) {
        if(xQueueReceive(gpio_evt_queue, &io_num, portMAX_DELAY)) {
            int ret;
            hyn_data->hyn_irq_flg = 1;
            if(hyn_data->work_mode < DIFF_MODE){
                ret = hyn_data->hyn_fuc_used->tp_report(); //读点

                for(u8 i=0; i< hyn_data->rp_buf.rep_num; i++){   //根据配置修改坐标原点
                    if(hyn_data->plat_data.swap_xy){
                        u16 tmp = hyn_data->rp_buf.pos_info[i].pos_x;
                        hyn_data->rp_buf.pos_info[i].pos_x = hyn_data->rp_buf.pos_info[i].pos_y;
                        hyn_data->rp_buf.pos_info[i].pos_y = tmp;
                    }
                    if(hyn_data->plat_data.reverse_x)
                        hyn_data->rp_buf.pos_info[i].pos_x = hyn_data->plat_data.x_resolution-hyn_data->rp_buf.pos_info[i].pos_x;
                    if(hyn_data->plat_data.reverse_y)
                        hyn_data->rp_buf.pos_info[i].pos_y = hyn_data->plat_data.y_resolution-hyn_data->rp_buf.pos_info[i].pos_y;
                }
                // HYN_INFO("ret:%d num:%d xy:(%d,%d)",ret,hyn_data->rp_buf.rep_num,hyn_data->rp_buf.pos_info[0].pos_x,hyn_data->rp_buf.pos_info[0].pos_y);
            }
            hyn_data->rp_buf.report_need = REPORT_NONE;
        }
    }
}

void touch_init(int sda, int scl, int irq,int rst)
{
    int ret = 0;
    static struct hyn_ts_data ts_data;
    memset((void*)&ts_data,0,sizeof(ts_data));
    hyn_data = &ts_data;
	HYN_INFO(HYN_DRIVER_VERSION);
    chip_hw.sda = sda;
    chip_hw.scl  = scl;
    chip_hw.irq  = irq;
    chip_hw.rst  = rst;
/*************************************************************/
//    handle            chip types
// &cst66xx_fuc   /*suport 36xx 35xx 66xx 68xx 148E*/
// &cst36xxes_fuc /*suport 154es 3654es 3640es*/
// &cst3240_fuc   /*suport 3240 */
// &cst92xx_fuc   /*suport 9217、9220 */
// &cst3xx_fuc    /*suport 328 128 140 148 340 348*/
// &cst7xx_fuc    /*suport 726 826 830 836u*/
// &cst8xxT_fuc   /*suport 816t 816d 820 08C*/
// &cst226se_fuc  /*suport 226se 8922*/
/*************************************************************/
    hyn_data->hyn_fuc_used = &cst66xx_fuc;  //根据芯片型号赋值
    hyn_data->plat_data.max_touch_num = MAX_POINTS_REPORT;   //最大手指数
    hyn_data->plat_data.x_resolution = 180;  //x最大分辨率
    hyn_data->plat_data.y_resolution = 640;  //y最大分辨率
    hyn_data->plat_data.swap_xy = 0;         //xy坐标交换
    hyn_data->plat_data.reverse_x = 0;       //x坐标反向
    hyn_data->plat_data.reverse_y = 0;       //y坐标反向

    hyn_data->plat_data.irq_gpio = chip_hw.irq;    //中断脚配置 11 
    hyn_data->plat_data.reset_gpio = chip_hw.rst;  //rest脚配置 2

    //配置rst 脚为push-pullp输出模式，输出1
    gpio_config_t io_conf = {};
    io_conf.intr_type = GPIO_INTR_DISABLE;
    io_conf.mode = GPIO_MODE_OUTPUT;
    io_conf.pin_bit_mask = (1UL<<hyn_data->plat_data.reset_gpio);
    io_conf.pull_down_en = 0;
    io_conf.pull_up_en = 0;
    gpio_config(&io_conf);

    //初始化I2c master ,配置速率、master addr
    hyn_i2c_init(chip_hw.sda,chip_hw.scl); //sda 15 scl 10

    //触摸芯片初始化
    ret = hyn_data->hyn_fuc_used->tp_chip_init(hyn_data); 
    if(ret){
        HYN_ERROR("I2c NAk");
        //return;
    }
    HYN_INFO("IC_info fw_project_id:%04x ictype:%04x fw_ver:%x"\
					,hyn_data->hw_info.fw_project_id,hyn_data->hw_info.fw_chip_type,hyn_data->hw_info.fw_ver);

#if HYN_POWER_ON_UPDATA
    if(ts_data.need_updata_fw){
        hyn_data->fw_file_name[0] = 0; //use .h to updata
        hyn_data->hyn_fuc_used->tp_updata_fw(hyn_data->fw_updata_addr,hyn_data->fw_updata_len);
    }
#endif

    //配置 int脚为 输入pull up，开启gpio 下降沿中断
    io_conf.intr_type = GPIO_INTR_NEGEDGE;
    io_conf.pin_bit_mask = (1UL<<hyn_data->plat_data.irq_gpio);
    io_conf.mode = GPIO_MODE_INPUT;
    io_conf.pull_up_en = 1;
    gpio_config(&io_conf);
    //create a queue to handle gpio event from isr
    gpio_evt_queue = xQueueCreate(10, sizeof(uint32_t));
    //start gpio task
    xTaskCreate(touch_int_handler, "touch_int_handler", 2048, NULL, 10, NULL);
    //install gpio isr service
    gpio_install_isr_service(0);
    //hook isr handler for specific gpio pin
    gpio_isr_handler_add(hyn_data->plat_data.irq_gpio, gpio_isr_handler, (void*) hyn_data->plat_data.irq_gpio);
}

struct hyn_ts_data *get_touch_data(void)
{
    return hyn_data;
}


