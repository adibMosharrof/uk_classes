import torch
import pathlib
from torchvision import transforms, models
import pytorch_lightning as pl
from torch import nn, optim
from torchmetrics import Accuracy
from torch.nn import functional as F


class FlowerModel(pl.LightningModule):
    def __init__(self, num_classes=5, data_size=None):
        super().__init__()
        self.save_hyperparameters()
        self.num_classes = num_classes
        self.criterion = nn.CrossEntropyLoss()
        self.train_acc = Accuracy()
        self.val_acc = Accuracy()
        self.test_acc = Accuracy()
        self.data_size = data_size
        self._init_model()

    def _init_model(self):
        self.cnn = nn.Sequential(
            nn.Conv2d(in_channels=3, out_channels=16, kernel_size=3, padding="same"),
            nn.ReLU(),
            # nn.BatchNorm2d(6),
            nn.Conv2d(in_channels=16, out_channels=32, kernel_size=3, padding="same"),
            nn.ReLU(),
            nn.MaxPool2d(2),
            # nn.BatchNorm2d(12),
            nn.Conv2d(in_channels=32, out_channels=64, kernel_size=3, padding="same"),
            nn.ReLU(),
            nn.MaxPool2d(2),
            # nn.BatchNorm2d(24),
            # nn.LazyLinear(128),
            nn.Flatten(),
            nn.Linear(129600, 128),
            nn.ReLU(),
            nn.Linear(128, self.num_classes),
        )

    def _shared_step(self, batch, acc, step_name):
        logits = self(batch)
        loss = self._get_loss(logits)
        preds = torch.argmax(logits["img_emb"], dim=1)

        accuracy = acc(preds, logits["class"]).detach()
        self.log(f"{step_name}/acc", accuracy, on_step=False, on_epoch=True)
        self.log(f"{step_name}/loss", loss, on_step=False, on_epoch=True)
        return {"loss": loss}

    def forward(self, data):
        data["img_emb"] = self.cnn(data["img"])
        return data

    def training_step(self, batch, index):
        return self._shared_step(batch, self.train_acc, "train")

    def validation_step(self, batch, index):
        return self._shared_step(batch, self.val_acc, "val")

    def test_step(self, batch, index):
        return self._shared_step(batch, self.test_acc, "test")

    # def training_epoch_end(self, outputs):
    #     self.log("data_size", self.data_size)

    def configure_optimizers(self):
        return optim.AdamW(self.parameters(), lr=1e-3, betas=(0.9, 0.99), eps=1e-5)

    def _get_loss(self, logits):
        loss = self.criterion(logits["img_emb"], logits["class"])
        return loss
