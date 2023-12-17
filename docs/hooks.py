# Changes to this file while `mkdocs serve` is running will cause infinite rebuilds.

from mkdocs.config.defaults import MkDocsConfig
import shutil


def copy_readme():
    in_path = "README.md"
    out_path = "docs/index.md"

    shutil.copyfile(in_path, out_path)

    with open(out_path, "r") as file:
        filedata = file.read()

    filedata = filedata.replace('src="docs/', 'src="')

    with open(out_path, "w") as file:
        file.write(filedata)


def on_pre_build(config: MkDocsConfig, **kwargs) -> None:
    # Limitation: the documentation will not be rebuilt if the README.md file is changed
    copy_readme()
