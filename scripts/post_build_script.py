
from os.path import join
from SCons.Script import AlwaysBuild, Builder, Default, DefaultEnvironment
from bin2uf2 import convert_to_uf2
Import("env")

def build_uf2(target, source, env):
  # Read the bin file
  binbuf = source[0].get_contents()
  
  # Convert bin to uf2
  uf2buf = convert_to_uf2(binbuf)

  # Write uf2 file
  with open(str(target[0]), mode='wb') as f:
    f.write(uf2buf)

  return None

# PROJECT_DIR = env["PROJECT_DIR"]
env.Append(
    BUILDERS=dict(
        BinToUF2=Builder(
            action=env.VerboseAction(build_uf2, "Building $TARGET"),
            suffix=".uf2"
        )
    )
)

target_uf2 = env.BinToUF2("$BUILD_DIR/${PROGNAME}.uf2", "$BUILD_DIR/${PROGNAME}.bin")
Default(target_uf2)