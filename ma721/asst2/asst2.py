from pytorch_lightning import plugins

import mlflow.pytorch
import pytorch_lightning as pl
from pytorch_lightning.plugins import DDPPlugin
from datetime import datetime
from flower_dataset import FlowerDataset
import utils
from model import FlowerModel
from flower_datamodule import FlowerDataModule


if __name__ == "__main__":
    start_time = datetime.now()
    args = utils.build_parser()
    data = utils.read_yaml_file()
    num_classes = 5
    flower_dm = FlowerDataModule(
        data_size=args.data_size,
        train_bs=args.train_bs,
        data=data,
        val_bs=args.val_bs,
        model_type=args.model_type,
    )
    model = FlowerModel(num_classes=num_classes, data_size=args.data_size)

    trainer = pl.Trainer(
        gpus=-1,
        plugins=DDPPlugin(find_unused_parameters=False),
        max_epochs=args.epochs,
        precision=16,
    )

    flower_dm.setup()
    batch = next(iter(flower_dm.train_dataloader()))

    model.forward(batch)
    mlflow.pytorch.autolog()
    with mlflow.start_run() as run:
        trainer.fit(model, flower_dm)
        # trainer.test(model, flower_dm)

    exec_time = datetime.now() - start_time
    print(f"Execution time : {exec_time}")
