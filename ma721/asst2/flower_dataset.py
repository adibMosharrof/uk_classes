from PIL import Image
from torch.utils.data import Dataset
import numpy as np
import torch


class FlowerDataset(Dataset):
    def __init__(self, data, tfms):
        super().__init__()
        self.data = data
        self.tfms = tfms

    def __len__(self):
        return len(self.data)

    def __getitem__(self, index) -> dict:
        row = self.data[index]
        img = np.array(Image.open(row["path"]))
        return {"img": self.tfms(img), "class": torch.tensor(row["class"])}
