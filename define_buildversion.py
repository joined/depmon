# ruff: noqa
# type: ignore

Import("projenv")

import subprocess

branch_name = (
    subprocess.check_output(["git", "rev-parse", "--abbrev-ref", "HEAD"])
    .decode("utf-8")
    .strip()
)
commit_hash = (
    subprocess.check_output(["git", "rev-parse", "HEAD"]).decode("utf-8").strip()
)

build_version = f"{branch_name}-{commit_hash}"
print(f"Build version: {build_version}")

projenv.Append(CPPDEFINES=[("BUILD_VERSION", build_version)])
