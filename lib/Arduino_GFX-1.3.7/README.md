# Arduino_GFX Add more support for display drivers!

This fork is Arduino_ The fork of GFX-1.3.7 has been added to this fork for the following display drivers：

| Screen type       | Driver chip         |  communication protocol   |  
| :------------------: | :------------------:| :-------------------------------:|   
| AMOLED             | SH8601              | QSPI                                  |   

# Add the following functions

| Function                                                               | Effects                  |
| :------------------------------------------------------------: | :------------------------------:| 
| void Display_Brightness(uint8_t brightness)            | Adjusting display brightness     |
| void SetContrast(uint8_t Contrast)      | Adjusting Display Contrast(Sunlight Readability Enhancement)     |

<br />

#### More examples added in "[more_examples](./more_examples)"
####  For other information, please consult [the original post](./README2.md)

