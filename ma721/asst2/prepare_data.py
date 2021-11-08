import os
from pathlib import Path
import yaml

data_path = Path("flower_photos")

classes = [x for x in data_path.iterdir() if x.is_dir()]

output = []

for i, c in enumerate(classes):
    imgs = [{"path": str(x), "class": i} for x in c.iterdir() if x.is_file()]
    output = output + imgs

with open("data.yml", "w") as out_file:
    yaml.dump(output, out_file)
