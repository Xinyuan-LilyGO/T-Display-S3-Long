import imp
import esptool
import gen_esp32part
from gen_esp32part import PartitionTable
import sys
import io
import csv
import spiffsgen
# argv = ['--chip ', 'esp32s3', ' merge_bin ', ' -o ', 'C:\Users\lewis\Desktop\esp32dev-camera\build\firmware_myenv_20220922_105433.bin', ' --flash_size ', '16MB', ' 0x0000 ', 'C:\Users\lewis\.platformio\packages\framework-arduinoespressif32@3.20003.220626\tools\sdk\esp32s3\bin\bootloader_qio_80m.bin', ' 0xe000 ', 'C:\Users\lewis\.platformio\packages\framework-arduinoespressif32@3.20003.220626\tools\partitions\boot_app0.bin', ' 0x8000 ', 'C:\Users\lewis\Desktop\esp32dev-camera\.pio\build\myenv\partitions.bin', ' 0x10000 ', 'C:\Users\lewis\Desktop\esp32dev-camera\.pio\build\myenv\firmware.bin']
# argv = ['--chip', 'esp32s3', 'merge_bin', '-o', 'C:\\Users\\lewis\\Desktop\\esp32dev-camera\\build\\firmware_myenv_20220922_105433.bin', '--flash_size', '16MB', ' 0x0000 ', 'C:\\Users\\lewis\\.platformio\\packages\\framework-arduinoespressif32@3.20003.220626\\tools\\sdk\\esp32s3\\bin\\bootloader_qio_80m.bin', ' 0xe000 ', 'C:\\Users\\lewis\\.platformio\\packages\\framework-arduinoespressif32@3.20003.220626\\tools\\partitions\\boot_app0.bin', ' 0x8000 ', 'C:\\Users\\lewis\\Desktop\\esp32dev-camera\\.pio\\build\\myenv\\partitions.bin', ' 0x10000 ', 'C:\\Users\\lewis\\Desktop\\esp32dev-camera\\.pio\\build\\myenv\\firmware.bin']

# argv = ['--chip', 'esp32s3', 'merge_bin', '-o', 'C:\\Users\\lewis\\Desktop\\esp32dev-camera\\build\\firmware_myenv_20220922_113419.bin', '--flash_size', '16MB', '0x0000', 'C:\\Users\\lewis\\.platformio\\packages\\framework-arduinoespressif32@3.20003.220626\\tools\\sdk\\esp32s3\\bin\\bootloader_qio_80m.bin', '0xe000', 'C:\\Users\\lewis\\.platformio\\packages\\framework-arduinoespressif32@3.20003.220626\\tools\\partitions\\boot_app0.bin', ' 0x8000 ', 'C:\\Users\\lewis\\Desktop\\esp32dev-camera\\.pio\\build\\myenv\\partitions.bin', '0x10000', 'C:\\Users\\lewis\\Desktop\\esp32dev-camera\\.pio\\build\\myenv\\firmware.bin']
# esptool.main(argv)



# argv = ['--input','C:\\Users\\lewis\\Desktop\\esp32dev-camera\\.pio\\build\\myenv\\partitions.bin','--quiet','--output','C:\\Users\\lewis\\Desktop\\esp32dev-camera\\firmware\\a.csv']
# gen_esp32part.main(argv)
# def getSpiffsOffset(file):
#     with open(file, 'r', newline='') as csvfile:
#         cr = csv.DictReader(csvfile)
#         for row in cr:
#             # print(row)
#             if row.get(' SubType') is not None and row[' SubType'] == 'spiffs':
#                 if row.get(' Offset') is not None:
#                     # print(row[' Offset'])
#                     return row[' Offset']
def _parse_size(value):
    if isinstance(value, int):
        return value
    elif value.isdigit():
        return int(value)
    elif value.startswith("0x"):
        return int(value, 16)
    elif value[-1].upper() in ("K", "M"):
        base = 1024 if value[-1].upper() == "K" else 1024 * 1024
        return int(value[:-1]) * base
    return value


def getSpiffsOffset(file):
    try:
        with open(file, 'r', newline='') as csvfile:
            cr = csv.DictReader(csvfile)
            for row in cr:
                # print(row)
                if row.get(' SubType') is not None and row[' SubType'] == 'spiffs':
                    # if row.get(' Offset') is not None:
                    #     # print(row[' Offset'])
                    #     return row[' Offset']
                    if row.get(' Size') is not None:
                        return  row.get(' Size')
    except:
        return 0

r = getSpiffsOffset('C:\\Users\\lewis\\Desktop\\esp32dev-camera\\build\\partitions.csv')
size = _parse_size(r)
print()


spiffsgen_commands = []
spiffsgen_commands.append(str(size))
spiffsgen_commands.append('../data')
spiffsgen_commands.append('spi.bin')
print(spiffsgen_commands)
spiffsgen.main(spiffsgen_commands)





