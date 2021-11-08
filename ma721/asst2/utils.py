import yaml
from argparse import ArgumentParser


def build_parser():
    parser = ArgumentParser()
    parser.add_argument(
        "-ds", "--data_size", type=int, default=3670, help="Data size for training"
    )
    parser.add_argument(
        "-e", "--epochs", type=int, default=5, help="Number of epochs for training"
    )
    parser.add_argument(
        "-tbs", "--train_bs", type=int, default=32, help="Train batch size"
    )
    parser.add_argument(
        "-vbs", "--val_bs", type=int, default=32, help="Validation batch size"
    )
    parser.add_argument("-mt", "--model_type", type=int, default=0, help="Model Type")
    parser.add_argument(
        "-d", "--desc", type=str, default="", help="No description provided"
    )
    return parser.parse_args()


def read_yaml_file():
    with open("data.yml", "r") as stream:
        data = yaml.safe_load(stream)
    return data
