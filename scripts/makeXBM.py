import os
from PIL import Image
from PIL import ImageOps

print("It runs bitch")

for file in os.listdir("./img/"):
    im = Image.open("./img/" + file)
    inv = ImageOps.invert(im)
    bw = inv.convert(mode="1")

    filename = "./include/" + file.partition(".")[0] + ".h"
    bw.save(filename, "XBM")

    with open(filename, 'r') as file:
        fileData = file.read()

    fileData = fileData.replace('char', 'uint8_t')
    varName = os.path.basename(filename).partition(".")[0]
    varName = varName[0].lower() + varName[1:]
    fileData = fileData.replace(
        'im_bits',
        varName)

    with open(filename, 'w') as file:
        file.write(fileData)
