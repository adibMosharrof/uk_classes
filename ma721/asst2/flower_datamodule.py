import pytorch_lightning as pl
from torchvision import transforms
from torch.utils.data import DataLoader, random_split
from flower_dataset import FlowerDataset
import torchvision.transforms as T


class FlowerDataModule(pl.LightningDataModule):
    def __init__(
        self,
        data_dir="flower_photos",
        data=None,
        data_size=3670,
        train_bs=256,
        val_bs=128,
        test_bs=128,
        model_type=0,
    ):
        super().__init__()
        self.data_dir = data_dir
        self.data_size = data_size
        self.train_bs = train_bs
        self.val_bs = val_bs
        self.test_bs = test_bs
        self.data = data
        self.model_type = model_type

    def setup(self, stage=None):
        ds = self._get_dataset()
        train_count = int(0.8 * self.data_size)
        val_count = int(0.2 * self.data_size)

        self.train_dataset, self.val_dataset = random_split(
            ds, (train_count, val_count)
        )

    def _get_dataset(self):
        tfm = transforms.Compose(
            [
                T.ToTensor(),
                T.Resize([180, 180]),
                # T.CenterCrop(128),
                T.Normalize((0.485, 0.456, 0.406), (0.229, 0.224, 0.225)),
            ]
        )
        return FlowerDataset(self.data, tfm)

    def train_dataloader(self):
        return DataLoader(
            self.train_dataset,
            batch_size=self.train_bs,
            num_workers=4,
            pin_memory=True,
            shuffle=False,
        )

    def val_dataloader(self):
        return DataLoader(
            self.val_dataset,
            batch_size=self.val_bs,
            num_workers=4,
            pin_memory=True,
            shuffle=False,
        )
