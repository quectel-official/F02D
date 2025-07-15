#!/usr/bin/env python

import os
import sys
import json
import shutil
import argparse

project = sys.argv[1]
loguart = sys.argv[2]

out_json_2M = {
    "magic": "RT-Thread",
    "version": "0.1",
    "count": 2,
    "section": [
        {
            "firmware": "bootloader.bin",
            "version": "2M.1220",
            "partition": "bootloader",
            "start_addr": "0x00000000",
            "size": "65280"
        },
        {
            "firmware": project.lower()+".bin",
            "version": "2M.1220",
            "partition": "app",
            "start_addr": "0x00011000",
            "size": "1156K"
        }
    ]
}

full_image_name = "all_2M.1220.bin"
uart_image_name = project.lower()+"_uart_2M.1220.bin"
bootloader_image_name = "bootloader.bin"

if loguart == 'uart1':
	bootloader_str = "../../ql_build/bootloader/bootloader1.bin"
else:
	bootloader_str = "../../ql_build/bootloader/bootloader2.bin"

firmware_str = "../../ql_build/gccout/"+project.lower()+".bin"

out_path = "tmp.json"

out_json_2M["section"][0]["firmware"] = bootloader_str
out_json_2M["section"][1]["firmware"] = firmware_str
out_json_2M = json.dumps(out_json_2M, sort_keys=True, indent=4)

print(out_json_2M)
with open(str(out_path), "w") as f:
    f.write(out_json_2M)

if 'linux' in sys.platform:
    os.system("./beken_packager {}".format(out_path))
elif sys.platform == 'win32':
    os.system(os.getcwd()+"/beken_packager.exe {}".format(out_path))
    
shutil.move(full_image_name, "../../ql_out/" + full_image_name)
shutil.move(uart_image_name, "../../ql_out/" + uart_image_name)
shutil.copy(bootloader_str,  "../../ql_out/" + bootloader_image_name)
os.remove(out_path)
