import mkdocs_gen_files


def generate_index():
    in_path = "README.md"
    out_path = "index.md"

    with open(in_path, "r") as file:
        filedata = file.read()

    filedata = filedata.replace('src="docs/', 'src="')

    with mkdocs_gen_files.open(out_path, "w") as file:
        file.write(filedata)


generate_index()
