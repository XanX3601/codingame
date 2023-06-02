import logging
import pathlib
import pyperclip
import rich.logging
import rich.traceback
import rich_click as click
import string


@click.group()
def cli():
    pass


@cli.command()
@click.argument("project")
def cpp_merge(project):
    """Merges a cpp project in a single file"""
    logging.info("cpp merge {project}".format(project=project))

    project_directory_path = pathlib.Path(project)

    include_directory_path = project_directory_path / "include"
    src_directory_path = project_directory_path / "src"

    # find headers and their class *************************
    header_file_paths = []
    class_name_to_header_file_path = {}

    for header_file_path in include_directory_path.rglob("*.h"):
        header_file_paths.append(header_file_path)

        with header_file_path.open("r") as header_file:
            for line in header_file:
                line = line.strip()

                if line.startswith("class"):
                    line = line.split()
                    class_name = line[1]

                    class_name = ''.join(
                        c for c in class_name if c not in string.punctuation
                    )

                    class_name_to_header_file_path[class_name] = header_file_path

                elif line.startswith("enum class"):
                    line = line.split()
                    class_name = line[2]

                    class_name = ''.join(
                        c for c in class_name if c not in string.punctuation
                    )

                    class_name_to_header_file_path[class_name] = header_file_path

    # for each header, find their order ********************
    header_file_path_to_predecessors = {}

    for header_file_path in header_file_paths:
        predecessors = set()

        with header_file_path.open("r") as header_file:
            for line in header_file:
                line = line.strip()

                for class_name, hfp in class_name_to_header_file_path.items():
                    if class_name in line and hfp != header_file_path:
                        predecessors.add(class_name_to_header_file_path[class_name])

                header_file_path_to_predecessors[header_file_path] = predecessors

    # order headers ****************************************
    header_file_paths_ordered = []
    header_file_paths_processed = set()

    while len(header_file_paths_ordered) != len(header_file_paths):
        found_next_header = False
        header_file_path_index = 0

        while not found_next_header:
            header_file_path = header_file_paths[header_file_path_index]

            predecessors = header_file_path_to_predecessors[header_file_path]
            if not predecessors and header_file_path not in header_file_paths_processed:
                found_next_header = True

            header_file_path_index += 1

        for predecessors in header_file_path_to_predecessors.values():
            if header_file_path in predecessors:
                predecessors.remove(header_file_path)

        header_file_paths_processed.add(header_file_path)
        header_file_paths_ordered.append(header_file_path)

    # find source files ***********************************
    source_file_paths = []

    for source_file_path in src_directory_path.rglob("*.cpp"):
        source_file_paths.append(source_file_path)

    # generate output text *********************************
    output_lines = []

    exclude_patterns = [
        '#include <{}'.format(project)
    ]
    
    file_paths_to_copy = header_file_paths_ordered + source_file_paths
    for file_path in file_paths_to_copy:
        with file_path.open('r') as file:
            for line in file:
                if all([p not in line for p in exclude_patterns]):
                    output_lines.append(line)

    output = ''.join(output_lines)

    # copy output in clipboard *****************************
    pyperclip.copy(output)

    logging.info('merge copied')

@cli.command()
@click.argument("file")
def python_copy(file):
    logging.info(f"python copy {file}")

    file_path = pathlib.Path(file)

    with file_path.open("r") as file:
        lines = [line for line in file]

    output = ''.join(lines)

    pyperclip.copy(output)
    logging.info("file copied")


if __name__ == "__main__":
    rich.traceback.install()

    logging_format = "%(message)s"
    logging.basicConfig(
        level="NOTSET",
        format=logging_format,
        datefmt="[%X]",
        handlers=[rich.logging.RichHandler()],
    )

    cli()
