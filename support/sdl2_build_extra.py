# ruff: noqa
# type: ignore

Import("env", "projenv")

# TODO Can we remove unused targets? Like monitor which does not work with SDL2

exec_name = "${BUILD_DIR}/${PROGNAME}${PROGSUFFIX}"

# Override unused "upload" to execute compiled binary
# TODO This opens the GUI again on close, why?
from SCons.Script import AlwaysBuild
AlwaysBuild(env.Alias("upload", exec_name, exec_name))

# Add custom target to explorer
env.AddTarget(
    name = "execute",
    dependencies = exec_name,
    actions = exec_name,
    title = "Execute",
    description = "Build and execute",
    group="General"
)
